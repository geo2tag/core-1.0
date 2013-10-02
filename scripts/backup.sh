#!/bin/bash
backup_dir="./db_backup";
geo2tag_dir="/opt/geo2tag/";

if [ ! -d "$backup_dir" ]; then
	mkdir $backup_dir
fi



echo $backup_dir
db_list=$(sudo -u postgres psql -l | awk '{ print $1}' | grep -vE '^-|^List|^Name|template[0|1]' | grep -v '|' | head -n -2);
for d in $db_list
do
  sudo -u postgres pg_dump $d | gzip -c >  $backup_dir/$d.out.gz
done
cp $geo2tag_dir/geo2tag.conf* $backup_dir
