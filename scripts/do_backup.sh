#!/bin/bash
cd /opt/backup

./check_free_space.sh

sudo ./backup.sh
