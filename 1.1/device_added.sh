#!/bin/bash
mount /dev/sdb1 /home
cp /var/log/syslog /home
cp /var/log/dmesg /home
umount /dev/sdb1