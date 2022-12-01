/*
 * dbfuncs.h
 *
 *  Created on: Nov. 30, 2022
 *      Author: Johnny
 */
#include <sys/iomsg.h>
#include <qdb/qdb.h>
#include <string.h>
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

/* Processes the result of a query, helper function to prevent code copying.
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int process_result(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols);

/* Retrieves the tables from the database, saves into the out parameters
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int get_tables(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols);

/* Retrieves a tables tags from the database, saves into the out parameters
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 *      It will contain only the 'name' of the tag in each row
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * table_num: the number of the table you want the tags for
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int get_tables_tags(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int table_num);


/* Retrieves the menu items from the database, saves into the out parameters
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int get_menu_items(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols);

/* Retrieves the reservations from the database, saves into the out parameters
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int get_reservations(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols);

/* Retrieves reservations for a given date from the database, saves into the out parameters
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int get_reservation(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int year, int month, int day);

/* Retrieves tables given they're not reserved already and have the min number of people.
 * The results will be in ascending order based on seat_num
 * @params
 * hdl: database handle returned by qdb_connect()
 * res: the out param that will hold the result if it is found. Will be NULL if something went wrong
 * rows: the out param that will hold the number of rows found. Will be 0 if no results. -1 if something went wrong
 * cols: the out param that will hold the number of columns found Will be 0 if no results. -1 if something went wrong
 * num_people: minimum number of people for the table
 * @returns
 * >=0 if successful
 * -1 if unsuccessful, errno will be set
 */
int query_tables(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int num_people);


//++++++++++++++++++ Implementations ++++++++++++++++++

//For testing insertions
void testprofile(qdb_hdl_t* hdl){
	int rc;
	profile_t prof;
	strcpy(prof.restaurant_name, "boring");
	strcpy(prof.address, "fake street");
	strcpy(prof.description, "bad restaurant");
	strcpy(prof.opens, "09:00");
	strcpy(prof.closes, "22:00");
	strcpy(prof.login, "user1");
	strcpy(prof.password, "123");
	rc = insert_profile(hdl, &prof);
	if (rc < 0){
		perror("insert_profile");
		return;
	}

}

void testlogin(qdb_hdl_t* hdl){
	int rc;
	rc = login_profile(hdl, "user1", "123");
	if (rc < 0){
		perror("login_profile");
		return;
	}
}

void testtable(qdb_hdl_t* hdl){
	int rc;
	table_t tab;
	tab.table_num = 12;
	tab.num_seats = 10;
	tab.isReserved = 0;
	rc = insert_table(hdl, &tab);
	if (rc < 0){
		perror("insert_table");
		return;
	}
}

void testtable2(qdb_hdl_t* hdl){
	int rc;
	rc = update_table(hdl, 12, 1);
	if (rc < 0){
		perror("update_table");
		return;
	}
}

void testreservation(qdb_hdl_t* hdl){
	int rc;
	reservations_t res;
	res.table_num = 12;
	res.year = 2022;
	res.month = 12;
	res.day = 1;
	strcpy(res.first_name, "John");
	strcpy(res.last_name, "Joel");
	res.phone_num = 123;
	res.start_hour = 11;
	res.start_min = 30;
	res.end_hour = 12;
	res.end_min = 30;

	rc = insert_reservation(hdl, &res);
	if (rc < 0){
		perror("insert_reservation");
		return;
	}
}

void testmetatag(qdb_hdl_t* hdl){
	int rc;
	rc = insert_meta_tag(hdl, "hall");
	if (rc < 0){
		perror("insert_meta_tag");
		return;
	}
}

void testmenuitem(qdb_hdl_t* hdl){
	int rc;
	menu_item_t m;
	strcpy(m.name, "Pie");
	strcpy(m.description, "Pumpkin pie");
	strcpy(m.type, "dessert");
	m.price = 6.40;

	rc = insert_menu_item(hdl, &m);
	if (rc < 0){
		perror("insert_menu_item");
		return;
	}
}

void testonlineorder(qdb_hdl_t* hdl){
	int rc;
	online_order_t o;
	o.year = 2022;
	o.month = 12;
	o.day = 1;
	o.hour = 20;
	o.minute = 34;
	o.total = 6.40;
	strcpy(o.first_name, "John");
	strcpy(o.last_name, "Joel");
	strcpy(o.address, "fake add");
	o.phone_num = 12345;

	rc = insert_online_order(hdl, &o);
	if (rc < 0){
		perror("insert_online_order");
		return;
	}
	printf("Success: insert_online_order\n");
}

void testtableorder(qdb_hdl_t* hdl){
	int rc;
	table_order_receipt_t t;
	t.total = 6.40;
	t.year = 2022;
	t.month = 12;
	t.day = 1;
	t.table_num = 12;

	rc = insert_table_order(hdl, &t);
	if (rc < 0){
		perror("insert_table_order");
		return;
	}
	printf("Success: insert_table_order\n");
}

void testtabletag(qdb_hdl_t* hdl){
	int rc;
	rc = insert_table_tag(hdl, 12, "hall");
	if (rc < 0){
		perror("insert_table_tag");
		return;
	}
	printf("Success: insert_table_tag\n");
}

void testonorderitem(qdb_hdl_t* hdl){
	int rc;
	int oid = 1; //TODO: replace with the order id created from prev test;
	rc = insert_on_order_item(hdl, "Pie", oid, 5);
	if (rc < 0){
		perror("insert_on_order_item");
		return;
	}
	printf("Success: insert_on_order_item");
}

void testtaborderitem(qdb_hdl_t* hdl){
	int rc;
	int oid = 1; //TODO: replace with the order id created from prev test;
	rc = insert_tab_order_item(hdl, "Pie", oid, 3);
	if (rc < 0){
		perror("insert_tab_order_item");
		return;
	}
	printf("Success: insert_tab_order_item\n");
}

//++++++++++++++++++ INSERTS ++++++++++++++++++
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
		//perror("login_profile statement");
		return -1;
	}
	//Get the results
	res = qdb_getresult(hdl);
	if (res == NULL){
		//perror("login_profile result");
		return -1;
	}
	//Get the rows returned in the result
	rows = qdb_rows(res);
	if (rows <= -1){
		//perror("login_profile rows");
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

//Not really an insertion, just an update
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


//++++++++++++++++++ SELECTS ++++++++++++++++++
int process_result(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols){
	res = qdb_getresult(hdl);
	if (res == NULL){
		return -1;
	}
	*rows = qdb_rows(res);
	*cols = qdb_columns(res);
	if (*rows == -1 || *cols == -1){
		return -1;
	}
	return 0;
}

int get_tables(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols){
	int rc;
	//These will be properly set later unless something goes wrong
	*rows = -1;
	*cols = -1;
	rc = qdb_statement(hdl,"SELECT * FROM 'TABLE';");
	if (rc < 0) return -1;
	return process_result(hdl, res, rows, cols);
}

int get_menu_items(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols){
	int rc;
	//These will be properly set later unless something goes wrong
	*rows = -1;
	*cols = -1;
	rc = qdb_statement(hdl,"SELECT * FROM 'MENU_ITEM';");
	if (rc < 0) return -1;
	return process_result(hdl, res, rows, cols);
}

int get_reservations(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols){
	int rc;
	//These will be properly set later unless something goes wrong
	*rows = -1;
	*cols = -1;
	rc = qdb_statement(hdl,"SELECT * FROM 'RESERVATION';");
	if (rc < 0) return -1;
	return process_result(hdl, res, rows, cols);
}

int get_reservation(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int year, int month, int day){
	int rc;
	//These will be properly set later unless something goes wrong
	*rows = -1;
	*cols = -1;
	rc = qdb_statement(hdl,"SELECT * FROM 'RESERVATION' WHERE year=%d AND month=%d AND day=%d;", year, month, day);
	if (rc < 0) return -1;
	return process_result(hdl, res, rows, cols);
}

int get_tables_tags(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int table_num){
	int rc;
	//These will be properly set later unless something goes wrong
	*rows = -1;
	*cols = -1;
	rc = qdb_statement(hdl,"SELECT (name) FROM 'TABLE' AS A INNER JOIN 'TABLE_TAG' WHERE A.table_num=%d;", table_num);
	if (rc < 0) return -1;
	return process_result(hdl, res, rows, cols);
}

int query_tables(qdb_hdl_t* hdl, qdb_result_t* res, int* rows, int* cols, int num_people){
	int rc;
		//These will be properly set later unless something goes wrong
		*rows = -1;
		*cols = -1;
		rc = qdb_statement(hdl,"SELECT * FROM 'TABLE' WHERE isReserved=0 AND seat_num>=%d ORDER BY seat_num ASC;", num_people);
		if (rc < 0) return -1;
		return process_result(hdl, res, rows, cols);
}

#endif /* DBFUNCS_H_ */
