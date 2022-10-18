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
import sys
import time
import subprocess as sp
import re

class asue_ctl:
    def __init__(self,cmd):
        self.UE = ""
        self.cmd_dict={"getIP" : self.getIP, "mtu" : self.mtu }

    def getIP(self):
        ueIP = f' ip netns exec {self.UE} ip a show dev pdn0 | grep --colour=never inet | grep pdn0'
        print(ueIP)
        return ueIP

    def mtu(self):
        mtuSize = f' ip netns exec {self.UE} ip a show dev pdn0 | grep --colour=never mtu'
        print(mtuSize)
        return mtuSize

if __name__ == "__main__":
    if len(sys.argv) == 3:
        command = sys.argv[1]
        Module=asue_ctl(sys.argv[0])
        Module.UE=sys.argv[2]
        print(command)
        Module.cmd_dict[command]()
        print(Module.cmd_dict[command])
    else:
        print("Too many arguments")
