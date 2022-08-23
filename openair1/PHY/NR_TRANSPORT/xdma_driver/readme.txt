version number : c1ae09621e1aecec8b957936a11ece02b0caf45b

------------------------------------------------------------------------
*** xdma fpga driver ***

#path
openairinterface5g/openair1/PHY/NR_TRANSPORT/xdma_driver

------------------------------------------------------------------------
*** oai code modify ***
#path
openairinterface5g/cmake_targets

CMakeLists.txt

#path
openairinterface5g/openair1/PHY/NR_TRANSPORT/

nr_ldpc_decoding_pym.h

nr_ulsch_decoding.c

------------------------------------------------------------------------
######################### first time installation ######################
*** driver operate procedures ***
# Server environment
    lspci | grep Xilinx
        "3b:00.0 Serial controller: Xilinx Corporation Device 8038"

# Build driver
  ## Install kernel module driver
    cd openairinterface5g/openair1/PHY/NR_TRANSPORT/xdma_driver/xdma

    sudo make clean

    sudo make install
  
  
  ## Copy execute and run
    cd openairinterface5g/openair1/PHY/NR_TRANSPORT/xdma_driver/tests

    sudo ./load_driver.sh

# Added ldpc decoder lib
cd openairinterface5g/openair1/PHY/NR_TRANSPORT/xdma_driver/

sudo mv libxdma_ldpc_dec.so /usr/lib

################################ Reboot #############################
*** driver operate procedures ***
# Execute and run
cd openairinterface5g/openair1/PHY/NR_TRANSPORT/xdma_driver/tests

sudo ./load_driver.sh
------------------------------------------------------------------------
*** Run OAI ***
cd openairinterface5g

source oaienv

cd cmake_targets

#first time
sudo ./build_oai -I

#for simulation
sudo ./build_oai -c -C -P

sudo ./phy_simulators/build/nr_ulsim
-s : SNR
-o : Enabled FPGA module (default is disabled)

#Notice: We create a folder contain FPGA LDPC decoder file "refer_datasheet.log" in /PHY/NR_TRANSPORTS/xdma_driver/log

