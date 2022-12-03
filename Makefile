
#
#	Makefile for hello project
#

DEBUG = -g
CC = qcc
LD = qcc
LIBS = -lqdb


TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntox86
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le


CFLAGS += $(DEBUG) $(TARGET) $(LIBS) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)
BINS = onlineClient server kitchenServer inHouseClient
all: $(BINS)

clean:
	rm -f *.o $(BINS);
#	cd solutions; make clean


server.o: server.c server.h
onlineClient.o: onlineClient.c server.h
inHouseClient.o: inHouseClient.c server.h
kitchenServer.o: kitchenServer.c server.h
