QUIET = @
OBJS := main.o daemon.o connection.o connect_manager.o

CC = g++
LDLIBS = -lpthread -lboost_system

.PHONY: all clean

all: daemon
clean:
	@echo Cleaning...
	$(QUIET)rm daemon *.o

daemon: $(OBJS)
main.o daemon.o: daemon.hpp
connect_manager.o connection.o daemon.o: connect_manager.hpp connection.hpp
