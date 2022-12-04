#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include "server.h"
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/netmgr.h>     // #define for ND_LOCAL_NODE is in here
#include <sys/iofunc.h>
#include <sys/dispatch.h>

int main(int argc, char **argv) {   
	// open a connection to the server
	int server_coid;
	if ((server_coid = name_open(SERVER_NAME, 0)) == -1) {
		printf("Error: Can not open connection to server with the given name.\n");
		return EXIT_FAILURE;
	}
	//reply of the msg will be reservation num or order num
	int confirmationNum;

    //start of online client, ask user if they want to
    //1) make a reservation, or
    //2) make an online order
    //exit client for any other input
    printf("------Online Client------\n");
    printf("Input:\n1 to make a reservation, or\n2 to make an online order.\n");
	printf("Other inputs will lead the program to terminate.\n");
	printf("Enter your choice:\n");
    int initialChoice;
    scanf("%d", &initialChoice);

	//get current date and time
	int hours, minutes, day, month, year;
	time_t now;
    time(&now);
	struct tm *local = localtime(&now);
	day = local->tm_mday;            // get day of month (1 to 31)
    month = local->tm_mon + 1;      // get month of year (0 to 11)
    year = local->tm_year + 1900;   // get year since 1900
	hours = local->tm_hour;         // get hours since midnight (0-23)
    minutes = local->tm_min;        // get minutes passed after the hour (0-59)

	//getting first/last names, and phone number is required for both options
	printf("\nInput your first name:\n");
	char fName[MAX_STRING_LEN];
	scanf("%s", fName);

	printf("\nInput your last name:\n");
	char lName[MAX_STRING_LEN];
	scanf("%s", lName);

	printf("\nInput phone number:\n");
	long long phoneNum;
	scanf("%lld", &phoneNum);

	uint16_t msg_type;
	//build reservation
    if (initialChoice == 1) {
		msg_type = CREATE_RESERVATION_MSG_TYPE;
		//create and build msg
		recv_msg_reservation_t reservationMsg;
		reservationMsg.type = CREATE_RESERVATION_MSG_TYPE;
		
		printf("\nInput number of people:\n");
		int numPeople;
		scanf("%d", &numPeople);
		reservationMsg.num_people = numPeople;

		reservationMsg.year = year;
		reservationMsg.month = month;
		reservationMsg.day = day;

		sprintf(reservationMsg.first_name, fName);
		sprintf(reservationMsg.last_name, lName);
		reservationMsg.phone_num = phoneNum;

		printf("\nInput starting hour of the reservation:\n");
		int startHour;
		scanf("%d", &startHour);
		reservationMsg.start_hour = startHour;

		printf("\nInput starting minute of the reservation:\n");
		int startMin;
		scanf("%d", &startMin);
		reservationMsg.start_min = startMin;
		//ending time... is this really needed?
		printf("\nInput ending hour of the reservation:\n");
		int endHour;
		scanf("%d", &endHour);
		reservationMsg.end_hour = endHour;

		printf("\nInput ending minute of the reservation:\n");
		int endMin;
		scanf("%d", &endMin);
		reservationMsg.end_min = endMin;

		//msgsendvs since sending iov and receiving int
		if (-1 == MsgSend(server_coid, &reservationMsg, sizeof(reservationMsg), &confirmationNum, sizeof(confirmationNum))) {
			printf("Error in Sending Message to Server\n");
			return EXIT_FAILURE;
		};
    }
	//build online order
    else if (initialChoice == 2) {
		msg_type = CREATE_ONLINE_ORDER_MSG_TYPE;

		recv_msg_online_order_t onlineOrderMsg;
		onlineOrderMsg.type = CREATE_ONLINE_ORDER_MSG_TYPE;

		onlineOrderMsg.year = year;
		onlineOrderMsg.month = month;
		onlineOrderMsg.day = day;
		onlineOrderMsg.hour = hours;
		onlineOrderMsg.minutes = minutes;

		sprintf(onlineOrderMsg.first_name, fName);
		sprintf(onlineOrderMsg.last_name, lName);
		onlineOrderMsg.phone_num = phoneNum;

		printf("\nInput your address:\n");
		char address[MAX_STRING_LEN];
		scanf("%s", address);
		sprintf(onlineOrderMsg.address, address);

		printf("\nInput how many items you would like to order.\n");
		printf("Maximum number of items you can order is %d.\n", MAX_ONLINE_ORDER_ITEMS);
		printf("\nEnter input:\n");
		int orderCount;
		scanf("%d", &orderCount);
		onlineOrderMsg.order_item_count = orderCount;

		int count = 0;
		while (count < orderCount) {
			printf("\nInput the items to order, one at a time.\n");
			scanf("%s", onlineOrderMsg.menu_items[count]);
			count ++;
		}
		strcpy(onlineOrderMsg.menu_items[count], "done");
		//msgsendvs since sending iov and receiving int
		if (-1 == MsgSend(server_coid, &onlineOrderMsg, sizeof(onlineOrderMsg), &confirmationNum, sizeof(confirmationNum))) {
			printf("Error in Sending Message to Server\n");
			return EXIT_FAILURE;
		};
		onlineOrderMsg.order_num = confirmationNum;

		//close off current connection to the main server
		name_close(server_coid);

		//open new connection to kitchen server
		if ((server_coid = name_open(KITCHEN_SERVER_NAME, 0)) == -1) {
			printf("Error: Can not open connection to kitchen server with the given name.\n");
			return EXIT_FAILURE;
		}

		//send msg, kitchenReply is useless
		int kitchenReply;
		if (-1 == MsgSend(server_coid, &onlineOrderMsg, sizeof(onlineOrderMsg), &kitchenReply, sizeof(kitchenReply))) {
			printf("Error in Sending Message to Kitchen Server\n");
			return EXIT_FAILURE;
		};
    }
	//else, its a wrong input, exit
	else {
		printf("Wrong input. Try again.\n");
		return EXIT_FAILURE;
	}

	//confirmation code of negative value, it means an error
	if (confirmationNum < 0) {
		printf("Error: Server was not able to complete the request.\n");
		return EXIT_FAILURE;
	}
	//else, note customer their action is complete, and give them the # associated
	else {
		if (msg_type == CREATE_RESERVATION_MSG_TYPE) {
			printf("Reservation successfully made.\n");
			printf("Reservation number: %d\n", confirmationNum);
		}
		//online order
		else if (msg_type == CREATE_ONLINE_ORDER_MSG_TYPE) {
			printf("Online order successfully made.\n");
			printf("Online order number: %d\n", confirmationNum);
		}
	}

	return 0;
}
