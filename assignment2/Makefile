CC=gcc
CFLAGS= -Wall -g
INCLUDES= 
LIBS= -lm

CLIENT = client.o
SERVER = server.o

client: ${CLIENT}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${CLIENT} ${LIBS}

server: ${SERVER}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${SERVER} ${LIBS}

clean: 
	-rm -f *.o core *.core