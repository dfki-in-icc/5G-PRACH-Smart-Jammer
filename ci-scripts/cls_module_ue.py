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
#
#   Required Python Version
#     Python 3.x
#
#---------------------------------------------------------------------

#to use isfile
import os
import sys
import logging
#to create a SSH object locally in the methods
import sshconnection
#time.sleep
import time


import re
import subprocess
from datetime import datetime

#for log rotation mgt
import cls_log_mgt

class Module_UE:

	def __init__(self,Module):
		#create attributes as in the Module dictionary
		for k, v in Module.items():
			setattr(self, k, v)
		self.UEIPAddress = ""
		self.cmd_dict= {"attach" : self.AttachScript,"detach" : self.DetachScript, "initializeUE" : self.InitializeUE, "terminateUE" : self.TerminateUE}#dictionary of function scripts
		self.ue_trace=''		



#-----------------$
#PUBLIC Methods$
#-----------------$

	#this method checks if the specified Process is running on the server hosting the module
	#if not it will be started
	def InitializeUEModule(self):
		HOST=f"{self.HostUsername}@{self.HostIPAddress}"
		COMMAND="ps aux | grep --colour=never inside Trigger UE " + self.Process['Name'] + " | grep -v grep "
		logging.debug(COMMAND)
		ssh = subprocess.Popen(["ssh", "%s" % HOST, COMMAND],shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		result = ssh.stdout.readlines()
		if len(result)!=0:
			logging.debug(self.Process['Name'] + " process found")
			return True 
		else:#start process and check again  
			logging.debug(self.Process['Name'] + " process NOT found")
			#starting the process
			logging.debug('Starting ' + self.Process['Name'])
			mySSH = sshconnection.SSHConnection()
			mySSH.open(self.HostIPAddress, self.HostUsername, self.HostPassword)
			mySSH.command(' sudo -S rm -f /tmp/module-ue.log','\$',5)
			output = self.Command("initializeUE")
			mySSH.close()
			return True
			

	#Generic command function, using function pointers dictionary
	def Command(self,cmd):
		mySSH = sshconnection.SSHConnection()
		mySSH.open(self.HostIPAddress, self.HostUsername, self.HostPassword)
		mySSH.command( self.cmd_dict[cmd] , '\$', 300)
		# hack: trim the first and last lines, which contain the
		# command prompt (first) and 
		result = '\r\n'.join(mySSH.getBefore().split('\r\n')[1:-1])
		mySSH.close()
		return result

	#this method retrieves the Module IP address (not the Host IP address) 
	def GetModuleIPAddress(self):
		HOST=self.HostUsername+'@'+self.HostIPAddress
		COMMAND = self.GetIP
		ssh = subprocess.Popen(["ssh", "%s" % HOST, COMMAND],shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		output = ssh.stdout.readlines()
		for i in range(0, len(output)):
			result = re.search('inet (?P<ip>[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)', output[i].decode("utf-8"))
			if result is not None:
				break
		if result is not None and result.group('ip') is not None:
			self.UEIPAddress = result.group('ip')
			logging.debug('\u001B[1mUE Module IP Address is ' + self.UEIPAddress + '\u001B[0m')
			return 1
		else:
			logging.debug('\u001B[1;37;41m Module IP Address Not Found! \u001B[0m')
			logging.debug(result)
			logging.debug(result.group('ip'))
			return 0

	def CheckModuleMTU(self):
		HOST=self.HostUsername+'@'+self.HostIPAddress
		COMMAND = self.MTUSize
		ssh = subprocess.Popen(["ssh", "%s" % HOST, COMMAND],shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
		output = ssh.stdout.readlines()
		result = re.search('mtu (?P<mtu>[0-9]+)', output[0].decode("utf-8"))
		if result is not None and result.group('mtu') is not None:
			logging.debug('\u001B[1mUE Module NIC MTU is ' + str(self.MTU) + ' as expected\u001B[0m')
			return 1
		else:
			logging.debug('\u001B[1;37;41m Incorrect Module NIC MTU or MTU not found! Expected: ' + str(self.MTU) + '\u001B[0m')
			return 0

	def EnableTrace(self):
		if self.ue_trace=="yes":
			mySSH = sshconnection.SSHConnection()
			mySSH.open(self.HostIPAddress, self.HostUsername, self.HostPassword)
			#delete old artifacts
			mySSH.command('echo ' + ' '  + ' | sudo -S rm -rf ci_qlog','\$',5)
			#start Trace, artifact is created in home dir
			mySSH.command('echo $USER; nohup sudo -E QLog/QLog -s ci_qlog -f NR5G.cfg > /dev/null 2>&1 &','\$', 5)
			mySSH.close()

	def DisableTrace(self):
		mySSH = sshconnection.SSHConnection()
		mySSH.open(self.HostIPAddress, self.HostUsername, self.HostPassword)
		mySSH.command('echo ' + ' '  + ' | sudo -S killall --signal=SIGINT *QLog*', '\$',5)
		mySSH.close()


	def TerminateUEModule(self):
		self.Command("terminateUE")


	def LogCollect(self):
		if self.ue_trace=="yes":
			mySSH = sshconnection.SSHConnection()
			mySSH.open(self.HostIPAddress, self.HostUsername, self.HostPassword)
			#archive qlog to USB stick in /media/usb-drive/ci_qlogs with datetime suffix
			now=datetime.now()
			now_string = now.strftime("%Y%m%d-%H%M")
			source='ci_qlog'
			destination= self.LogStore + '/ci_qlog_'+now_string+'.zip'
			#qlog artifact is zipped into the target folder
			mySSH.command('echo $USER; echo ' + ' '  + ' | nohup sudo -S zip -r '+destination+' '+source+' > /dev/null 2>&1 &','\$', 10)
			mySSH.close()
			#post action : log cleaning to make sure enough space is reserved for the next run
			Log_Mgt=cls_log_mgt.Log_Mgt(self.HostUsername,self.HostIPAddress, self.HostPassword, self.LogStore)
			Log_Mgt.LogRotation()
		else:
			destination=""
		return destination

