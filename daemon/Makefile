QUIET = @
OBJS := main.o daemon.o connection.o connect_manager.o stats.o
DEPS := ${OBJS:.o=.d}

CC = g++
CXXFLAGS = -MMD -Wall
LDLIBS = -lpthread -lboost_system

.PHONY: all clean

all: daemon
clean:
	@echo Cleaning...
	$(QUIET)rm -f daemon *.o *.d

-include ${DEPS}
daemon: $(OBJS)
