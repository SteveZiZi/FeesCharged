#!/bin/sh
#function change ip
#usage ./ifcfgdo.sh ETH MAC IP NETMASK GW

echo "Change NetInfo ......"
echo "ETH=$1"
echo "MAC=$2"
echo "IP=$3"
echo "NETMASK=$4"
echo "GW=$5"

#ifconfig "$1" down 
#sleep 1
#ifconfig "$1" hw ether "$2" 
#ifconfig "$1" up 

#service network stop 
ifconfig "$1" "$3" netmask "$4" 
route add -net default gw "$5" 
#service network start 

echo "Change NetInfo Done."

