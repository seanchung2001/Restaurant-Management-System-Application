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

//QDB FUNCTIONS BELOW

//Just for testing, insert a meta tag tho
int test_insert(qdb_hdl_t* hdl, char* str);

/* Creates a new profile in the database specified by the handle
 * @params
 * hdl: database handle returned by qdb_connect()
 * profile: structure that holds the necessary fields to create a new profile row in the database specified by the hdl
 * @returns
 * 0 if successful
 * -1 if unsuccessful
 * errno will be set
 */
int insert_profile(qdb_hdl_t* hdl, profile_t* profile);

/* Logs into a profile with the specified login and password
 * @params
 * hdl: database handle returned by qdb_connect()
 * login: string for the login
 * password: string for the password
 * @returns
 * 0 if successful
 * -1 if not successful
 */
int login_profile(qdb_hdl_t* hdl, char *login, char *password);


//Implementations
int test_insert(qdb_hdl_t* hdl, char *str){
	return qdb_statement(hdl, "INSERT INTO 'META_TAG' VALUES ('%q');", str);
}

int insert_profile(qdb_hdl_t* hdl, profile_t* profile){
	return qdb_statement(hdl, "INSERT INTO 'PROFILE' VALUES ('%q','%q','%q','%q','%q','%q','%q');", profile->restaurant_name, profile->address, profile->description, profile->opens, profile->closes, profile->login, profile->password);
}

int login_profile(qdb_hdl_t* hdl, char *login, char *password){
	return 0;
}


#endif /* DBFUNCS_H_ */
