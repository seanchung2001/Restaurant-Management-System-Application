#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "server.h"
#include "dbfuncs.h"
#include <string.h>

typedef union
{
	uint16_t type;
	struct _pulse pulse;
	recv_msg_reservation_t reservation;
	recv_msg_online_order_t online_order;
	recv_msg_table_order_t table_order;
	recv_get_table_in_house_t get_table;
	recv_table_print_receipt_t print_receipt;
} recv_buf_t;

//Function forward declaration
int create_reservation(int num_seats, char *meta_tag, int year, int month, int day, char *fName, char *lName, int phoneNum, int start_hour, int start_min, int duration_hour, int duration_min, int curYear, int curMonth, int curDay, int resID);
float create_online_order(int year, int month, int day, int hour, int minute, char *fName, char *lName, char *address, int phoneNum, int curYear, int curMonth, int curDay, int orderID);
int create_table_order(int table_num, int orderID);
int get_table_in_house(int num_people);
float table_print_receipt(int table_num);
int calculate_current_hour(int hour);
int calculate_current_month(int month);
int calculate_current_year(int year);

//Database structs
table_t tables[MAX_NUM_TABLES];\
//For debugging, tables will be read in from database
table_t table1;
table_t table2;
table_t table3;
table_t table4;
table_t table5;
int tables_size = 5;

menu_item_t menu[MAX_MENU_ITEMS];
//For debugging, menu items will be read in from database
menu_item_t menuItem1;
menu_item_t menuItem2;
menu_item_t menuItem3;

table_order_item_t table_orders_existing[MAX_TABLE_ORDERS];
int num_existing_orders = 0;

table_order_receipt_t table_order_receipts[MAX_TABLE_RECEIPTS];
int num_table_receipts = 0;

table_tag_t table_tags[MAX_TABLE_TAGS];
//For debugging use, table tags will be read in from database
table_tag_t tag1;
table_tag_t tag2;
table_tag_t tag3;
table_tag_t tag4;
table_tag_t tag5;

reservations_t today_reservations[MAX_NUM_RESERVATIONS];
int num_today_reservations = 0;
reservations_t future_reservations[MAX_NUM_RESERVATIONS];
int num_future_reservations = 0;

online_order_item_t online_order_items[MAX_ONLINE_ORDERS];
int num_online_order_items = 0;

online_order_t online_orders_today[MAX_ONLINE_ORDERS];
int num_online_orders_today = 0;
online_order_t online_orders_future[MAX_ONLINE_ORDERS];
int num_online_orders_future = 0;

int main(void) {

	//Connect to database
	//the hdl can be passed to dbfuncs to access the opened connection
	qdb_hdl_t* hdl;
	hdl = qdb_connect(DB_NAME, QDB_CONN_DFLT_SHARE);
	if (hdl == NULL) {
		perror("qdb_connect");
	}

//	char test_tag[] = "corner";
//  int rc;
//	rc = _test_insert(hdl, test_tag);
//	if (rc < 0){
//		perror("test_insert");
//	}
//	printf("test_insert returned: %d\n", rc);
//	return 0;

	//sean's variables
	name_attach_t *attach;
	int rcvid;
	recv_buf_t client_msg;

	//Set up a periodic timer
	struct sigevent sigevent;
	struct itimerspec itime;
	timer_t timerID;
	int coid;
	int get_table_num;
	int create_table_order_flag;
	int create_reservation_flag;
	float online_order_flag;
	float printReceiptTotal;
	int tableOrderID = 1000;
	int reservationID = 2000;
	int onlineOrderID = 3000;

	//Current date and time variables
	time_t curDate = time(NULL);
	struct tm dateTime = *localtime(&curDate);
	printf("now: %d-%d-%d %d:%d\n", dateTime.tm_year+1900, dateTime.tm_mon+1, dateTime.tm_mday, calculate_current_hour(dateTime.tm_hour), dateTime.tm_min);

	//generate tables for testing
	table1.table_num = 1;
	table1.isReserved = 0;
	table1.num_seats = 5;
	tables[0] = table1;
	table2.table_num = 2;
	table2.isReserved = 0;
	table2.num_seats = 4;
	tables[1] = table2;
	table3.table_num = 3;
	table3.isReserved = 0;
	table3.num_seats = 8;
	tables[2] = table3;
	table4.table_num = 4;
	table4.isReserved = 0;
	table4.num_seats = 16;
	tables[3] = table4;
	table5.table_num = 5;
	table5.isReserved = 0;
	table5.num_seats = 2;
	tables[4] = table5;

	//generate menu for testing
	strcpy(menuItem1.name, "steak and lobster");
	strcpy(menuItem1.description, "7oz sirloin steak and lobster tail");
	menuItem1.price = 27.99;
	strcpy(menuItem1.type, "Main");
	menuItem1.count = 0;
	menu[0] = menuItem1;
	strcpy(menuItem2.name, "french fries");
	strcpy(menuItem2.description, "french fries seasoned with salt and pepper");
	menuItem2.price = 6.99;
	strcpy(menuItem2.type, "side dish");
	menuItem2.count = 0;
	menu[1] = menuItem2;
	strcpy(menuItem3.name, "root beer float");
	strcpy(menuItem3.description, "16oz of root beer with a scoop of vanilla ice cream");
	menuItem3.price = 5.99;
	strcpy(menuItem3.type, "beverage");
	menuItem3.count = 0;
	menu[2] = menuItem3;

	//enerate table tags for testing
	strcpy(tag1.meta_tag_name, "outdoor");
	strcpy(tag2.meta_tag_name, "bar");
	strcpy(tag3.meta_tag_name, "window");
	strcpy(tag4.meta_tag_name, "window");
	strcpy(tag5.meta_tag_name, "party");
	tag1.table_num = 1;
	tag2.table_num = 2;
	tag3.table_num = 2;
	tag4.table_num = 3;
	tag5.table_num = 4;
	table_tags[0] = tag1;
	table_tags[1] = tag2;
	table_tags[2] = tag3;
	table_tags[3] = tag4;
	table_tags[4] = tag5;

	//generate reservation for testing
	/*reservations_t res1;
	res1.day = 29;
	res1.end_hour = 0;
	res1.end_min = 1;
	strcpy(res1.first_name, "Sean");
	strcpy(res1.last_name, "Chung");
	res1.month = 11;
	strcpy(res1.phoneNum, "613-242-7897");
	res1.start_hour = 16;
	res1.start_min = 50;
	res1.year = 2022;
	res1.table_num = 2;
	res1.id = 2002;
	res1.status = 0;
	today_reservations[0] = res1;
	num_today_reservations++;*/

	//mohammad's section

	//sean's section
	//register for our name for a channel
	attach = name_attach(NULL, SERVER_NAME, 0);
	coid = ConnectAttach(0, 0, attach->chid, _NTO_SIDE_CHANNEL, 0);
	SIGEV_PULSE_INIT(&sigevent, coid, 2, PERIODIC_TIMER_PULSE_CODE, 0);
	//will need to redesign to a one shot timer
	timer_create(CLOCK_REALTIME, &sigevent, &timerID);

	itime.it_value.tv_sec = 60; //1 minute expire time
	itime.it_value.tv_nsec = 0;
	itime.it_interval.tv_sec = 60; //expire every 1 minute
	itime.it_interval.tv_nsec = 0;
	timer_settime(timerID, 0, &itime, NULL);

	while (1) {
		//receive message/pulse
		rcvid = MsgReceive(attach->chid, &client_msg, sizeof(client_msg), NULL);
		if (0 == rcvid) {
			//received a pulse
			switch (client_msg.pulse.code) {
				case PERIODIC_TIMER_PULSE_CODE:
					//Check for reservations
					curDate = time(NULL);
					dateTime = *localtime(&curDate);
					printf("now: %d-%d-%d %d:%d\n", dateTime.tm_year+1900, dateTime.tm_mon+1, dateTime.tm_mday, calculate_current_hour(dateTime.tm_hour), dateTime.tm_min);
					for (int i = 0; i < num_today_reservations; i++) {
						//check if any reservations have started to set table to isReserved
						if (today_reservations[i].start_hour == calculate_current_hour(dateTime.tm_hour) && today_reservations[i].start_min == dateTime.tm_min) {
							tables[today_reservations[i].table_num-1].isReserved = 1;
							today_reservations[i].status = 1;
							printf("reservation started\n");
						}
						//check if any reservations have ended to free up tables
						if (today_reservations[i].end_hour == calculate_current_hour(dateTime.tm_hour) && today_reservations[i].end_min == dateTime.tm_min) {
							tables[today_reservations[i].table_num-1].isReserved = 0;
							today_reservations[i].status = 2;
							printf("reservation ended\n");
						}
					}
					break;
			}
		}
		else{
			// we got a message, check its type and process the msg based on its type
			printf("we got a message\n");
			printf("msg type: %d\n", client_msg.type);
			switch(client_msg.type) {
				case CREATE_RESERVATION_MSG_TYPE:
					create_reservation_flag = create_reservation(client_msg.reservation.num_people, client_msg.reservation.table_meta_tag, client_msg.reservation.year, client_msg.reservation.month, client_msg.reservation.day, client_msg.reservation.first_name, client_msg.reservation.last_name, client_msg.reservation.phone_num, client_msg.reservation.start_hour, client_msg.reservation.start_min, client_msg.reservation.end_hour, client_msg.reservation.end_min, calculate_current_year(dateTime.tm_year), calculate_current_month(dateTime.tm_mon), dateTime.tm_mday, reservationID);
					resp_msg_reservation_t resp_reservation_to_client;
					if (create_reservation_flag > 0) {
						resp_msg_reservation_t resp_reservation_to_client;
						resp_reservation_to_client.reservationID = reservationID;
						resp_reservation_to_client.table_num = create_reservation_flag;
						MsgReply(rcvid, 0, &resp_reservation_to_client, sizeof(resp_reservation_to_client));
						reservationID++;
						printf("reservation successful\n");
					}
					else {
						//If an error occured, return -1
						resp_reservation_to_client.reservationID = -1;
						resp_reservation_to_client.table_num = -1;
						MsgReply(rcvid, 0, &resp_reservation_to_client, sizeof(resp_reservation_to_client));
						printf("reservation failed\n");
					}
					break;
				case CREATE_ONLINE_ORDER_MSG_TYPE:
					//Iterate for the maximum number of orders a client can make online (16), or until the list is finished
					for (int i = 0; i < MAX_ONLINE_ORDER_ITEMS; i++){
						//Check if the order is complete
						if (strcmp(client_msg.online_order.menu_items[i], "done") == 0) {
							break;
						}
						//Find how many orders of each food
						for (int j = 0; j < MAX_MENU_ITEMS; j++) {
							if (strcmp(menu[j].name, client_msg.online_order.menu_items[i]) == 0) {
								menu[j].count++;
								break;
							}
							if (j == MAX_MENU_ITEMS-1) {
								//Client chose a menu item not on the menu or maximum number of items is reached (error number -1)
								online_order_flag = -1;
								MsgReply(rcvid, 0, &online_order_flag, sizeof(online_order_flag));
								printf("online order failed\n");
							}
						}
					}
					online_order_flag = create_online_order(client_msg.online_order.year, client_msg.online_order.month, client_msg.online_order.day, client_msg.online_order.hour, client_msg.online_order.minutes, client_msg.online_order.first_name, client_msg.online_order.last_name, client_msg.online_order.address, client_msg.online_order.phone_num, calculate_current_year(dateTime.tm_year), calculate_current_month(dateTime.tm_mon), dateTime.tm_mday, onlineOrderID);
					resp_msg_online_order_t online_order_resp;
					if (online_order_flag <= 0) {
						//if online order failed
						online_order_resp.orderID = -1;
						online_order_resp.total = 0;
						MsgReply(rcvid, 0, &online_order_resp, sizeof(online_order_resp));
						printf("online order failed\n");
					}
					else {
						//This will send the total amount of the order along with the order ID as a message back to the client
						online_order_resp.orderID = onlineOrderID;
						online_order_resp.total = online_order_flag;
						MsgReply(rcvid, 0, &online_order_resp, sizeof(online_order_resp));
						onlineOrderID++;
						printf("online order success\n");
					}
					break;
				case CREATE_TABLE_ORDER_MSG_TYPE:
					//Iterate for the maximum number of orders a client can make online (16), or until the list is finished
					for (int i = 0; i < MAX_TABLE_ORDER_ITEMS; i++){
						if (strcmp(client_msg.table_order.menu_items[i], "done") == 0) {
							break;
						}
						//Find how many orders of each food
						for (int j = 0; j < MAX_MENU_ITEMS; j++) {
							if (strcmp(menu[j].name, client_msg.table_order.menu_items[i]) == 0) {
								menu[j].count++;
								break;
							}
							if (j == MAX_MENU_ITEMS-1) {
								//Client chose a menu item not on the menu (error number -1) or maximum number of menu items has been exceeded (16)
								create_table_order_flag = -1;
								MsgReply(rcvid, 0, &create_table_order_flag, sizeof(create_table_order_flag));
								printf("table order failed\n");
							}
						}
					}
					create_table_order_flag = create_table_order(client_msg.table_order.table_num, tableOrderID);
					if (create_table_order_flag == 0) {
						//This will send the total amount of the order as a message back to the client
						MsgReply(rcvid, 0, &tableOrderID, sizeof(tableOrderID));
						tableOrderID++;
						printf("table order successful\n");
					}
					else {
						MsgReply(rcvid, 0, &create_table_order_flag, sizeof(create_table_order_flag));
						printf("table order failed\n");
					}
					break;
				case GET_TABLE_IN_HOUSE_MSG_TYPE:
					get_table_num = get_table_in_house(client_msg.get_table.num_people);
					MsgReply(rcvid, 0, &get_table_num, sizeof(get_table_num));
					printf("get table in house successful");
					break;
				case GET_PRINT_RECEIPT_MSG_TYPE:
					printReceiptTotal = table_print_receipt(client_msg.print_receipt.table_num);
					resp_table_print_receipt_t print_receipt_response;
					table_order_receipt_t tmpTable_receipt;
					if (printReceiptTotal > 0) {
						//Get the order ID in here and clear up any orders that are completed
						for (int i = 0; i < num_existing_orders; i++) {
							if (table_orders_existing[i].table_num == client_msg.print_receipt.table_num) {
								print_receipt_response.orderID = table_orders_existing[i].order_id;
								tmpTable_receipt.orderID = table_orders_existing[i].order_id;
								table_orders_existing[i].isComplete = 1;
							}
						}
						//Create the table receipt
						tmpTable_receipt.total = printReceiptTotal;
						tmpTable_receipt.year = calculate_current_year(dateTime.tm_year);
						tmpTable_receipt.month = calculate_current_month(dateTime.tm_mon);
						tmpTable_receipt.day = dateTime.tm_mday;
						tmpTable_receipt.table_num = client_msg.print_receipt.table_num;
						table_order_receipts[num_table_receipts++] = tmpTable_receipt;

						//Set the table to not reserved
						tables[client_msg.print_receipt.table_num-1].isReserved = 0;

						//Create the response to send to client
						print_receipt_response.table_num = client_msg.print_receipt.table_num;
						print_receipt_response.total = printReceiptTotal;
						printf("print receipt successful\n");
					}
					else{
						//If there exists no orders for that table, it failed
						print_receipt_response.orderID = -1;
						print_receipt_response.table_num = -1;
						print_receipt_response.total = 0;
						printf("print receipt failed\n");
					}
					MsgReply(rcvid, 0, &print_receipt_response, sizeof(print_receipt_response));
					break;
				default:
					printf("None of the message types were chosen\n");
					break;
			}
			//TODO: Read and write to database and update array of structs
		}
	}

return 0;
}
//Check if it is in the future
int create_reservation(int num_seats, char *meta_tag, int year, int month, int day, char *fName, char *lName, int phoneNum, int start_hour, int start_min, int duration_hour, int duration_min, int curYear, int curMonth, int curDay, int resID)
{
	int cpyNumSeats = num_seats;
	int found_table = 0;
	int flag = 0;
	if (curYear == year && curMonth == month && curDay == day) {
		while (cpyNumSeats <= MAX_NUM_SEATS) {
			for (int i = 0; i < tables_size; i++) {
				if (tables[i].isReserved == 0 && tables[i].num_seats == cpyNumSeats) {
					for(int k = 0; k < MAX_TABLE_TAGS; k++) {
						printf("table num: %d, table num seats: %d\n", tables[i].table_num, tables[i].num_seats);
						if (strcmp(table_tags[k].meta_tag_name, meta_tag) == 0 && table_tags[k].table_num == tables[i].table_num) {
							printf("table num: %d, table num seats: %d, table tag: %s\n", tables[i].table_num, tables[i].num_seats, table_tags[k].meta_tag_name);
							for (int j = 0; j < num_today_reservations; j++) {
								if (today_reservations[j].status == 0) {
									if ((today_reservations[j].table_num == tables[i].table_num && today_reservations[j].start_hour == start_hour) || (today_reservations[j].table_num == tables[i].table_num && today_reservations[j].start_hour == duration_hour) || (today_reservations[j].table_num == tables[i].table_num && today_reservations[j].end_hour == start_hour) || (today_reservations[j].table_num == tables[i].table_num && today_reservations[j].end_hour == duration_hour)) {
										flag = 1;
										printf("reservation was here before\n");
										break;
									}
								}
							}
							if (flag == 1) {
								flag = 0;
								break;
							}
							else {
								found_table = tables[i].table_num;
								break;
							}
						}
					}
				}
				if (found_table > 0) {
					break;
				}
			}
			if (found_table > 0) {
				break;
			}
			cpyNumSeats++;
		}

		if (found_table == 0) {
			//No tables were found
			return 0;
		}
		reservations_t tmpReservation;
		tmpReservation.day = day;
		tmpReservation.end_hour = duration_hour;
		tmpReservation.end_min = duration_min;
		strcpy(tmpReservation.first_name, fName);
		tmpReservation.id = resID;
		strcpy(tmpReservation.last_name, lName);
		tmpReservation.month = month;
		tmpReservation.phone_num = phoneNum;
		tmpReservation.start_hour = start_hour;
		tmpReservation.start_min = start_min;
		tmpReservation.status = 0;
		tmpReservation.table_num = found_table;
		tmpReservation.year = year;
		today_reservations[num_today_reservations++] = tmpReservation;
		return found_table;
	}
	else {
		while (cpyNumSeats <= MAX_NUM_SEATS) {
			for (int i = 0; i < tables_size; i++) {
				if (tables[i].num_seats == cpyNumSeats) {
					for(int k = 0; k < MAX_TABLE_TAGS; k++) {
						//printf("table num: %d, table num seats: %d, table tag: %s\n", tables[i].table_num, tables[i].num_seats, table_tags[k].meta_tag_name);
						if (strcmp(table_tags[k].meta_tag_name, meta_tag) == 0 && table_tags[k].table_num == tables[i].table_num) {
							printf("table num: %d, table num seats: %d, table tag: %s\n", tables[i].table_num, tables[i].num_seats, table_tags[k].meta_tag_name);
							for (int j = 0; j < num_future_reservations; j++) {
								if (future_reservations[j].table_num == tables[i].table_num && future_reservations[j].year == year && future_reservations[j].month == month && future_reservations[j].day == day && future_reservations[j].status == 0) {
									if (future_reservations[j].start_hour == start_hour || future_reservations[j].start_hour == duration_hour || future_reservations[j].end_hour == start_hour || future_reservations[j].end_hour == duration_hour) {
										flag = 1;
										printf("reservation was here before\n");
										break;
									}
								}
							}
							if (flag == 1) {
								flag = 0;
								break;
							}
							else {
								found_table = tables[i].table_num;
								break;
							}
						}
					}
				}
				if (found_table > 0) {
					break;
				}
			}
			if(found_table > 0) {
				break;
			}
			cpyNumSeats++;
		}
		if (found_table == 0) {
			//No tables were found
			return 0;
		}
		reservations_t tmpReservation;
		tmpReservation.day = day;
		tmpReservation.end_hour = duration_hour;
		tmpReservation.end_min = duration_min;
		strcpy(tmpReservation.first_name, fName);
		strcpy(tmpReservation.last_name, lName);
		tmpReservation.month = month;
		tmpReservation.phone_num = phoneNum;
		tmpReservation.start_hour = start_hour;
		tmpReservation.start_min = start_min;
		tmpReservation.status = 0;
		tmpReservation.table_num = found_table;
		tmpReservation.year = year;
		tmpReservation.id = resID;
		future_reservations[num_today_reservations++] = tmpReservation;
	}
	return found_table;
}

float create_online_order(int year, int month, int day, int hour, int minute, char *fName, char *lName, char *address, int phoneNum, int curYear, int curMonth, int curDay, int orderID)
{
	float total = 0;
	//Error check for seeing if the online order made is before the current day (invalid)
	if ((curYear >= year && curMonth >= month && curDay > day) || (curYear >= year && curMonth > month) || (curYear > year)) {
		return -1;
	}
	//Create online_order_items
	for (int i = 0; i < MAX_MENU_ITEMS; i++) {
		if (menu[i].count > 0) {
			online_order_item_t tmpOnlineOrder;
			strcpy(tmpOnlineOrder.menu_item_name, menu[i].name);
			tmpOnlineOrder.orderID = orderID;
			tmpOnlineOrder.count = 0;
			printf("menu[i] name: %s, count%d\n", menu[i].name, menu[i].count);
			for (int j = 0; j < menu[i].count; j++) {
				tmpOnlineOrder.count += 1;
				total += menu[i].price;
			}
			online_order_items[num_online_order_items++] = tmpOnlineOrder;
			menu[i].count = 0;
		}
	}

	//Create online order
	online_order_t makeOnlineOrder;
	strcpy(makeOnlineOrder.address, address);
	makeOnlineOrder.day = day;
	strcpy(makeOnlineOrder.first_name, fName);
	makeOnlineOrder.hour = hour;
	makeOnlineOrder.id = orderID;
	strcpy(makeOnlineOrder.last_name, lName);
	makeOnlineOrder.minute = minute;
	makeOnlineOrder.month = month;
	makeOnlineOrder.phone_num = phoneNum;
	makeOnlineOrder.year = year;
	makeOnlineOrder.total = total;

	if (curYear == year && curMonth == month && curDay == day) {
		online_orders_today[num_online_orders_today++] = makeOnlineOrder;
	}
	else {
		online_orders_future[num_online_orders_future++] = makeOnlineOrder;
	}
	//If there were no errors, return total
	for (int i = 0; i < num_online_orders_today; i++) {
		printf("online order today total: %f\n", online_orders_today[i].total);
	}
	for (int i = 0; i < num_online_order_items; i++) {
		printf("online order items name: %s, count: %d, id: %d\n", online_order_items[i].menu_item_name, online_order_items[i].count, online_order_items[i].orderID);
	}
	for (int i = 0; i < MAX_MENU_ITEMS; i++) {
		printf("menu name: %s, count: %d\n", menu[i].name, menu[i].count);
	}
	return total;
}

int create_table_order(int table_num, int orderID) {
	//Check if table isReserved
	/*if (tables[table_num-1].isReserved == 0) {
		//The table is currently not being used, return error
		return -1;
	}*/
	for (int i = 0; i < MAX_MENU_ITEMS; i++) {
		if (menu[i].count > 0) {
			printf("hi\n");
			printf("menu[i]: %s\n", menu[i].name);
			table_order_item_t tmpTableOrder;
			strcpy(tmpTableOrder.menu_item_name, menu[i].name);
			tmpTableOrder.table_num = table_num;
			tmpTableOrder.order_id = orderID;
			tmpTableOrder.count = 0;
			tmpTableOrder.isComplete = 0;
			for (int j = 0; j < menu[i].count; j++) {
				tmpTableOrder.count += 1;
			}
			table_orders_existing[num_existing_orders++] = tmpTableOrder;
			menu[i].count = 0;
		}
	}
	for (int i = 0; i < num_existing_orders; i++) {
		printf("table_orders_existing table num: %d, menu name: %s, orderID: %d, count: %d\n", table_orders_existing[i].table_num, table_orders_existing[i].menu_item_name, table_orders_existing[i].order_id, table_orders_existing[i].count);
	}
	tables[table_num-1].isReserved = 1;
	//If there were no errors, return 0
	return 0;
}

int get_table_in_house(int num_people) {
	int num_seats = num_people;
	printf("num_seats: %d\n", num_seats);
	while (num_seats <= MAX_NUM_SEATS) {
		for (int i = 0; i < tables_size; i++) {
			if (tables[i].num_seats == num_seats && tables[i].isReserved == 0) {
				tables[i].isReserved = 1;
				return tables[i].table_num;
			}
		}
		num_seats++;
	}
	return -1;
}

float table_print_receipt(int table_num) {
	float total = 0;
	for (int i = 0; i < num_existing_orders; i++) {
		if (table_orders_existing[i].table_num == table_num) {
			for (int j = 0; j < MAX_MENU_ITEMS; j++) {
				if (strcmp(table_orders_existing[i].menu_item_name, menu[j].name) == 0) {
					if (table_orders_existing[i].count > 1) {
						for (int k = 0; k < table_orders_existing[i].count; k++) {
							total += menu[j].price;
						}
					}
					else {
						total += menu[j].price;
					}
					break;
				}
			}
		}
	}
	if (total == 0) {
		return 0;
	}
	return total;
}

//The localtime() function is returning the incorrect hour (5 hours faster) this is just to correct the time
int calculate_current_hour(int hour) {
	if (hour == 0) {
		return 19;
	}
	else if (hour == 1) {
		return 20;
	}
	else if (hour == 2) {
		return 21;
	}
	else if (hour == 3) {
		return 22;
	}
	else if (hour == 4) {
		return 23;
	}
	else {
		return hour - 5;
	}

}

int calculate_current_year(int year) {
	return year + 1900;
}

int calculate_current_month(int month) {
	return month+1;
}
