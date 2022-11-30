# Database Setup

## Get the required executables and libraries for qdb to work
### For QDB:
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\sbin
- Copy the "qdb" file into your vms /system/xbin directory

- For libraries for sqlite3:
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\lib
- Copy the "libsqlite3.so.1" file into your vms /system/lib directory
- Copy the "libstrm.so.1" file into your vms /system/lib directory
- Copy the "libicui18n.so.67" file into your vms /system/lib directory
- Copy the "libicuuc.so.67" file into your vms /system/lib directory
- Copy the "libicudata.so.67" file into your vms /system/lib directory

## Create directories to store database files and schema file
- Start in root:
- cd pps
- mkdir -p qnx/qdb/config
- Copy the restaurant_db file into the config directory
- export TMPDIR=/data/home/root/tmp/db
- cd /
- cd /data/home/root/tmp
- cd /etc/mm/sql
- Copy the schema.sql file into the sql directory
- sqlite3 restaurant_db.db
    - cntrl + c to exit sqlite3
- sqlite3 restaurant_db.db < /etc/mm/sql/schema.sql
    - cntrl + c to exit sqlite3

## Run QDB
- Assuming pps is running already just enter:
- qdb
- to start the server

## Optional: Check if database exists using QDBC
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\bin
- Copy the "qdbc" file into your vms /system/xbin directory
- qdbc -d /pps/qnx/qdb/config/restaurant_db