CXXFLAGS += -g -Wall -Wextra -Werror -Wno-unused-value
DEFINES  += -DTRACE
#DEFINES  += -DTRACE -DNDEBUG

export CXXFLAGS
export DEFINES

all:
	$(MAKE) -C tests all

clean:
	$(MAKE) -C tests clean
