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
import logging
import sshconnection as SSH
import cls_oai_html
#import os
import re
#import time
#import subprocess
import sys
import constants as CONST
import helpreadme as HELP

class Cluster:
	def __init__(self):
		self.eNBIpAddr = ""
		self.eNBUserName = ""
		self.eNBPassword = ""
		self.eNBSourceCodePath = ""
		self.OCUserName = ""
		self.OCPassword = ""
		self.OCProjectName = ""
		self.ranRepository = ""
		self.ranBranch = ""
		self.ranCommitID= ""
		self.ranAllowMerge= False
		self.ranTargetBranch= ""
		#self.testResult = {}
		#self.testCount = [0,0,0]
		#self.testSummary = {}
		#self.testStatus = False

	def _createWorkspace(self, sshSession, password, sourcePath):
		# on RedHat/CentOS .git extension is mandatory
		result = re.search('([a-zA-Z0-9\:\-\.\/])+\.git', self.ranRepository)
		if result is not None:
			full_ran_repo_name = self.ranRepository.replace('git/', 'git')
		else:
			full_ran_repo_name = self.ranRepository + '.git'
		sshSession.command('mkdir -p ' + sourcePath, '\$', 5)
		sshSession.command('cd ' + sourcePath, '\$', 5)
		sshSession.command('if [ ! -e .git ]; then stdbuf -o0 git clone ' + full_ran_repo_name + ' .; else stdbuf -o0 git fetch --prune; fi', '\$', 600)
		# Raphael: here add a check if git clone or git fetch went smoothly
		sshSession.command('git config user.email "jenkins@openairinterface.org"', '\$', 5)
		sshSession.command('git config user.name "OAI Jenkins"', '\$', 5)

		sshSession.command('echo ' + password + ' | sudo -S git clean -x -d -ff', '\$', 30)
		sshSession.command('mkdir -p cmake_targets/log', '\$', 5)
		# if the commit ID is provided use it to point to it
		if self.ranCommitID != '':
			sshSession.command('git checkout -f ' + self.ranCommitID, '\$', 30)
		# if the branch is not develop, then it is a merge request and we need to do
		# the potential merge. Note that merge conflicts should already been checked earlier
		if (self.ranAllowMerge):
			if self.ranTargetBranch == '':
				if (self.ranBranch != 'develop') and (self.ranBranch != 'origin/develop'):
					sshSession.command('git merge --ff origin/develop -m "Temporary merge for CI"', '\$', 5)
			else:
				logging.debug('Merging with the target branch: ' + self.ranTargetBranch)
				sshSession.command('git merge --ff origin/' + self.ranTargetBranch + ' -m "Temporary merge for CI"', '\$', 5)

	def ClusterBuildgNB(self, HTML):
		if self.ranRepository == '' or self.ranBranch == '' or self.ranCommitID == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter')

		lIpAddr = self.eNBIPAddress
		lUserName = self.eNBUserName
		lPassWord = self.eNBPassword
		lSourcePath = self.eNBSourceCodePath
		if lIpAddr == '' or lUserName == '' or lPassWord == '' or lSourcePath == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter: no SSH Credentials')
		ocUserName = self.OCUserName
		ocPassword = self.OCPassword
		ocProjectName = self.OCProjectName
		if ocUserName == '' or ocPassword == '' or ocProjectName == '':
			HELP.GenericHelp(CONST.Version)
			sys.exit('Insufficient Parameter: no OC Credentials')

		logging.debug('Building on cluster triggered from server: ' + lIpAddr)
		mySSH = SSH.SSHConnection()
		mySSH.open(lIpAddr, lUserName, lPassWord)

		self.testCase_id = HTML.testCase_id

		self._createWorkspace(mySSH, lPassWord, lSourcePath)
		#check out correct branch?

		# need to check entitlements?

		# logging to OC Cluster and then switch to corresponding project
		# TODO don't hardcode path
		mySSH.command(f'oc login -u {ocUserName} -p {ocPassword} --server https://api.snoai.cs.eurecom.fr:6443 ', '\$', 30)
		if mySSH.getBefore().count('Login successful.') == 0:
			logging.error('\u001B[1m OC Cluster Login Failed\u001B[0m')
			mySSH.close()
			HTML.CreateHtmlTestRow('N/A', 'KO', CONST.OC_LOGIN_FAIL)
			RAN.prematureExit = True # todo necessary?
			return
		else:
			logging.debug('\u001B[1mLogin to OC Cluster Successfully\u001B[0m')

		mySSH.command(f'oc project {ocProjectName}', '\$', 30)
		if mySSH.getBefore().count(f'Already on project "{ocProjectName}"') == 0 and mySSH.getBefore().count(f'Now using project "{self.OCProjectName}"') == 0:
			logging.error(f'\u001B[1mUnable to access OC project {ocProjectName}\u001B[0m')
			mySSH.command('oc logout', '\$', 30)
			mySSH.close()
			HTML.CreateHtmlTestRow('N/A', 'KO', CONST.OC_PROJECT_FAIL)
			RAN.prematureExit = True
			return
		else:
			logging.debug(f'\u001B[1mNow using project {ocProjectName}\u001B[0m')

		mySSH.command('oc logout', '\$', 6)
		mySSH.close()

		HTML.CreateHtmlTestRow('N/A', 'OK', CONST.ALL_PROCESSES_OK)
		logging.info('\u001B[1mOS SN Cluster Pass\u001B[0m')
