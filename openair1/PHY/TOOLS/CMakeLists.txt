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
#! \file openair1/PHY/TOOLS/CMakelists.txt
#* \brief: build rules and checks for softscope shared libraries
#* \author Francois TABURET
#* \date 2023
#* \version 1.0
#* \company NOKIA BellLabs France
#* \email: francois.taburet@nokia-bell-labs.com
#* \note
#* \warning
#*/

include_directories ("/usr/include/X11")
set(XFORMS_SOURCE
  ${OPENAIR1_DIR}/PHY/TOOLS/lte_phy_scope.c
  )
set(XFORMS_ENB_SOURCE
  ${OPENAIR1_DIR}/PHY/TOOLS/lte_enb_scope.c
  )
set(XFORMS_UE_SOURCE
  ${OPENAIR1_DIR}/PHY/TOOLS/lte_ue_scope.c
  )
set(XFORMS_SOURCE_NR
  ${OPENAIR1_DIR}/PHY/TOOLS/nr_phy_scope.c
  )
set(XFORMS_SOURCE_SOFTMODEM
  ${OPENAIR_DIR}/executables/stats.c
  )
set(XFORMSINTERFACE_SOURCE
  ${OPENAIR1_DIR}/PHY/TOOLS/phy_scope_interface.c
  )
set(XFORMS_LIBRARIES "forms")

add_library(enbscope MODULE ${XFORMS_SOURCE} ${XFORMS_SOURCE_SOFTMODEM} ${XFORMS_ENB_SOURCE})
add_library(uescope MODULE ${XFORMS_SOURCE} ${XFORMS_SOURCE_SOFTMODEM} ${XFORMS_UE_SOURCE})
target_link_libraries(enbscope PRIVATE ${XFORMS_LIBRARIES})
target_link_libraries(enbscope PRIVATE asn1_nr_rrc asn1_lte_rrc)
target_link_libraries(uescope PRIVATE ${XFORMS_LIBRARIES})
target_link_libraries(uescope PRIVATE asn1_nr_rrc asn1_lte_rrc)

add_library(nrscope MODULE ${XFORMS_SOURCE_NR})
target_link_libraries(nrscope PRIVATE ${XFORMS_LIBRARIES})
target_link_libraries(nrscope PRIVATE asn1_nr_rrc asn1_lte_rrc)
# all libraries should be written to root build dir (default creates the same hierarchie under build as existing for sources)
set_target_properties(enbscope uescope nrscope 
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../../..
)
