PROGRAM = speech.x
SRCS    = nspeech.c utterance.c distance.c ticktock.c
OBJS    = nspeech.o utterance.o distance.o ticktock.o
CFLAGS  = -s -O
LDFLAGS = -lm

all:	${PROGRAM}

${PROGRAM}:	${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LDFLAGS}

clean:
	rm -f a.out *core *.o ${PROGRAM}

test:	${PROGRAM}
	./speech.x -al
