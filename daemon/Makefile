QUIET = @
OBJS := main.o daemon.o connection.o

CC = g++
LDLIBS = -lpthread -lboost_system

.PHONY: all clean

all: daemon
clean:
	@echo Cleaning...
	$(QUIET)rm daemon *.o

daemon: $(OBJS)
main.o daemon.o: daemon.hpp
daemon.o connection.o: connection.hpp
