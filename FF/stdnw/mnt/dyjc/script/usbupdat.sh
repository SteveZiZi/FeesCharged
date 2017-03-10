#!/bin/sh
#function: usb update control script

USBLOGFILE=/mnt/data/log/usb.log
USBROOTDIR=/usb/kge

#remove usb log
#rm -f $USBLOGFILE

#prompt
#echo "`date +%f`: Usb device plug in......" >> $USBLOGFILE

#copy system info
BACKUPDIR=$USBROOTDIR/backup
BACKUPFLAG=$BACKUPDIR/backup.key
DEVFILE=/mnt/system/address.ini
DEVADDR=$(grep '^Address' $DEVFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g')
CURDATE=$(date +%m%d%H%M)
if [ "$DEVADDR" == "" ]; then
	DEVADDR=backup
fi
BACKUPDEVDIR=$BACKUPDIR/$DEVADDR
if [ -r $BACKUPFLAG ]; then
	echo "`date +%f`: Copy system info start......" >> $USBLOGFILE
	mkdir -p $BACKUPDEVDIR
	rm -f $BACKUPDEVDIR/$CURDATE.tgz
	tar -cvf $BACKUPDEVDIR/$CURDATE.tgz /mnt/data /mnt/dyjc /mnt/system >> $USBLOGFILE
	echo "`date +%f`: Copy system info OK!" >> $USBLOGFILE
else
	echo "`date +%f`: Backup flag ($BACKUPFLAG) is not existing!" >> $USBLOGFILE
fi

#update system
USBUPDTKEY=$USBROOTDIR/update/update.key
USBUPDTMD5=$USBROOTDIR/update/update.md5
USBUPDTTGZ=$USBROOTDIR/update/update.tgz
CURVERFILE=/mnt/system/sysrun.inf

if [ -r $USBUPDTKEY ]; then
	UPDTVER=$(grep '^MPVersion' $USBUPDTKEY | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g' | sed 's/[.]//g')
fi
if [ -r $CURVERFILE ]; then
	CURRVER=$(grep '^MPVersion' $CURVERFILE | cut -f1 -d# | cut -f2 -d= | sed 's/[[:space:]]//g' | sed 's/[.]//g')
fi
if [ "$UPDTVER" == "" ]; then
	UPDTVER=999
fi
if [ "$CURRVER" == "" ]; then
	CURRVER=999
fi
if [ -r $USBUPDTKEY ]; then
	echo "`date +%f`: Find update key: $USBUPDTKEY" >> $USBLOGFILE
	if [ "$UPDTVER" != "$CURRVER" ]; then
		echo "`date +%f`: System will update ($CURRVER -> $UPDTVER)" >> $USBLOGFILE
		mkdir -p /mnt/update
		cp -f $USBUPDTTGZ /mnt/update/update.tgz
		cp -f $USBUPDTMD5 /mnt/update/update.md5
		echo "`date +%f`: System will reboot......" >> $USBLOGFILE
		touch /mnt/update/update.key
	else
		echo "`date +%f`: Can't update, because of update version($UPDTVER) same as current system version($CURRVER)!" >> $USBLOGFILE
	fi
else
	echo "`date +%f`: Not find update key: $USBUPDTKEY" >> $USBLOGFILE
fi

#exec user script
USERSCRIPT=$USBROOTDIR/script/kgeusbop.sh
if [ -r $USBUPDTKEY ]; then
	echo "`date +%f`: System need update,can not execute usb script!" >> $USBLOGFILE
elif [ -r $USERSCRIPT ]; then
	echo "`date +%f`: Exec user script($USERSCRIPT) start......" >> $USBLOGFILE
	cp -f $USERSCRIPT /tmp/kgeusbop.sh
	chmod +x /tmp/kgeusbop.sh
	/tmp/kgeusbop.sh
	echo "`date +%f`: Exec user script OK!" >> $USBLOGFILE
else
	echo "`date +%f`: Not find user script: $USERSCRIPT" >> $USBLOGFILE
fi

#copy log file
cp -f $USBLOGFILE $USBROOTDIR/usblog.txt

#umount usb device
umount /usb

