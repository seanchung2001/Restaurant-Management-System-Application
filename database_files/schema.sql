-- Database name: restaurant_db

BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS 'profile' (
    'restaurant_name'   TEXT NOT NULL,
    'address'           TEXT NOT NULL,
    'description'       TEXT NOT NULL,
    'opens'             TEXT NOT NULL,  -- "HH:MM" format
    'closes'            TEXT NOT NULL,  -- "HH:MM" format
    'login'             TEXT NOT NULL,
    'password'          TEXT NOT NULL,  -- Look into qnx encrypt() and setkey() functions to store encrypted pass
    PRIMARY KEY('restaurant_name', 'address')
);

CREATE TABLE IF NOT EXISTS 'table' (
    'table_num'         INTEGER PRIMARY KEY,
    'seat_num'          INTEGER NOT NULL,
    'isReserved'        INTEGER NOT NULL   -- 0 or 1 for false or true
);

-- These may be deleted at the end of the their date day
CREATE TABLE IF NOT EXISTS 'reservation'(
    'id'                INTEGER PRIMARY KEY,  -- This should prevent using deleted reservation's ids
    'table_num'      INTEGER NOT NULL,
    'date'              TEXT NOT NULL,      -- "YYYY-MM-DD HH:MM" format
    'first_name'        TEXT NOT NULL,
    'last_name'         TEXT NOT NULL,
    'phone_num'         INTEGER NOT NULL,
    'start_time'        TEXT NOT NULL,      -- "HH:MM"
    'duration'          INTEGER NOT NULL,   -- "HH:MM"
    CONSTRAINT fk_reservation_table_number FOREIGN KEY ('table_num') REFERENCES 'table'('table_num')
);

CREATE TABLE IF NOT EXISTS 'meta_tag'(
    'name'              TEXT PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS 'menu_item'(
    'name'              TEXT PRIMARY KEY,
    'description'       TEXT NOT NULL,
    'price'             REAL NOT NULL, -- Floating point
    'type'              TEXT NOT NULL -- Entree, main, beverage, etc.  
);

CREATE TABLE IF NOT EXISTS 'online_order' (
    'id'                INTEGER PRIMARY KEY,
    'date'              TEXT NOT NULL,      -- "YYYY-MM-DD HH:MM"
    'total'             REAL NOT NULL,      -- Floating point 
    'first_name'        TEXT NOT NULL,
    'last_name'         TEXT NOT NULL,
    'address'           TEXT NOT NULL,
    'phone_num'         INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS 'table_order' (
    'id'                INTEGER PRIMARY KEY,
    'total'             REAL NOT NULL,      -- Floating point
    'date'              TEXT NOT NULL,       -- "YYYY-MM-DD HH:MM"
    'table_num'         INTEGER NOT NULL,
    CONSTRAINT fk_table_order_table_number FOREIGN KEY ('table_num') REFERENCES 'table'('table_num')
);

--START OF THE RELATIONAL TABLES
--They act as a relationship between two tables
--If at least one exists, then there's a connection between
--the tables, otherwise not

-- Represents the tags associated with a table
CREATE TABLE IF NOT EXISTS 'table_tag'(
    'table_num'         INTEGER NOT NULL,
    'name'              TEXT NOT NULL,
    PRIMARY KEY('table_num', 'name'),
    CONSTRAINT fk_table_tag_table_num FOREIGN KEY ('table_num') REFERENCES 'table'('table_num'),
    CONSTRAINT fk_table_tag_name      FOREIGN KEY ('name') REFERENCES 'meta_tag'('name')   
);

CREATE TABLE IF NOT EXISTS 'on_order_item'(
    'name'              TEXT NOT NULL,
    'oid'               INTEGER NOT NULL,
    'count'             INTEGER NOT NULL,   -- Times this same item is ordered by the online order
    PRIMARY KEY('name', 'oid'),
    CONSTRAINT fk_on_order_item_name FOREIGN KEY ('name') REFERENCES 'menu_item'('name'),
    CONSTRAINT fk_on_order_item_oid  FOREIGN KEY ('oid') REFERENCES 'online_order'('id')
);

CREATE TABLE IF NOT EXISTS 'tab_order_item'(
    'name'              TEXT NOT NULL,
    'oid'               INTEGER NOT NULL,
    'count'             INTEGER NOT NULL,   -- Times this same item is ordered by the online order
    PRIMARY KEY('name', 'oid'),
    CONSTRAINT fk_on_order_item_name FOREIGN KEY ('name') REFERENCES 'menu_item'('name'),
    CONSTRAINT fk_on_order_item_oid  FOREIGN KEY ('oid') REFERENCES 'table_order'('id')
);

END TRANSACTION;

