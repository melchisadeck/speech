PROGRAM = speech.x
SRCS    = main.c utterance.c distance.c ticktock.c
OBJS    = main.o utterance.o distance.o ticktock.o
CFLAGS  = -s -O3
LDFLAGS = -lm

all:	${PROGRAM}

${PROGRAM}:	${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LDFLAGS}

clean:
	rm -f a.out *core *.o ${PROGRAM}

test:	${PROGRAM}
	./speech.x -al
