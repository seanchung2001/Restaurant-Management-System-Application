#include <sys/iomsg.h>
#include <qdb/qdb.h>

#ifndef SERVER_H_
#define SERVER_H_

#define SERVER_NAME "rmsa" //use this as the name to register in the pathname space to create and connect to the channel
#define KITCHEN_SERVER_NAME "kitchen"

//Will remove this once we figure out what exactly needs to be sent back to the client
#define RESERVATION_SUCCESS   "Reservation was successful!"
#define RESERVATION_FAILED    "Reservation failed, please try again"
#define ONLINE_ORDER_SUCCESS  "Online order was successful!"
#define ONLINE_ORDER_FAILED   "Online order failed, please try again"
#define TABLE_ORDER_SUCCESS   "Table order was successful!"
#define TABLE_ORDER_FAILED    "Table order failed, please try again"

//Feel free to change these MAX values as you see fit.
#define MAX_STRING_LEN         256  //General string length maximum
#define MAX_NUM_SEATS          16   //Maximum number of seats a table can have
#define MAX_TABLE_ORDER_ITEMS  16   //Maximum number of menu items a customer can order at a table
#define MAX_ONLINE_ORDER_ITEMS 16   //Maximum number of menu items a customer can order online
#define MAX_TABLE_ORDERS       300  //Maximum number of table orders
#define MAX_MENU_ITEMS         3    //Number of items on our menu (should be changed to a larger number later)
#define MAX_TABLE_RECEIPTS     400  // Maximum number of table receipts for the day
#define MAX_TABLE_TAGS         5  //Number of table tags in our database.
#define MAX_NUM_RESERVATIONS   20  //Maximum number of reservations the system can store
#define MAX_ONLINE_ORDERS      300 //Maximum number of online orders the system can store per day
#define MAX_NUM_TABLES         48
#define MAX_META_TAGS          7

#define CREATE_RESERVATION_MSG_TYPE (_IO_MAX+200)
#define CREATE_ONLINE_ORDER_MSG_TYPE (_IO_MAX+201)
#define CREATE_TABLE_ORDER_MSG_TYPE (_IO_MAX+202)
#define GET_TABLE_IN_HOUSE_MSG_TYPE (_IO_MAX+203)
#define GET_PRINT_RECEIPT_MSG_TYPE (_IO_MAX+204)
#define PERIODIC_TIMER_PULSE_CODE (_PULSE_CODE_MINAVAIL+2)

//meta-tags
#define WINDOW_SEATS          "window"
#define BAR_SEATS             "bar"
#define PARTY_SIZE_SEATS      "party"
#define COUPLE_SEATS          "couple"
#define OUTDOOR_SEATS         "outdoor"
#define PATIO_SEATS           "patio"
#define BOOTH_SEATS           "booth"

// Receive a message from client
typedef struct recv_msg_reservation {
	uint16_t type;
	struct _pulse pulse;
	int num_people;  //required
	char table_meta_tag[MAX_STRING_LEN+1];
	int year; //required
	int month; //required
	int day; //required
	char first_name[MAX_STRING_LEN+1]; //required
	char last_name[MAX_STRING_LEN+1]; //required
	long long int phone_num; //required
	int start_hour; //required
	int start_min; //required
	int end_hour; //required
	int end_min;
} recv_msg_reservation_t;

typedef struct resp_msg_reservation {
	int table_num;
	int reservationID;
}resp_msg_reservation_t;

typedef struct recv_msg_online_order {
	uint16_t type;
	struct _pulse pulse;
	char menu_items[MAX_ONLINE_ORDER_ITEMS][MAX_STRING_LEN+1];//menu should be a string, items separated by a comma only (no space)
	int year;
	int month;
	int day;
	int hour;
	int minutes;
	char first_name[MAX_STRING_LEN+1];
	char last_name[MAX_STRING_LEN+1];
	char address[MAX_STRING_LEN+1];
	long long int phone_num;
	int order_num; //just for the kitchen server
	int order_item_count; //just for the kitchen server
} recv_msg_online_order_t;

typedef struct resp_msg_online_order {
	int orderID;
	float total;
}resp_msg_online_order_t;

typedef struct recv_msg_table_order {
	uint16_t type;
	struct _pulse pulse;
	char menu_items[MAX_TABLE_ORDER_ITEMS][MAX_STRING_LEN+1];
	int table_num;
	int order_num; //just for the kitchen server
	int order_item_count; //just for the kitchen server
} recv_msg_table_order_t;

typedef struct recv_get_table_in_house {
	uint16_t type;
	struct _pulse pulse;
	int num_people;
	//we can add a string for meta tags if we want to allow that later
}recv_get_table_in_house_t;

typedef struct recv_table_print_receipt {
	uint16_t type;
	struct _pulse pulse;
	int table_num;
}recv_table_print_receipt_t;

typedef struct resp_table_print_receipt {
	int orderID;
	int table_num;
	float total;
}resp_table_print_receipt_t;

typedef struct profile {
	char restaurant_name[MAX_STRING_LEN+1];
	char address[MAX_STRING_LEN+1];
	char description[MAX_STRING_LEN+1];
	char opens[MAX_STRING_LEN+1];
	char closes[MAX_STRING_LEN+1];
	char login[MAX_STRING_LEN+1];
	char password[MAX_STRING_LEN+1];
}profile_t;

typedef struct table {
	int table_num;
	int num_seats;
	int isReserved;
}table_t;

typedef struct menu_items {
	char name[MAX_STRING_LEN+1];
	char description[MAX_STRING_LEN+1];
	char type[MAX_STRING_LEN+1];
	float price;
	int count;
}menu_item_t;

typedef struct table_order_item {
	char menu_item_name[MAX_STRING_LEN+1];
	int order_id;
	int count;
	int table_num;
	int isComplete;  //Whether the client has paid with print receipt 0 for no, 1 for yes
}table_order_item_t;

typedef struct online_order_item {
	char menu_item_name[MAX_STRING_LEN+1];
	int orderID;
	int count;
}online_order_item_t;

//for table order receipt printing
typedef struct table_order_receipt {
	int orderID;
	float total;
	int year;
	int month;
	int day;
	int table_num;
}table_order_receipt_t;

typedef struct online_order {
	int id;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	char first_name[MAX_STRING_LEN+1];
	char last_name[MAX_STRING_LEN+1];
	char address[MAX_STRING_LEN+1];
	long long phone_num;
	float total;
}online_order_t;

//For reservations
typedef struct reservations {
	int id;
	int table_num;
	int year;
	int month;
	int day;
	char first_name[MAX_STRING_LEN+1];
	char last_name[MAX_STRING_LEN+1];
	long long phone_num;
	int start_hour;
	int start_min;
	int end_hour;
	int end_min;
	int status; //0=not complete, 1=ongoing, 2=complete
}reservations_t;

typedef struct table_tag {
	int table_num;
	char meta_tag_name[MAX_STRING_LEN+1]; //window, bar, outdoors, party size, couple seats (two seats for couples), etc.
}table_tag_t;


#endif /* SERVER_H_ */
