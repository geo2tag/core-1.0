#!/bin/bash
ymd=$(date "+%Y-%m-%d");
backup_dir="./db_backup/$ymd";
geo2tag_dir="/opt/geo2tag/";

md5sums_file="md5sums_$ymd";
backup_archive_file="backup_$ymd.tar.gz";

if [ ! -d "$backup_dir" ]; then
	mkdir -p $backup_dir
fi

echo "Saving databases dumps"
db_list=$(sudo -u postgres psql -l -t | awk '{ print $1}' | grep -vE 'template[0|1]' | grep -v '|');
for d in $db_list
do
  echo "Dumping db = $d"
  sudo -u postgres pg_dump $d > $backup_dir/$d.sql
done

echo "Saving configuration files"
cp $geo2tag_dir/geo2tag.conf* $backup_dir/
chown -R backup_operator:backup_operator $backup_dir/..
cd $backup_dir/
tar -cvzf $backup_archive_file ./*
md5sum * > $md5sums_file
rm `ls | grep -Ev "$md5sums_file|$backup_archive_file"`
