WARNINGS += -Wall -Wextra -Werror -Wno-unused-value
CXXFLAGS += -g -std=c++11 ${WARNINGS}
DEFINES  += -DTRACE
#DEFINES  += -DTRACE -DNDEBUG

export CXXFLAGS
export DEFINES

all:
	$(MAKE) -C tests all

clean:
	$(MAKE) -C tests clean
