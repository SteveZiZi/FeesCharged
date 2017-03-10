#!/bin/sh
#function change ip
#see ipconfig.ini
#call ipcfgdo.sh

if [ -r /mnt/dyjc/set/user/ipconfig.ini ]; then
	IPCFGFILE=/mnt/dyjc/set/user/ipconfig.ini
elif [ -r /mnt/dyjc/set/default/ipconfig.ini ]; then
	IPCFGFILE=/mnt/dyjc/set/default/ipconfig.ini
fi

if [ "$IPCFGFILE" == "" ]; then
	echo "Warning:ipconfig.ini is not find!!!"
	exit 1
else
	IP_eth0=$(grep '^IP_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	NETMASK_eth0=$(grep '^NETMASK_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	GW_eth0=$(grep '^GW_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	MAC_eth0=$(grep '^MAC_eth0' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	IP_eth1=$(grep '^IP_eth1' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	NETMASK_eth1=$(grep '^NETMASK_eth1' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	GW_eth1=$(grep '^GW_eth1' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
	MAC_eth1=$(grep '^MAC_eth1' $IPCFGFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
fi

if [ "$IP_eth0" == "" ]; then
	IP_eth0=192.168.0.100
fi
if [ "$NETMASK_eth0" == "" ]; then
	NETMASK_eth0=255.255.255.0
fi
if [ "$GW_eth0" == "" ]; then
	GW_eth0=192.168.0.1
fi
if [ "$MAC_eth0" == "" ]; then
	MAC_eth0=00:01:02:B4:56:58
fi
if [ "$IP_eth1" == "" ]; then
	IP_eth1=192.168.1.100
fi
if [ "$NETMASK_eth1" == "" ]; then
	NETMASK_eth1=255.255.255.0
fi
if [ "$GW_eth1" == "" ]; then
	GW_eth1=192.168.1.1
fi
if [ "$MAC_eth1" == "" ]; then
	MAC_eth1=00:01:02:B4:56:59
fi

/mnt/dyjc/script/ipcfgdo.sh "eth0" "$MAC_eth0" "$IP_eth0" "$NETMASK_eth0" "$GW_eth0"
/mnt/dyjc/script/ipcfgdo.sh "eth1" "$MAC_eth1" "$IP_eth1" "$NETMASK_eth1" "$GW_eth1"

