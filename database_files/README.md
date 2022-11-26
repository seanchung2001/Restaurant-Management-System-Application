# Database Setup

## Get the required executables and libraries for qdb to work
- For QDB:
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\sbin
- Copy the "qdb" file into your vms /system/xbin directory

- For libraries for sqlite3:
- On windows, visit: C:\Users<your_user>\qnx710\target\qnx7\x86_64\usr\lib
- Copy the "libsqlite3.so.1" file into your vms /system/lib directory
- Copy the "libstrm.so.1" file into your vms /system/lib directory
- Copy the "libicui18n.so.67" file into your vms /system/lib directory
- Copy the "libicuuc.so.67" file into your vms /system/lib directory
- Copy the "libicudata.so.67" file into your vms /system/lib directory
- 

## Create directories to store database files and schema file
- Start in root:
- cd etc
- mkdir -p mm/sql
- cd /
- cd tmp
- mkdir restaurant_db

