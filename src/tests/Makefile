TEST_BIN  = ../../avltest
INCLUDES  = -I../ -I./
CXXFLAGS += -Wall -Wextra
LDFLAGS  += -lm

all:
	g++ AvlTest.cpp main.cpp ${INCLUDES} ${CXXFLAGS} ${LDFLAGS} ${DEFINES} -o ${TEST_BIN}

clean:
	rm -f ${TEST_BIN}
