#!/bin/bash

warning_message="This script should be executed in directory where backup is placed, e.g where both backup_y-m-d.tar.gz and md5sums_y-m-d files exists";

tar_exists=`ls backup_*tar.gz | wc -l`;
md5_sums_exists=`ls md5sums_* | wc -l`;
if [ $tar_exists != "1" -o $md5_sums_exists != "1" ]
then
	echo $warning_message
	exit 1
fi

echo "========= Extracting backup archive for checking" 
tar -xzf backup_*tar.gz

echo "========= Checking files"
md5sum --check md5sums_*

result_exit_code="0";
if [ $? != "0" ]
then
	echo "========= File verification failed, read the log above"
	result_exit_code="1";	
fi

echo "========= Removing extracted files"
rm `ls | grep -Ev "backup_|md5sums_"`
