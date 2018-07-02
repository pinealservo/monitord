QUIET = @
OBJS := client.o
DEPS := ${OBJS:.o=.d}

CC = g++
CXXFLAGS = -MMD -Wall
LDLIBS = -lpthread -lboost_system

.PHONY: all clean

all: client
clean:
	@echo Cleaning...
	$(QUIET)rm -f client *.o *.d

-include ${DEPS}
client: $(OBJS)
