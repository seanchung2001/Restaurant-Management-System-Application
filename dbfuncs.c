/*
 * dbfuncs.c
 *
 *  Created on: Nov. 30, 2022
 *      Author: Johnny
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qdb/qdb.h>
#include "server.h"
#include "dbfuncs.h"

int test_insert(qdb_hdl_t* hdl, char *str){
	return qdb_statement(hdl, "INSERT INTO 'META_TAG' VALUES (%q)", str);
}

int insert_profile(qdb_hdl_t* hdl, profile_t* profile){
	return qdb_statement(hdl, "INSERT INTO 'PROFILE' VALUES (%q,%q,%q,%q,%q,%q,%q);", profile->restaurant_name, profile->address, profile->description, profile->opens, profile->closes, profile->login, profile->password);
}

int login_profile(qdb_hdl_t* hdl, char *login, char *password){
	return 0;
}
