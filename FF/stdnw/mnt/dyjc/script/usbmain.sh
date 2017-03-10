#!/bin/sh
#function: usb main control script

USBPLUGINFLAG=/usb/kgedummy.flg
USBLOGFILE=/mnt/data/log/usb.log
USBTESTFLAG=/usb/factest/testusb.flg

#update usb disk flag
touch $USBPLUGINFLAG
#remove usb log
rm -f $USBLOGFILE
#prompt
echo "`date +%f`: USB device plug in......" >> $USBLOGFILE

if [ -r $USBTESTFLAG ]; then
	echo "`date +%f`: Factory testing......" >> $USBLOGFILE
else
	/mnt/dyjc/script/usbupdat.sh
fi

