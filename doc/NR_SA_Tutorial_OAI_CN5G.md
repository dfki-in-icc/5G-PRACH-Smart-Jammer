<table style="border-collapse: collapse; border: none;">
  <tr style="border-collapse: collapse; border: none;">
    <td style="border-collapse: collapse; border: none;">
      <a href="http://www.openairinterface.org/">
         <img src="./images/oai_final_logo.png" alt="" border=3 height=50 width=150>
         </img>
      </a>
    </td>
    <td style="border-collapse: collapse; border: none; vertical-align: center;">
      <b><font size = "5">OAI 5G NR SA tutorial: setting up OAI CN5G</font></b>
    </td>
  </tr>
</table>

**Table of Contents**

[[_TOC_]]

#  1. Scenario
In this tutorial we describe how to configure and run a 5G end-to-end setup with OAI CN5G, OAI gNB and COTS UE.

Minimum hardware requirements:
- Laptop/Desktop/Server for OAI CN5G and OAI gNB
    - Operating System: [Ubuntu 22.04.1 LTS](https://releases.ubuntu.com/22.04.1/ubuntu-22.04.1-desktop-amd64.iso)
    - CPU: 8 cores x86_64 @ 3.5 GHz
    - RAM: 32 GB
- Laptop for UE
    - Operating System: Microsoft Windows 10 x64
    - CPU: 4 cores x86_64
    - RAM: 8 GB
    - Windows driver for Quectel MUST be equal or higher than version **2.2.4**
- [USRP B210](https://www.ettus.com/all-products/ub210-kit/), [USRP N300](https://www.ettus.com/all-products/USRP-N300/) or [USRP X300](https://www.ettus.com/all-products/x300-kit/)
    - Please identify the network interface(s) on which the USRP is connected and update the gNB configuration file
- Quectel RM500Q
    - Module, M.2 to USB adapter, antennas and SIM card
    - Firmware version of Quectel MUST be equal or higher than **RM500QGLABR11A06M4G**


# 2. OAI CN5G

## 2.1 OAI CN5G pre-requisites

```bash
sudo apt install -y git net-tools putty

sudo apt install -y apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu  $(lsb_release -cs)  stable"
sudo apt update
sudo apt install -y docker docker-ce

# Add your username to the docker group, otherwise you will have to run in sudo mode.
sudo usermod -a -G docker $(whoami)
reboot

# https://docs.docker.com/compose/install/
sudo curl -L "https://github.com/docker/compose/releases/download/v2.12.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
```

## 2.2 OAI CN5G Setup

```bash
# Git oai-cn5g-fed repository
git clone https://gitlab.eurecom.fr/oai/cn5g/oai-cn5g-fed.git ~/oai-cn5g-fed

# Pull docker images
docker pull oaisoftwarealliance/oai-amf:develop
docker pull oaisoftwarealliance/oai-nrf:develop
docker pull oaisoftwarealliance/oai-smf:develop
docker pull oaisoftwarealliance/oai-udr:develop
docker pull oaisoftwarealliance/oai-udm:develop
docker pull oaisoftwarealliance/oai-ausf:develop
docker pull oaisoftwarealliance/oai-spgwu-tiny:develop
docker pull oaisoftwarealliance/trf-gen-cn5g:latest
```

## 2.3 OAI CN5G Configuration files
Download and copy configuration files:
- To be done
```bash
wget -O ~/oai-cn5g.zip https://gitlab.eurecom.fr/oai/openairinterface5g/-/archive/NR_SA_Tutorials_vX/openairinterface5g-NR_SA_Tutorials_vX.zip?path=doc/tutorial_resources/oai-cn5g
unzip ~/oai-cn5g.zip
mv ~/openairinterface5g-NR_SA_Tutorials_vX-doc-tutorial_resources-oai-cn5g/doc/tutorial_resources/oai-cn5g ~/oai-cn5g
rm -r ~/openairinterface5g-NR_SA_Tutorials_vX-doc-tutorial_resources-oai-cn5g ~/oai-cn5g.zip
```
- Make mysql healthcheck script executable
```bash
chmod +x ~/oai-cn5g/healthscripts/mysql-healthcheck.sh
```

## 2.4 IMS
```bash
docker build --no-cache --target ims --tag asterisk-ims:latest --file ~/oai-cn5g/Dockerfile .
```

# 3. Run OAI CN5G
## 3.1 Start OAI CN5G
```bash
cd ~/oai-cn5g
docker-compose up -d
```

## 3.2 Stop OAI CN5G
```bash
cd ~/oai-cn5g
docker-compose down
```

# 4. Testing with OAI gNB
## 4.1 Testing with COTS UE
Please check this link:
[Testing with OAI gNB and COTS UE](NR_SA_Tutorial_COTS_UE.md)

## 4.1 Testing with OAI nrUE
Please check this link:
[Testing with OAI gNB and OAI nrUE](NR_SA_Tutorial_OAI_nrUE.md)