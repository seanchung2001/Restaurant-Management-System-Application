/*
 * dbfuncs.h
 *
 *  Created on: Nov. 30, 2022
 *      Author: Johnny
 */
#include <sys/iomsg.h>
#include <qdb/qdb.h>
#include "server.h"

#ifndef DBFUNCS_H_
#define DBFUNCS_H_
#define DB_NAME "/dev/qdb/restaurant_db" //Use this in qdb_connect(DB_NAME, QDB_CONN_DFLT_SHARE);


//Just for testing
int _test_insert(qdb_hdl_t* hdl, char* str);

//++++++++++++++++++ Function Signatures and Documentation ++++++++++++++++++
/* For most of the functions, you can call qdb_getresult() after to get the result of the query
 * check the documentation for it here: http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.qdb_en_dev_guide%2Fapi%2Fqdb_getresult.html
 * I doubt insert_* functions will have any result, but who knows
 */

/* Creates a new profile in the database specified by the handle
 * @params
 * hdl: database handle returned by qdb_connect()
 * profile: structure that holds the necessary fields to create a new profile row in the database specified by the hdl
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_profile(qdb_hdl_t* hdl, profile_t* profile);

/* Logs into a profile with the specified login and password
 * @params
 * hdl: database handle returned by qdb_connect()
 * login: string for the login
 * password: string for the password
 * @returns
 * >=0 if successful
 * -1 if not successful or something went wrong
 */
int login_profile(qdb_hdl_t* hdl, char *login, char *password);

/* Creates a new table with the specified fields in t
 * @params
 * hdl: database handle returned by qdb_connect()
 * t: structure that holds the fields
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_table(qdb_hdl_t* hdl, table_t* t);

/* Updates a table's reserved status to newval
 * @params
 * hdl: database handle returned by qdb_connect()
 * t_num: table_num of the table to be updated
 * newval: the new value to be assigned to isReserved, should be boolean (0 or 1)
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int update_table(qdb_hdl_t* hdl, int t_num, int newval);

/* Creates a new reservation
 * @params
 * hdl: database handle returned by qdb_connect()
 * r: the structure containing the fields of a reservation
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_reservation(qdb_hdl_t* hdl, reservations_t* r);

/* Creates a new tag for tables
 * @params
 * hdl: database handle returned by qdb_connect()
 * tag: the string for the tag
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_meta_tag(qdb_hdl_t* hdl, char* tag);

/* Creates a new menu item
 * @params
 * hdl: database handle returned by qdb_connect()
 * m: the structure with the fields for the menu item
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_menu_item(qdb_hdl_t* hdl, menu_item_t* m);

/* Creates a new online order
 * @params
 * hdl: database handle returned by qdb_connect()
 * o: the structure with the fields for the order
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_online_order(qdb_hdl_t* hdl, online_order_t* o);

/* Creates a new table order
 * @params
 * hdl: database handle returned by qdb_connect()
 * t: the structure with the fields for the order
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_table_order(qdb_hdl_t* hdl, table_order_receipt_t* t);

/* Creates a new table tag relation for a table
 * @params
 * hdl: database handle returned by qdb_connect()
 * table_num: the table's number
 * name: the name of the meta tag for the table to be associated with
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_table_tag(qdb_hdl_t* hdl, int table_num, char* name);

/* Creates a new online order item relation for an online order
 * @params
 * hdl: database handle returned by qdb_connect()
 * name: the name of the menu item ordered
 * oid: the id of the order
 * count: number of the item ordered
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_on_order_item(qdb_hdl_t* hdl, char* name, int oid, int count);

/* Creates a new table order item relation for a table order
 * @params
 * hdl: database handle returned by qdb_connect()
 * name: the name of the menu item ordered
 * oid: the id of the order
 * count: number of the item ordered
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int insert_tab_order_item(qdb_hdl_t* hdl, char* name, int oid, int count);


//++++++++++++++++++ Implementations ++++++++++++++++++

int _test_insert(qdb_hdl_t* hdl, char *str){
	return qdb_statement(hdl, "INSERT INTO 'META_TAG' VALUES ('%q');", str);
}

int insert_profile(qdb_hdl_t* hdl, profile_t* profile){
	return qdb_statement(hdl, "INSERT INTO 'PROFILE' VALUES ('%q','%q','%q','%q','%q','%q','%q');", profile->restaurant_name, profile->address, profile->description, profile->opens, profile->closes, profile->login, profile->password);
}

int login_profile(qdb_hdl_t* hdl, char *login, char *password){
	qdb_result_t* res;
	int rc;
	int rows;
	rc = qdb_statement(hdl, "SELECT * FROM 'PROFILE' WHERE login='%q' AND password='%q';", login, password);
	if (rc == -1){
		perror("login_profile statement");
		return -1;
	}
	//Get the results
	res = qdb_getresult(hdl);
	if (res == NULL){
		perror("login_profile result");
		return -1;
	}
	//Get the rows returned in the result
	rows = qdb_rows(res);
	if (rows <= -1){
		perror("login_profile rows");
		return -1;
	}
	//No profile with this login and password
	if (rows == 0) return -1;
	//At least one profile found, login successful
	return 0;
}

int insert_table(qdb_hdl_t* hdl, table_t* t){
	return qdb_statement(hdl, "INSERT INTO 'TABLE' VALUES (%d,%d,%d);", (int)t->table_num, (int)t->num_seats, (int)t->isReserved);
}

int update_table(qdb_hdl_t* hdl, int t_num, int newval){
	return qdb_statement(hdl, "UPDATE 'TABLE' SET isReserved=%d WHERE table_num=%d;", newval, t_num);
}

int insert_reservation(qdb_hdl_t* hdl, reservations_t* r){
	return qdb_statement(hdl, "INSERT INTO 'RESERVATION' ('table_num','year','month','day','first_name','last_name','phone_num','start_hour','start_min','end_hour','end_min') VALUES (%d,%d,%d,%d,'%q','%q',%d,%d,%d,%d,%d);", (int)r->table_num, (int)r->year, (int)r->month, (int)r->day, r->first_name, r->last_name, (int)r->phone_num, (int)r->start_hour, (int)r->start_min, (int)r->end_hour, (int)r->end_min);
}

int insert_meta_tag(qdb_hdl_t* hdl, char* tag){
	return qdb_statement(hdl, "INSERT INTO 'META_TAG' VALUES ('%q');", tag);
}

int insert_menu_item(qdb_hdl_t* hdl, menu_item_t* m){
	return qdb_statement(hdl, "INSERT INTO 'MENU_ITEM' VALUES ('%q','%q',%f,'%q');", m->name, m->description, m->price, m->type);
}

int insert_online_order(qdb_hdl_t* hdl, online_order_t* o){
	return qdb_statement(hdl, "INSERT INTO 'ONLINE_ORDER' ('year','month','day','hour','min','total','first_name','last_name','address','phone_num') VALUES (%d,%d,%d,%d,%d,%f,'%q','%q','%q',%d);",(int)o->year,(int)o->month,(int)o->day,(int)o->hour,(int)o->minute,o->total,o->first_name,o->last_name,o->address,(int)o->phone_num);
}

int insert_table_order(qdb_hdl_t* hdl, table_order_receipt_t* t){
	return qdb_statement(hdl, "INSERT INTO 'TABLE_ORDER' ('total','year','month','day','table_num') VALUES (%f,%d,%d,%d,%d);",(int)t->total,(int)t->year,(int)t->month,(int)t->day,(int)t->table_num);
}

int insert_table_tag(qdb_hdl_t* hdl, int table_num, char* name){
	return qdb_statement(hdl, "INSERT INTO 'TABLE_TAG' VALUES (%d,'%q');",table_num,name);
}

int insert_on_order_item(qdb_hdl_t* hdl, char* name, int oid, int count){
	return qdb_statement(hdl, "INSERT INTO 'ON_ORDER_ITEM' VALUES ('%q',%d,%d);", name, oid, count);
}

int insert_tab_order_item(qdb_hdl_t* hdl, char* name, int oid, int count){
	return qdb_statement(hdl, "INSERT INTO 'TAB_ORDER_ITEM' VALUES ('%q',%d,%d);", name, oid, count);
}

#endif /* DBFUNCS_H_ */
