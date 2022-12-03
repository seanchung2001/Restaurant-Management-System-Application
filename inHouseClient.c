#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include "server.h"
#include <sys/types.h>
#include <unistd.h>
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
	//reply of the msg will be table number, order number, or the actual receipt
	int confirmationNum;

    //start of in house client, ask user if they want to
    //1) get a table, or
    //2) order (in-house) or
    //3) get receipt
    //exit client for any other input
    printf("------In-House Client------\n");
    printf("Input:\n1 to get a table, or\n2 to make a order, or 3 to get receipt.\n");
	printf("Other inputs will lead the program to terminate.\n");
	printf("Enter your choice:\n");
    int initialChoice;
    scanf("%d", &initialChoice);

	uint16_t msg_type;
	//getting a table
    if (initialChoice == 1) {
		msg_type = GET_TABLE_IN_HOUSE_MSG_TYPE;
		//create and build msg
		recv_get_table_in_house_t getTableMsg;
		getTableMsg.type = GET_TABLE_IN_HOUSE_MSG_TYPE;
		
		printf("\nInput number of people:\n");
		int numPeople;
		scanf("%d", &numPeople);
		getTableMsg.num_people = numPeople;

		//send msg, confirmationNum will be table number
		if (-1 == MsgSend(server_coid, &getTableMsg, sizeof(getTableMsg), &confirmationNum, sizeof(confirmationNum))) {
			printf("Error in Sending Message to Server\n");
			return EXIT_FAILURE;
		};
    }
	//make an in-house order
    else if (initialChoice == 2) {
		msg_type = CREATE_TABLE_ORDER_MSG_TYPE;

		recv_msg_table_order_t inHouseOrderMsg;
		inHouseOrderMsg.type = CREATE_TABLE_ORDER_MSG_TYPE;

		printf("\nInput table number:\n");
		int tableNum;
		scanf("%d", &tableNum);
        inHouseOrderMsg.table_num = tableNum;

		printf("\nInput how many items you would like to order.\n");
		printf("Maximum number of items you can order is %d.\n", MAX_TABLE_ORDER_ITEMS);
		printf("\nEnter input:\n");
		int orderCount;
		scanf("%d", &orderCount);
        inHouseOrderMsg.order_item_count = orderCount;

		int count = 0;
		while (count < orderCount) {
			printf("\nInput the items to order, one at a time.\n");
			scanf("%s", inHouseOrderMsg.menu_items[count]);
			count ++;
		}
		strcpy(inHouseOrderMsg.menu_items[count], "done");

		//send msg, confirmationNum will be order number
		if (-1 == MsgSend(server_coid, &inHouseOrderMsg, sizeof(inHouseOrderMsg), &confirmationNum, sizeof(confirmationNum))) {
			printf("Error in Sending Message to Server\n");
			return EXIT_FAILURE;
		};
        inHouseOrderMsg.order_num = confirmationNum;

        //close off current connection to the main server
        name_close(server_coid);

        //open new connection to kitchen server
        if ((server_coid = name_open(KITCHEN_SERVER_NAME, 0)) == -1) {
            printf("Error: Can not open connection to kitchen server with the given name.\n");
            return EXIT_FAILURE;
        }

        //send msg, kitchenReply is useless
        int kitchenReply;
        if (-1 == MsgSend(server_coid, &inHouseOrderMsg, sizeof(inHouseOrderMsg), &kitchenReply, sizeof(kitchenReply))) {
            printf("Error in Sending Message to Kitchen Server\n");
            return EXIT_FAILURE;
        };
    }
    //printing receipt
    else if (initialChoice == 3) {
	msg_type = GET_PRINT_RECEIPT_MSG_TYPE;
	//create and build msg
	recv_table_print_receipt_t printReceiptMsg;
	printReceiptMsg.type = GET_PRINT_RECEIPT_MSG_TYPE;

	printf("\nInput table number:\n");
	int tableNum;
	scanf("%d", &tableNum);
	printReceiptMsg.table_num = tableNum;

	//send msg, reply will be the actual receipt
	resp_table_print_receipt_t respReceipt;
	if (-1 == MsgSend(server_coid, &printReceiptMsg, sizeof(printReceiptMsg), &respReceipt, sizeof(respReceipt))) {
		printf("Error in Sending Message to Server\n");
		return EXIT_FAILURE;
	};
	printf("-----------------------\n");
	printf("Receipt for the order: %d\n", respReceipt.orderID);
        printf("Total: %d\n", respReceipt.total);
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
		if (msg_type == GET_TABLE_IN_HOUSE_MSG_TYPE) {
			printf("Got a table.\n");
			printf("Table number: %d\n", confirmationNum);
		}
		//in house order
		else if (msg_type == CREATE_TABLE_ORDER_MSG_TYPE) {
			printf("In-house order successfully made.\n");
			printf("In-house order number: %d\n", confirmationNum);
		}
	}

	return 0;
}
