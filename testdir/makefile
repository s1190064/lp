CC = g++
CFLAGS = -DTEST -g
SOURCEDIR = ../
TESTDIR = 
PSOURCEDIR = ../
CFILES = ${SOURCEDIR}scanner.cc ${PSOURCEDIR}error.cc
TESTFILES = ${TESTDIR}driver.cc

all:	test1 test2 test3 test4 test5 test6

test1 : ${CFILES}
	${CC} ${CFLAGS} -DTEST1 -o $@ ${TESTFILES} ${CFILES}

test2 : ${CFILES}
	${CC} ${CFLAGS} -DTEST2 -o $@ ${TESTFILES} ${CFILES}

test3 : ${CFILES}
	${CC} ${CFLAGS} -DTEST3 -o $@ ${TESTFILES} ${CFILES}

test4 : ${CFILES}
	${CC} ${CFLAGS} -DTEST4 -o $@ ${TESTFILES} ${CFILES}

test5 : ${CFILES}
	${CC} ${CFLAGS} -DTEST5 -o $@ ${TESTFILES} ${CFILES}

test6 : ${CFILES}
	${CC} ${CFLAGS} -DTEST6 -o $@ ${TESTFILES} ${CFILES}

clean:
	rm -f *.o test[0-9] test[0-9][0-9] test[0-9].result test[0-9][0-9].result tmp.in

