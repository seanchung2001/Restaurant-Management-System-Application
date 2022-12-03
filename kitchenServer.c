#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "server.h"
#include <string.h>

int main(int argc, char **argv) {   

    printf("Kitchen Server Running...\n");

    //kitchen will only take orders
    typedef union {
        uint16_t type;
        struct _pulse pulse;
        recv_msg_online_order_t online_order;
        recv_msg_table_order_t table_order;
    } recv_buf_t;

    //variables
    name_attach_t *attach;
	int rcvid;
	recv_buf_t client_msg;
    int kitchenReply = 0;

    //register for our name for a channel
	//register a name in the pathname space and create a channel
	if ((attach = name_attach(NULL, KITCHEN_SERVER_NAME, _NTO_CHF_DISCONNECT)) == NULL) {
		return EXIT_FAILURE;
	}

    //the kitchen server will always be receiving msgs
	while(1) {
		//code to receive msg or pulse from client
		rcvid = MsgReceive(attach->chid, &client_msg, sizeof(client_msg), NULL);
		//error handling
		if (rcvid == -1) {
			break;
		}
		//check if it was a pulse or a message
		// if it is a pulse -> check the pulse code to see if the client is gone/disconnected and print (client is gone)  else if the pulse is something else print the code and value of the pulse
		if (rcvid == 0) {
			//check pulse code
			switch (client_msg.pulse.code) {
				//client is disconnected
				case	_PULSE_CODE_DISCONNECT:
					printf("client is gone\n");
					ConnectDetach(client_msg.pulse.scoid);
					break;
				//else
				default:
					printf("unknown pulse received, code = %d\n", client_msg.pulse.code);
			}
			continue;
		}
		// if it was a message
		else {
			switch(client_msg.type) {
                case CREATE_ONLINE_ORDER_MSG_TYPE: 
                    printf("\n*****************************************\n");
                    printf("Online Order received\n");
                    printf("Order Number: %d\n", client_msg.online_order.order_num);
                    printf("Name: %s %s\n", client_msg.online_order.first_name, client_msg.online_order.last_name);
                    printf("Items:\n");
                    //iterate menu_items and printf
                    for(int i=0; i<client_msg.online_order.order_item_count; i++) {
                        printf("%s\n", client_msg.online_order.menu_items[i]);
                    }
                    //reply with a confirmation (0)
                    MsgReply(rcvid, 0, &kitchenReply, sizeof(kitchenReply));
                    break;

                case CREATE_TABLE_ORDER_MSG_TYPE:
                    printf("\n*****************************************\n");
                    printf("In-House Order received\n");
                    printf("Order Number: %d\n", client_msg.table_order.order_num);
                    printf("Table Number: %d\n", client_msg.table_order.table_num);
                    printf("Items:\n");
                    //iterate menu_items and printf
                    for(int i=0; i<client_msg.table_order.order_item_count; i++) {
                        printf("%s\n", client_msg.table_order.menu_items[i]);
                    }
                    //reply with a confirmation (0)
                    MsgReply(rcvid, 0, &kitchenReply, sizeof(kitchenReply));
                    break;

                default:
					printf("None of the message types were chosen\n");
					break;
			}
		}
	}

	//remove the name from the namespace and destroy the channel
	name_detach(attach, 0);

	return 0;
}
