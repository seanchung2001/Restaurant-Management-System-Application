#ifndef _IOV_SERVER_H_
#define _IOV_SERVER_H_

#include <sys/iomsg.h>

#define SERVER_NAME "rmsa" //use this as the name to register in the pathname space to create and connect to the channel

#define RESERVATION_SUCCESS   "Reservation was successful!"
#define RESERVATION_FAILED    "Reservation failed, please try again"
#define ONLINE_ORDER_SUCCESS  "Online order was successful!"
#define ONLINE_ORDER_FAILED   "Online order failed, please try again"
#define TABLE_ORDER_SUCCESS   "Table order was successful!"
#define TABLE_ORDER_FAILED    "Table order failed, please try again"

#define MAX_STRING_LEN    256

#define CREATE_RESERVATION_MSG_TYPE (_IO_MAX+200)
#define CREATE_ONLINE_ORDER_MSG_TYPE (_IO_MAX+201)
#define CREATE_TABLE_ORDER_MSG_TYPE (_IO_MAX+202)
#define PERIODIC_TIMER_PULSE_CODE (_PULSE_CODE_MINAVAIL+2)

// Receive a message from client
typedef struct recv_msg_reservation {
	uint16_t type;
	struct _pulse pulse;
	uint16_t table_num;
	uint16_t num_people;  //required
	char table_meta_tag[MAX_STRING_LEN+1];
	char date[MAX_STRING_LEN+1]; //required
	char first_name[MAX_STRING_LEN+1]; //required
	char last_name[MAX_STRING_LEN+1]; //required
	char phone_num[MAX_STRING_LEN+1]; //required
	uint16_t start_time; //required
	uint16_t duration; //required
} recv_msg_reservation_t;

typedef struct recv_msg_online_order {
	uint16_t type;
	struct _pulse pulse;
	char *menu_items[MAX_STRING_LEN+1];
	char date[MAX_STRING_LEN+1];
	char first_name[MAX_STRING_LEN+1];
	char last_name[MAX_STRING_LEN+1];
	char address[MAX_STRING_LEN+1];
	char phone_num[MAX_STRING_LEN+1];
} recv_msg_online_order_t;

typedef struct recv_msg_table_order {
	uint16_t type;
	struct _pulse pulse;
	char menu_item[MAX_STRING_LEN+1];
} recv_msg_table_order_t;

#endif //_IOV_SERVER_H_
