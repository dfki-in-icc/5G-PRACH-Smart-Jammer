# OAI 5GCN Docker compose With Mounted Configuration File

The docker-compose way of configuring network functions is restrictive, to give more freedom to CN5G users, we propose to use this docker-compose file in which configuration files are mounted. The structure of this folder


```shell
.
├── conf
│   ├── oai-amf.conf
│   ├── oai-ausf.conf
│   ├── oai-nrf.conf
│   ├── oai-smf.conf
│   ├── oai-spgwu-tiny.conf
│   ├── oai-udm.conf
│   ├── users.conf
│   ├── sip.conf
│   └── oai-udr.conf
├── database
│   └── oai_db.sql
├── docker-compose.yaml
├── healthscripts
│   └── mysql-healthcheck.sh
└── README.md <--- YOU ARE HERE

3 directories, 11 files
``` 

Configuration for all network functions is present in `./conf` folder please edit the configuration there. 

In docker-compose file we just define the network functions which will be deployed using the docker-compose. OAI official docker-hub repository is https://hub.docker.com/u/oaisoftwarealliance. In the repository there are three type of image tags,

1. Latest (latest release or master branch of the network function)
2. develop (Build using develop branch of the network function) 
3. version tags (ex. v1.5.0)


## Create IMS image

```
docker build --no-cache --target ims --tag asterisk-ims:latest --file Dockerfile .
```

## To deploy OAI 5G CN

``` shell
docker-compose up -d
``` 

wait till the time all contianers are healthy, also check if SPGWU and SMF are sharing PFCP heartbeat. Very important for creating a PDU session. 

```shell
# This command means SPGWU has received heartbeat request from smf
docker logs oai-spgwu-tiny | grep 'Received SX HEARTBEAT REQUEST' | wc -l
```

```shell
# This command means smf is starting the heartbeat procedure
docker logs oai-smf | grep 'PFCP HEARTBEAT PROCEDURE' | wc -l
```
If you see the above lines in the log of `oai-spgwu-tiny` and `oai-smf` container it means core network PFCP session is created between SMF and SPGWU and its properly deployed. Now you can start using OAI 5G core Network. 


To properly configure the ims at the moment we need to add a rule in spgwu manaully 

```shell
docker exec -it oai-spgwu-tiny  iptables -t nat -A POSTROUTING -o eth0 -s 12.2.1.0/24 -j SNAT --to 192.168.70.134
docker exec -it oai-spgwu-tiny  ip route add 12.2.1.0/24 via 12.1.1.1 dev tun0
```


# To un-deploy

```shell
docker-compose down -t2
```
