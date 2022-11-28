# Database Setup

## Get the required executables and libraries for qdb to work
### For QDB:
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\sbin
- Copy the "qdb" file into your vms /system/xbin directory

### For libraries for sqlite3:
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
- export TMPDIR=/data/home/root/tmp
- cd /
- cd /etc/mm/sql
    - if this directory doesn't exist, create it with mkdir from the deepest subdirectory
- Copy the schema.sql file into the sql directory

## Run QDB
- Assuming pps is running already just enter:
- qdb
- to start the server
    - it will use the config file under /pps/qnx/qdb/config

## Optional: Check if database works using QDBC
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\bin
- Copy the "qdbc" file into your vms /system/xbin directory
- cd /
- cd /pps/qnx/qdb/config
- qdbc -d restaurant_db
- Should be able to issue sql commands now into the tables defined by the schema
    - Can try: INSERT INTO 'table' ('table_num', 'seat_num', 'isReserved') VALUES (1,5,0);
    - SELECT * FROM 'table';
    - DELETE FROM 'table' WHERE table_num=1;
