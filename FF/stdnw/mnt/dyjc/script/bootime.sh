#!/bin/sh
#function: record boot time to bootime.log

BOOT_LOGFILE=/mnt/data/log/bootime.log
SIZE_LOGFILE=100000

#remove boot log
if [ -r $BOOT_LOGFILE ]; then
	CUR_SIZE=`cat $BOOT_LOGFILE | wc -c`
	if [ $CUR_SIZE -gt $SIZE_LOGFILE ]; then
		rm -f $BOOT_LOGFILE
	fi
fi

#record boot time
date +%f >> $BOOT_LOGFILE


