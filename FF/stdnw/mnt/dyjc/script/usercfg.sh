#!/bin/sh
#function: user config

#make log dir
mkdir -p /mnt/data/log/

#change IP
/mnt/dyjc/script/ipconfig.sh

#run update.sh
if [ -r /mnt/dyjc/script/update.sh ]; then
	echo "Do some work after update......"
	chmod +x /mnt/dyjc/script/update.sh
	/mnt/dyjc/script/update.sh
	rm -f /mnt/dyjc/script/update.sh
fi

