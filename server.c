#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "server.h"

typedef union
{
	uint16_t type;
	struct _pulse pulse;
	recv_msg_reservation_t reservation;
	recv_msg_online_order_t online_order;
	recv_msg_table_order_t table_order;
} recv_buf_t;

//Function forward declaration
int create_reservation(int num_seats, char *meta_tag, char *date, char *fName, char *lName, char *phoneNum, int start_time, int duration);
int create_online_order(char **menu_items, char *date, char *fName, char *lName, char *address, char *phoneNum);
int create_table_order(char *menu_item);

int main(void) {

	//sean's variables
	name_attach_t *attach;
	int rcvid;
	recv_buf_t client_msg;
	//Set up a periodic timer
	struct sigevent sigevent;
	struct itimerspec itime;
	timer_t timerID;
	int coid;

	//mohammad's section

	//sean's section
	//register for our name for a channel
	attach = name_attach(NULL, SERVER_NAME, 0);
	coid = ConnectAttach(0, 0, attach->chid, _NTO_SIDE_CHANNEL, 0);
	SIGEV_PULSE_INIT(&sigevent, coid, SIGEV_PULSE_PRIO_INHERIT, PERIODIC_TIMER_PULSE_CODE, 0);
	timer_create(CLOCK_REALTIME, &sigevent, &timerID);

	itime.it_value.tv_sec = 1; //60 millisecond expiry time
	itime.it_value.tv_nsec = 0;
	itime.it_interval.tv_sec = 1;
	itime.it_interval.tv_nsec = 0;
	timer_settime(timerID, 0, &itime, NULL);

	while (1) {
		//receive message/pulse
		printf("waiting for message\n");
		rcvid = MsgReceive(attach->chid, &client_msg, sizeof(client_msg), NULL);
		if (0 == rcvid) {
			//received a pulse
			switch (client_msg.pulse.code) {
				case PERIODIC_TIMER_PULSE_CODE:
					//Check for reservations
					printf("periodic timer went off\n");

					//Free up tables
					break;
			}
		}
		else{
			// we got a message, check its type and process the msg based on its type
			switch(client_msg.type) {
				case CREATE_RESERVATION_MSG_TYPE:
					if (create_reservation(client_msg.reservation.num_people, client_msg.reservation.table_meta_tag, client_msg.reservation.date, client_msg.reservation.first_name, client_msg.reservation.last_name, client_msg.reservation.phone_num, client_msg.reservation.start_time, client_msg.reservation.duration) == 0) {
						MsgReply(rcvid, 0, RESERVATION_SUCCESS, sizeof(RESERVATION_SUCCESS));
						printf("reservation successful\n");
					}
					else {
						MsgReply(rcvid, 0, RESERVATION_FAILED, sizeof(RESERVATION_FAILED));
						printf("reservation failed\n");
					}
					break;
				case CREATE_ONLINE_ORDER_MSG_TYPE:
					if (create_online_order(client_msg.online_order.menu_items, client_msg.online_order.date, client_msg.online_order.first_name, client_msg.online_order.last_name, client_msg.online_order.address, client_msg.online_order.phone_num)) {
						//This will send the total amount of the order as a message back to the client
						MsgReply(rcvid, 0, ONLINE_ORDER_SUCCESS, sizeof(ONLINE_ORDER_SUCCESS));
						printf("online order successful\n");
					}
					else {
						MsgReply(rcvid, 0, ONLINE_ORDER_FAILED, sizeof(ONLINE_ORDER_FAILED));
						printf("online order failed\n");
					}
					break;
				case CREATE_TABLE_ORDER_MSG_TYPE:
					if (create_table_order(client_msg.table_order.menu_item) == 0) {
						//This will send the total amount of the order as a message back to the client
						MsgReply(rcvid, 0, TABLE_ORDER_SUCCESS, sizeof(TABLE_ORDER_SUCCESS));
						printf("table order successful\n");
					}
					else {
						MsgReply(rcvid, 0, TABLE_ORDER_FAILED, sizeof(TABLE_ORDER_FAILED));
						printf("table order failed\n");
					}
					break;
			}
			//Check the time and reservations to update any tables to 'isReserved'

			//Check the time and reservation duration to see if any tables should be freed

			//Check the time and update the date if it is 12AM (New day)
			//Clear appropriate database tables for new day
			//Read in data for the new day
		}
	}

return 0;
}

int create_reservation(int num_seats, char *meta_tag, char *date, char *fName, char *lName, char *phoneNum, int start_time, int duration)
{
	//Add/retrieve the following to the database, and check for errors while adding

	//If there were no errors, return 0
	return 0;
}

int create_online_order(char **menu_items, char *date, char *fName, char *lName, char *address, char *phoneNum)
{
	//Add/retrieve the following to the database and check for errors while adding

	//If there were no errors, return 0
	return 0;
}

int create_table_order(char *menu_item) {
	//Add/retrieve stuff to database and check for errors

	//If there were no errors, return 0
	return 0;
}


