"""
Licensed to the OpenAirInterface (OAI) Software Alliance.

This code is licensed to OSA under one or more contributor license agreements.
See the NOTICE file distributed with this work for additional information
regarding copyright ownership.
The OpenAirInterface Software Alliance licenses this file to You under
the OAI Public License, Version 1.1  (the "License"); you may not use this file
except in compliance with the License.
You may obtain a copy of the License at

    http://www.openairinterface.org/?page_id=698

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
------------------------------------------------------------------------------
For more information about the OpenAirInterface (OAI) Software Alliance:

    contact@openairinterface.org

"""


class NodeB:
    """Object class to support any RAN (eNB/gNB) operations."""

    def __init__(self, infra, config, deployment, git_info):
        self.Deployment = deployment
        self.Infra = infra
        self.GitInfo = git_info
        for key, val in config.items():
            setattr(self, key, val)


    def PrintDeploy(self):
        print(self.Deploy)
        
    def PrintGitInfo(self):
        print(self.GitInfo)