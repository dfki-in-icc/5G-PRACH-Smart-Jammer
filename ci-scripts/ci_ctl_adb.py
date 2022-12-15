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


import logging
import sys
import time
import re
import subprocess as sp
from time import sleep

class adb_ctl:
	#---------------
	#private methods
	#---------------
	def __init__(self,cmd):
		self.cmd_dict= {"attach": self.attach,"detach":self.detach, "initializeUE":self.initializeUE, "terminateUE":self.terminateUE}#dictionary of function pointers

	#--------------
	#public methods
	#--------------
	def attach(self):
		logging.debug(self.UE)
		flightMode_OFF = 'adb -s  ' + self.UE + ' shell /data/local/tmp/on'
		logging.debug(flightMode_OFF)
		sp.getoutput('adb -s ' +  self.UE + ' shell /data/local/tmp/on')
		dataConnectionState=sp.getoutput('adb -s ' + self.UE + ' shell "dumpsys telephony.registry"')
		result = re.search('mDataConnectionState.*=(?P<state>[0-9\-]+)', dataConnectionState)
		if result is None:
			logging.debug('\u001B[1;37;41m mDataConnectionState Not Found! \u001B[0m')
		else:
			mDataConnectionState = int(result.group('state'))
			if mDataConnectionState == 2:
				logging.debug('\u001B[1mUE (' + self.UE + ') Attach Completed\u001B[0m')
				return

	def detach(self):
		logging.debug(self.UE)
		disable_data = 'adb -s  '+ self.UE + ' shell "svc data disable"'
		logging.debug(disable_data)
		sp.getoutput('adb -s ' + self.UE + ' shell "svc data disable"')
		self.checkUEStatus()

	def initializeUE(self):
		logging.debug(self.UE)
		enable_data = 'adb -s  '+ self.UE + ' shell "svc data enable"'
		flightMode_ON = 'adb -s  ' + self.UE + ' shell /data/local/tmp/off'
		logging.debug(enable_data)
		logging.debug(flightMode_ON)
		sp.getoutput('adb -s  ' + self.UE + ' shell "svc data enable"')
		sp.getoutput('adb -s  ' + self.UE + ' shell /data/local/tmp/off')
		self.checkUEStatus()

	def terminateUE(self):
		logging.debug(self.UE)
		flightMode_ON = 'adb -s  ' + self.UE + ' shell /data/local/tmp/off'
		logging.debug(flightMode_ON)
		sp.getoutput('adb -s  ' + self.UE +  ' shell /data/local/tmp/off')
		self.checkUEStatus()

	def checkUEStatus(self):
		logging.debug(self.UE)
		connectionState = sp.getoutput('adb -s ' + self.UE + ' shell "dumpsys telephony.registry"')
		result = re.search('mServiceState=(?P<serviceState>[0-9]+)', connectionState)
		serviceState = 'Service State: UNKNOWN'
		if result is not None:
			lServiceState = int(result.group('serviceState'))
			if lServiceState == 3:
				serviceState = 'Service State: RADIO_POWERED_OFF'
			if lServiceState == 1:
				serviceState = 'Service State: OUT_OF_SERVICE'
			if lServiceState == 0:
				serviceState = 'Service State: IN_SERVICE'
			if lServiceState == 2:
				serviceState = 'Service State: EMERGENCY_ONLY'
		result = re.search('mDataConnectionState=(?P<dataConnectionState>[0-9]+)',connectionState)
		dataConnectionState = 'Data State:    UNKNOWN'
		if result is not None:
			lDataConnectionState = int(result.group('dataConnectionState'))
			if lDataConnectionState == 0:
				dataConnectionState = 'Data State:    DISCONNECTED'
			if lDataConnectionState == 1:
				dataConnectionState = 'Data State:    CONNECTING'
			if lDataConnectionState == 2:
				dataConnectionState = 'Data State:    CONNECTED'
			if lDataConnectionState == 3:
				dataConnectionState = 'Data State:    SUSPENDED'
		logging.debug('\u001B[1;37;44m Status Check (' + str(self.UE) + ') \u001B[0m')
		logging.debug('\u001B[1;34m    ' + serviceState + '\u001B[0m')
		logging.debug('\u001B[1;34m    ' + dataConnectionState + '\u001B[0m')


if __name__ == "__main__":
	if len(sys.argv) == 3:
		command = sys.argv[1]
		Module=adb_ctl(sys.argv[0])
		Module.UE = sys.argv[2]
		print("inside main")
		print(command)
		Module.cmd_dict[command]()
		print(Module.cmd_dict)
	else:
		print("Too many arguments")
