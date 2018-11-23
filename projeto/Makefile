COMPILER_TYPE= gnu

CC= gcc

PROG= towerdefense
SRCS= towerdefense.c keyboard.c timer.c mouse.c program.c vb.c lowProgram.c bitmap.c caminhos.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>