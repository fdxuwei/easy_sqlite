CC = g++
LDLIBS = -lsqlite3

TEST = test

all : ${TEST}

${TEST}: test_easy_sqlite.cpp easy_sqlite.cpp
	${CC} -o $@ $^ ${LDLIBS}
	
clean:
	rm -f *.o ${TEST}