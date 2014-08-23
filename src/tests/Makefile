WARNINGS ?= -Wall -Wextra -Werror -Wno-unused-value
TEST_BIN  = ../../avltest
INCLUDES  = -I../ -I./
CXXFLAGS ?= -g -std=c++11 ${WARNINGS}
LDFLAGS  += -lm

all:
	g++ AvlTests.cpp main.cpp ${INCLUDES} ${CXXFLAGS} ${LDFLAGS} ${DEFINES} -o ${TEST_BIN}

clean:
	rm -f ${TEST_BIN}
