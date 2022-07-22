# OAI FHI (with ORAN-7.2) Documentation #

 *  [O-RAN FHI 7.2](./targets/ARCH/ORAN_FHI/doc/ORANLIB.md)
 *  [O-RAN FHI Integration with OAI](./targets/ARCH/ORAN_FHI/doc/OAIFHI.md)
 *  [OAI FHI Test Status with multiple commercial RUs](./targets/ARCH/ORAN_FHI/doc/OAIFHITEST.md)
 
# OAI FHI Build Instructions (intended for 5G-NR) #

 * To download O-RAN FHI PHY library
 
   git clone https://gerrit.o-ran-sc.org/r/o-du/phy.git
 
# Prerequisites to run Platforms 
 
 * DPDK (Data Plane Development Kit)
   
   Install DPDK version 19.11 (https://launchpad.net/ubuntu/+source/dpdk/19.11.12-0ubuntu0.20.04.1)
   
   tar xJf dpdk-<version>.tar.xz
   cd dpdk-<version>

 * Build DPDK instructions via meson
   
   cd dpdk-<version>
   meson build
   cd build
   sudo ninja
   sudo ninja install
   
 The above will compile DPDK in the build subdir, and install corresponding libs, drivers and headers into the system 
 /usr/local
 
 After the installation, to use DPDK, run "CFLAG" and "LDFLAG" commands
 
   pkg-config --cflags libdpdk
   pkg-config --libs libdpdk
   
   export RTE_SDK=$PWD
   export RTE_TARGET=x86_64-native-linuxapp-gcc
   make config T=$RTE_TARGET O=$RTE_TARGET
   cd $RTE_TARGET
   make -j 10
   
# ORAN Fronthaul Library #
  
  To get the ORAN FHI library and installation, follow the instructions

  git clone https://gerrit.o-ran-sc.org/r/o-du/phy.git
  go to FHI_LIB
   
# OAI-FHI BUILD and Compilation #
 
   $ git clone https://gitlab.eurecom.fr/oai/openairinterface5g.git
   $ cd openairinterface5g
   $ git checkout ORAN_FHI_integration
   $ git branch (to check the branch status)
   $ source oaienv
   $ cd cmake_targets
   $ ./build_oai --gNB -C -c (Add, -I as well if it is first time to use for external dependencies)

# Compile the oran target library and link it to the oai transpro #

 * Compile the ORAN FHI target

   $ cd <oai_folder>/openairinterface5g/cmake_targets/ran_build/build
   $ make oran_fhlib_5g
   
 * link it to the project

   $ ln -sf liboran_fhlib_5g.so liboai_transpro.so
   
 * Make a linkage to the libxran.so from fhi_lib to <oai_folder>/openairinterface5g/cmake_targets/ran_build/build

