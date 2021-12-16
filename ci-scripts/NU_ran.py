#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *      http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *      contact@openairinterface.org
# */
#---------------------------------------------------------------------
# Python for CI of OAI-eNB + COTS-UE
#
#   Required Python Version
#     Python 3.x
#
#   Required Python Package
#     pexpect
#---------------------------------------------------------------------

#-----------------------------------------------------------
# Import
#-----------------------------------------------------------
import sys              # arg
import re               # reg
import logging
import os
from os import path
import time
from multiprocessing import Process, Lock, SimpleQueue
import yaml


#-----------------------------------------------------------
# OAI Testing modules
#-----------------------------------------------------------
import sshconnection as SSH
import helpreadme as HELP
import constants as CONST

#-----------------------------------------------------------
# Class Declaration
#-----------------------------------------------------------
class RANManagement():

	def __init__(self):
		
		self.prematureExit = False
		self.ranRepository = ''
		self.ranBranch = ''
		self.ranAllowMerge = False
		self.ranCommitID = ''
		self.ranTargetBranch = ''
		self.eNBIPAddress = ''
		self.eNBUserName = ''
		self.eNBPassword = ''
		self.eNBSourceCodePath = ''
		self.eNB1IPAddress = ''
		self.eNB1UserName = ''
		self.eNB1Password = ''
		self.eNB1SourceCodePath = ''
		self.eNB2IPAddress = ''
		self.eNB2UserName = ''
		self.eNB2Password = ''
		self.eNB2SourceCodePath = ''
		self.Build_eNB_args = ''
		self.backgroundBuild = False
		self.backgroundBuildTestId = ['', '', '']
		self.Build_eNB_forced_workspace_cleanup = False
		self.Initialize_eNB_args = ''
		self.imageKind = ''
		self.air_interface = ['', '', ''] #changed from 'lte' to '' may lead to side effects in main
		self.eNB_instance = 0
		self.eNB_serverId = ['', '', '']
		self.eNBLogFiles = ['', '', '']
		self.eNBOptions = ['', '', '']
		self.eNBmbmsEnables = [False, False, False]
		self.eNBstatuses = [-1, -1, -1]
		self.flexranCtrlInstalled = False
		self.flexranCtrlStarted = False
		self.flexranCtrlDeployed = False
		self.flexranCtrlIpAddress = ''
		self.testCase_id = ''
		self.epcPcapFile = ''
		self.runtime_stats= ''
		self.datalog_rt_stats={}
		self.eNB_Trace = '' #if 'yes', Tshark will be launched at initialization
		self.USRPIPAddress = ''
		self.runStage = ''  # For NU CI - 'Pre' or 'Post'



#-----------------------------------------------------------
# RAN management functions
#-----------------------------------------------------------
	def BuildeNB(self, HTML):
		if self.ranRepository == '' or self.ranBranch == '' or self.ranCommitID == '' or self.runStage == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter')

		if self.runStage == 'Post':
			HTML.CreateHtmlTestRow(self.Build_eNB_args, 'OK', CONST.ALL_PROCESSES_OK)
			return

		# Check if we build an 5G-NR gNB or an LTE eNB or an OCP eNB
		result = re.search('--eNBocp', self.Build_eNB_args)
		if result is not None:
			self.air_interface[self.eNB_instance] = 'ocp-enb'
		else:
			result = re.search('--RU', self.Build_eNB_args)
			if result is not None:
				self.air_interface[self.eNB_instance] = 'oairu'
			else:
				result = re.search('--gNB', self.Build_eNB_args)
				if result is not None:
					self.air_interface[self.eNB_instance] = 'nr-softmodem'
				else:
					self.air_interface[self.eNB_instance] = 'lte-softmodem'

		self.testCase_id = HTML.testCase_id

		if (self.ranAllowMerge):
			if self.ranTargetBranch == '':
				if (self.ranBranch != 'develop') and (self.ranBranch != 'origin/develop'):
					#sed -i '/line: \"OAI/ a \  when: \{\{ oai_version \}\} != "develop" and \{\{ oai_version \}\} != "origin\/develop"' ../colosseum-cm/ansible/build-oai.yml
					os.system('sed -i "/line: \"OAI/ a \  when: {{ oai_version }} != \"develop\" and {{ oai_version }} != \"origin\/develop\"" ../colosseum-cm/ansible/build-oai.yml')
					os.system('sed -i "/line: \"OAI/ a \  command: git merge --ff origin\/develop -m \"Temporary merge for CI\"" ../colosseum-cm/ansible/build-oai.yml')
					os.system('sed -i "/line: \"OAI/ a \  delegate_to: \"{{ hostname }}\"" ../colosseum-cm/ansible/build-oai.yml')
					os.system('sed -i "/line: \"OAI/ a \- name: Check merge with develop if MR" ../colosseum-cm/ansible/build-oai.yml')
					os.system('sed -i "/line: \"OAI/ a  " ../colosseum-cm/ansible/build-oai.yml')
			else:
				logging.debug('Merging with the target branch: ' + self.ranTargetBranch)
				merge_command = 'git merge --ff origin/' + self.ranTargetBranch + ' -m "Temporary merge for CI"'
				os.system('sed -i "/line: \"OAI/ a \  when: {{ oai_version }} != \"develop\" and {{ oai_version }} != \"origin\/develop\"" ../colosseum-cm/ansible/build-oai.yml')
				os.system('sed -i "/line: \"OAI/ a \ ' + merge_command + ' " ../colosseum-cm/ansible/build-oai.yml')
				os.system('sed -i "/line: \"OAI/ a \  delegate_to: \"{{ hostname }}\"" ../colosseum-cm/ansible/build-oai.yml')
				os.system('sed -i "/line: \"OAI/ a \- name: Check merge with develop if MR" ../colosseum-cm/ansible/build-oai.yml')
				os.system('sed -i "/line: \"OAI/ a  " ../colosseum-cm/ansible/build-oai.yml')

	def WaitBuildeNBisFinished(self, HTML):
		if self.eNB_serverId[self.eNB_instance] == '0':
			lIpAddr = self.eNBIPAddress
			lUserName = self.eNBUserName
			lPassWord = self.eNBPassword
			lSourcePath = self.eNBSourceCodePath
		elif self.eNB_serverId[self.eNB_instance] == '1':
			lIpAddr = self.eNB1IPAddress
			lUserName = self.eNB1UserName
			lPassWord = self.eNB1Password
			lSourcePath = self.eNB1SourceCodePath
		elif self.eNB_serverId[self.eNB_instance] == '2':
			lIpAddr = self.eNB2IPAddress
			lUserName = self.eNB2UserName
			lPassWord = self.eNB2Password
			lSourcePath = self.eNB2SourceCodePath
		if lIpAddr == '' or lUserName == '' or lPassWord == '' or lSourcePath == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter')
		logging.debug('Waiting for end of build on server: ' + lIpAddr)
		mySSH = SSH.SSHConnection()
		mySSH.open(lIpAddr, lUserName, lPassWord)
		count = 40
		buildOAIprocess = True
		while (count > 0) and buildOAIprocess:
			mySSH.command('ps aux | grep --color=never build_ | grep -v grep', '\$', 6)
			result = re.search('build_oai', mySSH.getBefore())
			if result is None:
				buildOAIprocess = False
			else:
				count -= 1
				time.sleep(30)
		mySSH.close()
		self.checkBuildeNB(lIpAddr, lUserName, lPassWord, lSourcePath, self.backgroundBuildTestId[int(self.eNB_instance)], HTML)

	def checkBuildeNB(self, lIpAddr, lUserName, lPassWord, lSourcePath, testcaseId, HTML):
		HTML.testCase_id=testcaseId

		mySSH = SSH.SSHConnection()
		mySSH.open(lIpAddr, lUserName, lPassWord)
		mySSH.command('cd ' + lSourcePath + '/cmake_targets', '\$', 3)
		mySSH.command('ls ran_build/build', '\$', 3)
		mySSH.command('ls ran_build/build', '\$', 3)

		#check if we have the build corresponding to the air interface keywords (nr-softmode, lte-softmodem, ocp-enb)
		logging.info('CHECK Build with IP='+lIpAddr+' SourcePath='+lSourcePath)
		result = re.search(self.air_interface[self.eNB_instance], mySSH.getBefore())
		if result is None:
			buildStatus = False #if not, build failed
		else:
			buildStatus = True 
			# Generating a BUILD INFO file
			mySSH.command('echo "SRC_BRANCH: ' + self.ranBranch + '" > ../LAST_BUILD_INFO.txt', '\$', 2)
			mySSH.command('echo "SRC_COMMIT: ' + self.ranCommitID + '" >> ../LAST_BUILD_INFO.txt', '\$', 2)
			if (self.ranAllowMerge):
				mySSH.command('echo "MERGED_W_TGT_BRANCH: YES" >> ../LAST_BUILD_INFO.txt', '\$', 2)
				if self.ranTargetBranch == '':
					mySSH.command('echo "TGT_BRANCH: develop" >> ../LAST_BUILD_INFO.txt', '\$', 2)
				else:
					mySSH.command('echo "TGT_BRANCH: ' + self.ranTargetBranch + '" >> ../LAST_BUILD_INFO.txt', '\$', 2)
			else:
				mySSH.command('echo "MERGED_W_TGT_BRANCH: NO" >> ../LAST_BUILD_INFO.txt', '\$', 2)
				
				
		mySSH.command('mkdir -p build_log_' + testcaseId, '\$', 5)
		mySSH.command('mv log/* ' + 'build_log_' + testcaseId, '\$', 5)
		mySSH.command('mv compile_oai_enb.log ' + 'build_log_' + testcaseId, '\$', 5)
		if self.eNB_serverId[self.eNB_instance] != '0':
			mySSH.command('cd cmake_targets', '\$', 5)
			mySSH.command('if [ -e tmp_build' + testcaseId + '.zip ]; then rm -f tmp_build' + testcaseId + '.zip; fi', '\$', 5)
			mySSH.command('zip -r -qq tmp_build' + testcaseId + '.zip build_log_' + testcaseId, '\$', 5)
			mySSH.close()
			if (os.path.isfile('./tmp_build' + testcaseId + '.zip')):
				os.remove('./tmp_build' + testcaseId + '.zip')
			mySSH.copyin(lIpAddr, lUserName, lPassWord, lSourcePath + '/cmake_targets/tmp_build' + testcaseId + '.zip', '.')
			if (os.path.isfile('./tmp_build' + testcaseId + '.zip')):
				mySSH.copyout(self.eNBIPAddress, self.eNBUserName, self.eNBPassword, './tmp_build' + testcaseId + '.zip', self.eNBSourceCodePath + '/cmake_targets/.')
				os.remove('./tmp_build' + testcaseId + '.zip')
				mySSH.open(self.eNBIPAddress, self.eNBUserName, self.eNBPassword)
				mySSH.command('cd ' + self.eNBSourceCodePath + '/cmake_targets', '\$', 5)
				#-qq quiet / -u update orcreate files
				mySSH.command('unzip -o -u -qq -DD tmp_build' + testcaseId + '.zip', '\$', 5)
				mySSH.command('rm -f tmp_build' + testcaseId + '.zip', '\$', 5)
				mySSH.close()
		else:
			mySSH.close()

		#generate logging info depending on buildStatus and air interface
		if buildStatus:
			logging.info('\u001B[1m Building OAI ' + self.air_interface[self.eNB_instance] + ' Pass\u001B[0m')
			HTML.CreateHtmlTestRow(self.Build_eNB_args, 'OK', CONST.ALL_PROCESSES_OK)
		else:
			logging.error('\u001B[1m Building OAI ' + self.air_interface[self.eNB_instance] + ' Failed\u001B[0m')
			HTML.CreateHtmlTestRow(self.Build_eNB_args, 'KO', CONST.ALL_PROCESSES_OK)
			HTML.CreateHtmlTabFooter(False)
			sys.exit(1)

	def InitializeeNB(self, HTML, EPC):
		if self.runStage == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter')
		
		if self.runStage == 'Post':
			self.AnalyzeeNB(HTML, EPC)
			return

		#if self.runStage == 'Pre':
		logging.debug("InitializeeNB")
		self.testCase_id = HTML.testCase_id

		full_config_file = self.Initialize_eNB_args.replace('-O ','')
		logging.debug('Full config file:' + full_config_file)
		extra_options = ''
		extIdx = full_config_file.find('.conf')
		if (extIdx > 0):
			extra_options = full_config_file[extIdx + 5:]
			logging.debug('Extra options:' + extra_options)
			os.system('sed -i "s/<< gnb_args >>/' + extra_options + '/g" ../colosseum-cm/ansible/oai.yml')
			full_config_file = full_config_file[:extIdx + 5]
			config_path, config_file = os.path.split(full_config_file)
			print("InitialzeeNB: gnb config file: " + full_config_file)
			config_file_list = full_config_file.split('/')
			full_config_file = '\/'.join(config_file_list)
			os.system('sed -i "s/<< config_file >>/' + full_config_file + '/g" ../colosseum-cm/ansible/oai.yml')
		else:
			sys.exit('Insufficient Parameter')

		result = re.search('noS1', str(self.Initialize_eNB_args))
		eNBinNoS1 = False
		if result is not None:
			eNBinNoS1 = True
			logging.debug('\u001B[1m gNB is in noS1 configuration \u001B[0m')
			os.system('sed -i "s/<< noS1 >>/1/g" ../colosseum-cm/ansible/oai.yml')
			os.system('sed -i "s/<< num_nodes >>/2/g" ../colosseum-cm/ansible/oai.yml')
		else:
			os.system('sed -i "s/<< noS1 >>/0/g" ../colosseum-cm/ansible/oai.yml')
			os.system('sed -i "s/<< num_nodes >>/3/g" ../colosseum-cm/ansible/oai.yml')

	def CheckeNBProcess(self, status_queue):
		try:
			# At least the instance 0 SHALL be on!
			if self.eNBstatuses[0] == 0:
				lIpAddr = self.eNBIPAddress
				lUserName = self.eNBUserName
				lPassWord = self.eNBPassword
			elif self.eNBstatuses[0] == 1:
				lIpAddr = self.eNB1IPAddress
				lUserName = self.eNB1UserName
				lPassWord = self.eNB1Password
			elif self.eNBstatuses[0] == 2:
				lIpAddr = self.eNB2IPAddress
				lUserName = self.eNB2UserName
				lPassWord = self.eNB2Password
			else:
				lIpAddr = self.eNBIPAddress
				lUserName = self.eNBUserName
				lPassWord = self.eNBPassword
			mySSH = SSH.SSHConnection()
			mySSH.open(lIpAddr, lUserName, lPassWord)
			mySSH.command('stdbuf -o0 ps -aux | grep --color=never ' + self.air_interface[self.eNB_instance] + ' | grep -v grep', '\$', 5)
			result = re.search(self.air_interface[self.eNB_instance], mySSH.getBefore())
			if result is None:
				logging.debug('\u001B[1;37;41m eNB Process Not Found! \u001B[0m')
				status_queue.put(CONST.ENB_PROCESS_FAILED)
			else:
				status_queue.put(CONST.ENB_PROCESS_OK)
			mySSH.close()
		except:
			os.kill(os.getppid(),signal.SIGUSR1)

	def TerminateeNB(self, HTML, EPC):
		logging.debug('TerminateeNB: Colosseum will stop processes automatically')
	
	def AnalyzeeNB(self, HTML, EPC):
		fileToAnalyze="../logfiles/gnb-logs/gnb.log"
		nodeB_prefix = 'g'
		if path.exists(fileToAnalyze):
			logging.info('\u001B[1m Analyzing ' + nodeB_prefix + 'NB logfile \u001B[0m ' + fileToAnalyze)
			logStatus = self.AnalyzeLogFile_eNB(fileToAnalyze, HTML)
			#logStatus = -1
			if (logStatus < 0):
				logging.info('\u001B[1m' + nodeB_prefix + 'NB Failed \u001B[0m')
				HTML.CreateHtmlTestRow('N/A', 'KO', logStatus)
				self.preamtureExit = True
				self.eNBmbmsEnables[int(self.eNB_instance)] = False
				sys.exit('Failed Scenario')
				#return
			else:
				logging.info('\u001B[1m' + nodeB_prefix + 'NB Completed \u001B[0m')
				HTML.CreateHtmlTestRow(self.runtime_stats, 'OK', CONST.ALL_PROCESSES_OK)
		self.eNBmbmsEnables[int(self.eNB_instance)] = False
		self.eNBstatuses[int(self.eNB_instance)] = -1
        
	def LogCollecteNB(self):
		mySSH = SSH.SSHConnection()
		mySSH.open(self.eNBIPAddress, self.eNBUserName, self.eNBPassword)
		mySSH.command('cd ' + self.eNBSourceCodePath, '\$', 5)
		mySSH.command('cd cmake_targets', '\$', 5)
		mySSH.command('echo ' + self.eNBPassword + ' | sudo -S mv /tmp/enb_*.pcap .','\$',20)
		mySSH.command('echo ' + self.eNBPassword + ' | sudo -S mv /tmp/gnb_*.pcap .','\$',20)
		mySSH.command('echo ' + self.eNBPassword + ' | sudo -S rm -f enb.log.zip', '\$', 5)
		mySSH.command('echo ' + self.eNBPassword + ' | sudo -S zip enb.log.zip enb*.log core* enb_*record.raw enb_*.pcap gnb_*.pcap enb_*txt physim_*.log *stats.log', '\$', 60)
		mySSH.command('echo ' + self.eNBPassword + ' | sudo -S rm enb*.log core* enb_*record.raw enb_*.pcap gnb_*.pcap enb_*txt physim_*.log *stats.log', '\$', 5)
		mySSH.close()

	def AnalyzeLogFile_eNB(self, eNBlogFile, HTML):
		if (not os.path.isfile('./' + eNBlogFile)):
			return -1
		enb_log_file = open('./' + eNBlogFile, 'r')
		exitSignalReceived = False
		foundAssertion = False
		msgAssertion = ''
		msgLine = 0
		foundSegFault = False
		foundRealTimeIssue = False
		rrcSetupComplete = 0
		rrcReleaseRequest = 0
		rrcReconfigRequest = 0
		rrcReconfigComplete = 0
		rrcReestablishRequest = 0
		rrcReestablishComplete = 0
		rrcReestablishReject = 0
		rlcDiscardBuffer = 0
		rachCanceledProcedure = 0
		uciStatMsgCount = 0
		pdcpFailure = 0
		ulschFailure = 0
		ulschAllocateCCEerror = 0
		uplinkSegmentsAborted = 0
		ulschReceiveOK = 0
		gnbRxTxWakeUpFailure = 0
		gnbTxWriteThreadEnabled = False
		cdrxActivationMessageCount = 0
		dropNotEnoughRBs = 0
		mbmsRequestMsg = 0
		htmleNBFailureMsg = ''
		isRRU = False
		isSlave = False
		slaveReceivesFrameResyncCmd = False
		X2HO_state = CONST.X2_HO_REQ_STATE__IDLE
		X2HO_inNbProcedures = 0
		X2HO_outNbProcedures = 0
		global_status = CONST.ALL_PROCESSES_OK
		# Runtime statistics
		runTime = ''
		userTime = ''
		systemTime = ''
		maxPhyMemUsage = ''
		nbContextSwitches = ''
		#NSA FR1 check
		NSA_RAPROC_PUSCH_check = 0
		#dlsch and ulsch statistics (dictionary)
		dlsch_ulsch_stats = {}
		#real time statistics (dictionary)
		real_time_stats = {}
		#count "problem receiving samples" msg
		pb_receiving_samples_cnt = 0
		#count "removing UE" msg
		removing_ue = 0
		#NSA specific log markers
		nsa_markers ={'SgNBReleaseRequestAcknowledge': [],'FAILURE': [], 'scgFailureInformationNR-r15': [], 'SgNBReleaseRequest': []}
	
		#the datalog config file has to be loaded
		datalog_rt_stats_file='datalog_rt_stats.yaml'
		if (os.path.isfile(datalog_rt_stats_file)):
			yaml_file=datalog_rt_stats_file
		elif (os.path.isfile('ci-scripts/'+datalog_rt_stats_file)):
			yaml_file='ci-scripts/'+datalog_rt_stats_file
		else:
			logging.error("Datalog RT stats yaml file cannot be found")
			sys.exit("Datalog RT stats yaml file cannot be found")

		with open(yaml_file,'r') as f:
			datalog_rt_stats = yaml.load(f,Loader=yaml.FullLoader)
		rt_keys = datalog_rt_stats['Ref'] #we use the keys from the Ref field  

		line_cnt=0 #log file line counter
		for line in enb_log_file.readlines():
			line_cnt+=1
			# Runtime statistics
			result = re.search('Run time:' ,str(line))
			if result is not None:
				runTime = str(line).strip()
			if runTime != '':
				result = re.search('Time executing user inst', str(line))
				if result is not None:
					fields=line.split(':')
					userTime = 'userTime : ' + fields[1].replace('\n','')
				result = re.search('Time executing system inst', str(line))
				if result is not None:
					fields=line.split(':')
					systemTime = 'systemTime : ' + fields[1].replace('\n','')
				result = re.search('Max. Phy. memory usage:', str(line))
				if result is not None:
					fields=line.split(':')
					maxPhyMemUsage = 'maxPhyMemUsage : ' + fields[1].replace('\n','')
				result = re.search('Number of context switch.*process origin', str(line))
				if result is not None:
					fields=line.split(':')
					nbContextSwitches = 'nbContextSwitches : ' + fields[1].replace('\n','')
			if X2HO_state == CONST.X2_HO_REQ_STATE__IDLE:
				result = re.search('target eNB Receives X2 HO Req X2AP_HANDOVER_REQ', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__TARGET_RECEIVES_REQ
				result = re.search('source eNB receives the X2 HO ACK X2AP_HANDOVER_REQ_ACK', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__SOURCE_RECEIVES_REQ_ACK
			if X2HO_state == CONST.X2_HO_REQ_STATE__TARGET_RECEIVES_REQ:
				result = re.search('Received LTE_RRCConnectionReconfigurationComplete from UE', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__TARGET_RRC_RECFG_COMPLETE
			if X2HO_state == CONST.X2_HO_REQ_STATE__TARGET_RRC_RECFG_COMPLETE:
				result = re.search('issue rrc_eNB_send_PATH_SWITCH_REQ', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__TARGET_SENDS_SWITCH_REQ
			if X2HO_state == CONST.X2_HO_REQ_STATE__TARGET_SENDS_SWITCH_REQ:
				result = re.search('received path switch ack S1AP_PATH_SWITCH_REQ_ACK', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__IDLE
					X2HO_inNbProcedures += 1
			if X2HO_state == CONST.X2_HO_REQ_STATE__SOURCE_RECEIVES_REQ_ACK:
				result = re.search('source eNB receives the X2 UE CONTEXT RELEASE X2AP_UE_CONTEXT_RELEASE', str(line))
				if result is not None:
					X2HO_state = CONST.X2_HO_REQ_STATE__IDLE
					X2HO_outNbProcedures += 1

			if self.eNBOptions[int(self.eNB_instance)] != '':
				res1 = re.search('max_rxgain (?P<requested_option>[0-9]+)', self.eNBOptions[int(self.eNB_instance)])
				res2 = re.search('max_rxgain (?P<applied_option>[0-9]+)',  str(line))
				if res1 is not None and res2 is not None:
					requested_option = int(res1.group('requested_option'))
					applied_option = int(res2.group('applied_option'))
					if requested_option == applied_option:
						htmleNBFailureMsg += '<span class="glyphicon glyphicon-ok-circle"></span> Command line option(s) correctly applied <span class="glyphicon glyphicon-arrow-right"></span> ' + self.eNBOptions[int(self.eNB_instance)] + '\n\n'
					else:
						htmleNBFailureMsg += '<span class="glyphicon glyphicon-ban-circle"></span> Command line option(s) NOT applied <span class="glyphicon glyphicon-arrow-right"></span> ' + self.eNBOptions[int(self.eNB_instance)] + '\n\n'
			result = re.search('Exiting OAI softmodem', str(line))
			if result is not None:
				exitSignalReceived = True
			result = re.search('[Ss]egmentation [Ff]ault', str(line))
			if result is not None and not exitSignalReceived:
				foundSegFault = True
			result = re.search('[Cc]ore [dD]ump', str(line))
			if result is not None and not exitSignalReceived:
				foundSegFault = True
			result = re.search('./ran_build/build/lte-softmodem', str(line))
			if result is not None and not exitSignalReceived:
				foundSegFault = True
			result = re.search('[Aa]ssertion', str(line))
			if result is not None and not exitSignalReceived:
				foundAssertion = True
			result = re.search('LLL', str(line))
			if result is not None and not exitSignalReceived:
				foundRealTimeIssue = True
			if foundAssertion and (msgLine < 3):
				msgLine += 1
				msgAssertion += str(line)
			result = re.search('Setting function for RU', str(line))
			if result is not None:
				isRRU = True
			if isRRU:
				result = re.search('RU 0 is_slave=yes', str(line))
				if result is not None:
					isSlave = True
				if isSlave:
					result = re.search('Received RRU_frame_resynch command', str(line))
					if result is not None:
						slaveReceivesFrameResyncCmd = True
			result = re.search('LTE_RRCConnectionSetupComplete from UE', str(line))
			if result is not None:
				rrcSetupComplete += 1
			result = re.search('Generate LTE_RRCConnectionRelease|Generate RRCConnectionRelease', str(line))
			if result is not None:				rrcReleaseRequest += 1
			result = re.search('Generate LTE_RRCConnectionReconfiguration', str(line))
			if result is not None:
				rrcReconfigRequest += 1
			result = re.search('LTE_RRCConnectionReconfigurationComplete from UE rnti', str(line))
			if result is not None:
				rrcReconfigComplete += 1
			result = re.search('LTE_RRCConnectionReestablishmentRequest', str(line))
			if result is not None:
				rrcReestablishRequest += 1
			result = re.search('LTE_RRCConnectionReestablishmentComplete', str(line))
			if result is not None:
				rrcReestablishComplete += 1
			result = re.search('LTE_RRCConnectionReestablishmentReject', str(line))
			if result is not None:
				rrcReestablishReject += 1
			result = re.search('CDRX configuration activated after RRC Connection', str(line))
			if result is not None:
				cdrxActivationMessageCount += 1
			result = re.search('uci->stat', str(line))
			if result is not None:
				uciStatMsgCount += 1
			result = re.search('PDCP.*Out of Resources.*reason', str(line))
			if result is not None:
				pdcpFailure += 1
			result = re.search('could not wakeup gNB rxtx process', str(line))
			if result is not None:
				gnbRxTxWakeUpFailure += 1
			result = re.search('tx write thread ready', str(line))
			if result is not None:
				gnbTxWriteThreadEnabled = True
			result = re.search('ULSCH in error in round|ULSCH 0 in error', str(line))
			if result is not None:
				ulschFailure += 1
			result = re.search('ERROR ALLOCATING CCEs', str(line))
			if result is not None:
				ulschAllocateCCEerror += 1
			result = re.search('uplink segment error.*aborted [1-9] segments', str(line))
			if result is not None:
				uplinkSegmentsAborted += 1
			result = re.search('ULSCH received ok', str(line))
			if result is not None:
				ulschReceiveOK += 1
			result = re.search('BAD all_segments_received', str(line))
			if result is not None:
				rlcDiscardBuffer += 1
			result = re.search('Canceled RA procedure for UE rnti', str(line))
			if result is not None:
				rachCanceledProcedure += 1
			result = re.search('dropping, not enough RBs', str(line))
			if result is not None:
				dropNotEnoughRBs += 1
			if self.eNBmbmsEnables[int(self.eNB_instance)]:
				result = re.search('MBMS USER-PLANE.*Requesting.*bytes from RLC', str(line))
				if result is not None:
					mbmsRequestMsg += 1
			#FR1 NSA test : add new markers to make sure gNB is used
			result = re.search('\[gNB [0-9]+\]\[RAPROC\] PUSCH with TC_RNTI [0-9a-fA-F]+ received correctly, adding UE MAC Context UE_id [0-9]+\/RNTI [0-9a-fA-F]+', str(line))
			if result is not None:
				NSA_RAPROC_PUSCH_check = 1
			#dlsch and ulsch statistics
			#keys below are the markers we are loooking for, loop over this keys list
			#everytime these markers are found in the log file, the previous ones are overwritten in the dict
			#eventually we record and print only the last occurence 
			keys = {'UE ID','dlsch_rounds','dlsch_total_bytes','ulsch_rounds','ulsch_total_bytes_scheduled'}
			for k in keys:
				result = re.search(k, line)
				if result is not None:
					#remove 1- all useless char before relevant info (ulsch or dlsch) 2- trailing char
					dlsch_ulsch_stats[k]=re.sub(r'^.*\]\s+', r'' , line.rstrip())
			#real time statistics for gNB
			for k in rt_keys:
				result = re.search(k, line)     
				if result is not None:
					#remove 1- all useless char before relevant info  2- trailing char
					line=line.replace('[0m','')
					tmp=re.match(rf'^.*?(\b{k}\b.*)',line.rstrip()) #from python 3.6 we can use literal string interpolation for the variable k, using rf' in the regex
					if tmp!=None: #with ULULULUULULULLLL at the head of the line, we skip it
						real_time_stats[k]=tmp.group(1)

			#count "problem receiving samples" msg
			result = re.search('\[PHY\]\s+problem receiving samples', str(line))
			if result is not None:
				pb_receiving_samples_cnt += 1
			#count "Removing UE" msg
			result = re.search('\[MAC\]\s+Removing UE', str(line))
			if result is not None:
				removing_ue += 1

			#nsa markers logging
			for k in nsa_markers:
				result = re.search(k, line)
				if result is not None:
					nsa_markers[k].append(line_cnt)					

		enb_log_file.close()
		logging.debug('   File analysis completed')
		if (self.air_interface[self.eNB_instance] == 'lte-softmodem') or (self.air_interface[self.eNB_instance] == 'ocp-enb'):
			nodeB_prefix = 'e'
		else:
			nodeB_prefix = 'g'

		if nodeB_prefix == 'g':
			if ulschReceiveOK > 0:
				statMsg = nodeB_prefix + 'NB showed ' + str(ulschReceiveOK) + ' "ULSCH received ok" message(s)'
				logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
				htmleNBFailureMsg += statMsg + '\n'
			if gnbRxTxWakeUpFailure > 0:
				statMsg = nodeB_prefix + 'NB showed ' + str(gnbRxTxWakeUpFailure) + ' "could not wakeup gNB rxtx process" message(s)'
				logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
				htmleNBFailureMsg += statMsg + '\n'
			if gnbTxWriteThreadEnabled:
				statMsg = nodeB_prefix + 'NB ran with TX Write thread enabled'
				logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
				htmleNBFailureMsg += statMsg + '\n'
			#FR1 NSA test : add new markers to make sure gNB is used
			if NSA_RAPROC_PUSCH_check:
				statMsg = '[RAPROC] PUSCH with TC_RNTI message check for ' + nodeB_prefix + 'NB : PASS '
				htmlMsg = statMsg+'\n'
			else:
				statMsg = '[RAPROC] PUSCH with TC_RNTI message check for ' + nodeB_prefix + 'NB : FAIL or not relevant'
				htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			#problem receiving samples log
			statMsg = '[PHY] problem receiving samples msg count =  '+str(pb_receiving_samples_cnt)
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			#nsa markers
			statMsg = 'logfile line count = ' + str(line_cnt)			
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			if len(nsa_markers['SgNBReleaseRequestAcknowledge'])!=0:
				statMsg = 'SgNBReleaseRequestAcknowledge = ' + str(len(nsa_markers['SgNBReleaseRequestAcknowledge'])) + ' occurences , starting line ' + str(nsa_markers['SgNBReleaseRequestAcknowledge'][0])
			else:
				statMsg = 'SgNBReleaseRequestAcknowledge = ' + str(len(nsa_markers['SgNBReleaseRequestAcknowledge'])) + ' occurences' 
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			statMsg = 'FAILURE = ' + str(len(nsa_markers['FAILURE'])) + ' occurences'
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg

			#ulsch and dlsch statistics
			if len(dlsch_ulsch_stats)!=0: #check if dictionary is not empty
				statMsg=''
				for key in dlsch_ulsch_stats: #for each dictionary key
					statMsg += dlsch_ulsch_stats[key] + '\n' 
					logging.debug(dlsch_ulsch_stats[key])
				htmleNBFailureMsg += statMsg

			#real time statistics
			datalog_rt_stats['Data']={}
			if len(real_time_stats)!=0: #check if dictionary is not empty
				for k in real_time_stats:
					tmp=re.match(r'^(?P<metric>.*):\s+(?P<avg>\d+\.\d+) us;\s+\d+;\s+(?P<max>\d+\.\d+) us;',real_time_stats[k])
					if tmp is not None:
						metric=tmp.group('metric')
						avg=float(tmp.group('avg'))
						max=float(tmp.group('max'))
						datalog_rt_stats['Data'][metric]=["{:.0f}".format(avg),"{:.0f}".format(max),"{:.2f}".format(avg/datalog_rt_stats['Ref'][metric])]
				#once all metrics are collected, store the data as a class attribute to build a dedicated HTML table afterward
				self.datalog_rt_stats=datalog_rt_stats
				#check if there is a fail => will render the test as failed
				for k in datalog_rt_stats['Data']:
					if float(datalog_rt_stats['Data'][k][2])> datalog_rt_stats['Threshold'][k]: #condition for fail : avg/ref is greater than the fixed threshold
						#setting prematureExit is ok although not the best option
						self.prematureExit=True
			else:
				statMsg = 'No real time stats found in the log file\n'
				logging.debug('No real time stats found in the log file')
				htmleNBFailureMsg += statMsg

		else:
			#Removing UE log
			statMsg = '[MAC] Removing UE msg count =  '+str(removing_ue)
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			#nsa markers
			statMsg = 'logfile line count = ' + str(line_cnt)			
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			if len(nsa_markers['SgNBReleaseRequest'])!=0:
				statMsg = 'SgNBReleaseRequest = ' + str(len(nsa_markers['SgNBReleaseRequest'])) + ' occurences , starting line ' + str(nsa_markers['SgNBReleaseRequest'][0])
			else:
				statMsg = 'SgNBReleaseRequest = ' + str(len(nsa_markers['SgNBReleaseRequest'])) + ' occurences'
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg
			statMsg = 'scgFailureInformationNR-r15 = ' + str(len(nsa_markers['scgFailureInformationNR-r15'])) + ' occurences'
			htmlMsg = statMsg+'\n'
			logging.debug(statMsg)
			htmleNBFailureMsg += htmlMsg			


		if uciStatMsgCount > 0:
			statMsg = nodeB_prefix + 'NB showed ' + str(uciStatMsgCount) + ' "uci->stat" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if pdcpFailure > 0:
			statMsg = nodeB_prefix + 'NB showed ' + str(pdcpFailure) + ' "PDCP Out of Resources" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if ulschFailure > 0:
			statMsg = nodeB_prefix + 'NB showed ' + str(ulschFailure) + ' "ULSCH in error in round" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if ulschAllocateCCEerror > 0:
			statMsg = nodeB_prefix + 'NB showed ' + str(ulschAllocateCCEerror) + ' "eNB_dlsch_ulsch_scheduler(); ERROR ALLOCATING CCEs" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if uplinkSegmentsAborted > 0:
			statMsg = nodeB_prefix + 'NB showed ' + str(uplinkSegmentsAborted) + ' "uplink segment error 0/2, aborted * segments" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if dropNotEnoughRBs > 0:
			statMsg = 'eNB showed ' + str(dropNotEnoughRBs) + ' "dropping, not enough RBs" message(s)'
			logging.debug('\u001B[1;30;43m ' + statMsg + ' \u001B[0m')
			htmleNBFailureMsg += statMsg + '\n'
		if rrcSetupComplete > 0:
			rrcMsg = nodeB_prefix + 'NB completed ' + str(rrcSetupComplete) + ' RRC Connection Setup(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
			rrcMsg = ' -- ' + str(rrcSetupComplete) + ' were completed'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if rrcReleaseRequest > 0:
			rrcMsg = nodeB_prefix + 'NB requested ' + str(rrcReleaseRequest) + ' RRC Connection Release(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if rrcReconfigRequest > 0 or rrcReconfigComplete > 0:
			rrcMsg = nodeB_prefix + 'NB requested ' + str(rrcReconfigRequest) + ' RRC Connection Reconfiguration(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
			rrcMsg = ' -- ' + str(rrcReconfigComplete) + ' were completed'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if rrcReestablishRequest > 0 or rrcReestablishComplete > 0 or rrcReestablishReject > 0:
			rrcMsg = nodeB_prefix + 'NB requested ' + str(rrcReestablishRequest) + ' RRC Connection Reestablishment(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
			rrcMsg = ' -- ' + str(rrcReestablishComplete) + ' were completed'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
			rrcMsg = ' -- ' + str(rrcReestablishReject) + ' were rejected'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if self.eNBmbmsEnables[int(self.eNB_instance)]:
			if mbmsRequestMsg > 0:
				rrcMsg = 'eNB requested ' + str(mbmsRequestMsg) + ' times the RLC for MBMS USER-PLANE'
				logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
				htmleNBFailureMsg += rrcMsg + '\n'
		if X2HO_inNbProcedures > 0:
			rrcMsg = 'eNB completed ' + str(X2HO_inNbProcedures) + ' X2 Handover Connection procedure(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if X2HO_outNbProcedures > 0:
			rrcMsg = 'eNB completed ' + str(X2HO_outNbProcedures) + ' X2 Handover Release procedure(s)'
			logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rrcMsg + '\n'
		if self.eNBOptions[int(self.eNB_instance)] != '':
			res1 = re.search('drx_Config_present prSetup', self.eNBOptions[int(self.eNB_instance)])
			if res1 is not None:
				if cdrxActivationMessageCount > 0:
					rrcMsg = 'eNB activated the CDRX Configuration for ' + str(cdrxActivationMessageCount) + ' time(s)'
					logging.debug('\u001B[1;30;43m ' + rrcMsg + ' \u001B[0m')
					htmleNBFailureMsg += rrcMsg + '\n'
				else:
					rrcMsg = 'eNB did NOT ACTIVATE the CDRX Configuration'
					logging.debug('\u001B[1;37;43m ' + rrcMsg + ' \u001B[0m')
					htmleNBFailureMsg += rrcMsg + '\n'
		if rachCanceledProcedure > 0:
			rachMsg = nodeB_prefix + 'NB cancelled ' + str(rachCanceledProcedure) + ' RA procedure(s)'
			logging.debug('\u001B[1;30;43m ' + rachMsg + ' \u001B[0m')
			htmleNBFailureMsg += rachMsg + '\n'
		if isRRU:
			if isSlave:
				if slaveReceivesFrameResyncCmd:
					rruMsg = 'Slave RRU received the RRU_frame_resynch command from RAU'
					logging.debug('\u001B[1;30;43m ' + rruMsg + ' \u001B[0m')
					htmleNBFailureMsg += rruMsg + '\n'
				else:
					rruMsg = 'Slave RRU DID NOT receive the RRU_frame_resynch command from RAU'
					logging.debug('\u001B[1;37;41m ' + rruMsg + ' \u001B[0m')
					htmleNBFailureMsg += rruMsg + '\n'
					self.prematureExit = True
					global_status = CONST.ENB_PROCESS_SLAVE_RRU_NOT_SYNCED
		if foundSegFault:
			logging.debug('\u001B[1;37;41m ' + nodeB_prefix + 'NB ended with a Segmentation Fault! \u001B[0m')
			global_status = CONST.ENB_PROCESS_SEG_FAULT
		if foundAssertion:
			logging.debug('\u001B[1;37;41m ' + nodeB_prefix + 'NB ended with an assertion! \u001B[0m')
			htmleNBFailureMsg += msgAssertion
			global_status = CONST.ENB_PROCESS_ASSERTION
		if foundRealTimeIssue:
			logging.debug('\u001B[1;37;41m ' + nodeB_prefix + 'NB faced real time issues! \u001B[0m')
			htmleNBFailureMsg += nodeB_prefix + 'NB faced real time issues!\n'
		if rlcDiscardBuffer > 0:
			rlcMsg = nodeB_prefix + 'NB RLC discarded ' + str(rlcDiscardBuffer) + ' buffer(s)'
			logging.debug('\u001B[1;37;41m ' + rlcMsg + ' \u001B[0m')
			htmleNBFailureMsg += rlcMsg + '\n'
			global_status = CONST.ENB_PROCESS_REALTIME_ISSUE
		HTML.htmleNBFailureMsg=htmleNBFailureMsg
		# Runtime statistics for console output and HTML
		if runTime != '':
			logging.debug(runTime)
			logging.debug(userTime)
			logging.debug(systemTime)
			logging.debug(maxPhyMemUsage)
			logging.debug(nbContextSwitches)
			self.runtime_stats='<pre>'+runTime + '\n'+ userTime + '\n' + systemTime + '\n' + maxPhyMemUsage + '\n' + nbContextSwitches+'</pre>'
		return global_status
