#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include "server.h"
#include <sys/types.h>
#include <unistd.h>

//Resource:
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.getting_started/topic/s1_msg_Identifying_messages.html
//http://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.lib_ref/topic/m/msgsendvs.html

int main(int argc, char **argv)
{
	//reservation or online order or in house order
	//have a variable or sth
	//is taking arugments from argv the only way?
	//or can we have it to allow constant interaction
	//eg) view menu n stuff
	//variable to recognize what to do

	//assuming we are using initial command line input to get all the info at once
	//argv[0]
	//argv[1]
	//argv[2]
	//argv[3]
	//argv[4]
	uint16_t msg_type;

	//if argument length is 2, its an in-house order
	//argv[0] = menu_items
	//argv[1] = table_num
//	if (argc == 2) {
//		msg_type = CREATE_TABLE_ORDER_MSG_TYPE;
//		//build the actual msg first, may be necessary for setting size in the header
//		recv_msg_table_order_t tableOrderMsg;
//		tableOrderMsg.menu_items = argv[0];
//		tableOrderMsg.table_num = argv[1];
//		tableOrderMsg.type = CREATE_TABLE_ORDER_MSG_TYPE;
//	}
//
//	//if argument length is 5, its an online order
//	//argv[0] = menu_items
//	//argv[1] = first_name
//	//argv[2] = last_name
//	//argv[3] = address
//	//argv[4] = phone_num
//	else if (argc == 5) {
//		msg_type = CREATE_ONLINE_ORDER_MSG_TYPE;
//		//build the actual msg first, may be necessary for setting size in the header
//		recv_msg_online_order_t onlineOrderMsg;
//		onlineOrderMsg.menu_items = argv[0];
//		onlineOrderMsg.first_name = argv[1];
//		onlineOrderMsg.last_name = argv[2];
//		onlineOrderMsg.address = argv[3];
//		onlineOrderMsg.phone_num = argv[4];
//		onlineOrderMsg.type = CREATE_ONLINE_ORDER_MSG_TYPE;
//		//onlineOrderMsg.date = ?????
//		//wait for date assignment?
//	}
//
//	//if argument length is 6, its a reservation
//	//argv[0] = num_people
//	//argv[1] = first_name
//	//argv[2] = last_name
//	//argv[3] = phone_num
//	//argv[4] = start_time
//	//argv[4] = duration
//	else if (argc == 6) {
//		msg_type = CREATE_RESERVATION_MSG_TYPE;
//		//build the actual msg first, may be necessary for setting size in the header
//		recv_msg_reservation_t reservationMsg;
//		reservationMsg.num_people = argv[0];
//		reservationMsg.first_name = argv[1];
//		reservationMsg.last_name = argv[2];
//		reservationMsg.phone_num = argv[3];
//		reservationMsg.start_time = argv[4];
//		reservationMsg.duration = argv[5];
//		//reservationMsg.date = ?????
//		//wait for date assignment?
//	}
//
//	//else, its a wrong input, exit
//	else {
//		printf("Wrong number of input. Try again.\n");
//		return EXIT_FAILURE;
//	}
//
//	// open a connection to the server
//	int server_coid;
//	if ((server_coid = name_open(SERVER_NAME, 0)) == -1) {
//		printf("Error: Can not open connection to server with the given name.\n");
//		return EXIT_FAILURE;
//	}
//
//	//build the header, set IOV
//	header_t header;
//	iov_t siov[2];
//	//reservation
//	if (msg_type == CREATE_RESERVATION_MSG_TYPE) {
//		//building header
//		header.data_size = sizeof(reservationMsg);
//		header.type = CREATE_RESERVATION_MSG_TYPE;
//		//set actual data to iov
//		SETIOV(&siov[1], reservationMsg, sizeof(reservationMsg));
//	}
//	//online order
//	else if (msg_type == CREATE_ONLINE_ORDER_MSG_TYPE) {
//		//building header
//		header.data_size = sizeof(onlineOrderMsg);
//		header.type = CREATE_ONLINE_ORDER_MSG_TYPE;
//		//set actual data to iov
//		SETIOV(&siov[1], onlineOrderMsg, sizeof(onlineOrderMsg));
//	}
//	//in house order
//	else {
//		//building header
//		header.data_size = sizeof(tableOrderMsg);
//		header.type = CREATE_TABLE_ORDER_MSG_TYPE;
//		//set actual data to iov
//		SETIOV(&siov[1], tableOrderMsg, sizeof(tableOrderMsg));
//	}
//
//	//setting header after
//	SETIOV(&siov[0], &header, sizeof(header));
//
//	//send iovs and receive confirmation
//	//we will send confirmation right? decide on confirmation code
//	int confirmationCode;
//	//msgsendvs since sending iov and receiving int
//	if (-1 == MsgSendvs(server_coid, siov, 2, &confirmationCode, sizeof(confirmationCode))) {
//		printf("Error in Sending Message to Server\n");
//		return EXIT_FAILURE;
//	};
//
//	//confirmation code of 0 probably should mean
//	if (confirmationCode == 0) {
//		//reservation
//		if (msg_type == CREATE_RESERVATION_MSG_TYPE) {
//			printf("Reservation successfully made.\n");
//		}
//		//online order
//		else if (msg_type == CREATE_ONLINE_ORDER_MSG_TYPE) {
//			printf("Online order successfully made.\n");
//		}
//		//in house order
//		else {
//			printf("In house order successfully made.\n");
//		}
//	}
//	//else, error
//	else {
//		printf("Error: Server was not able to complete the request.\n");
//		return EXIT_FAILURE;
//	}

	return 0;
}
