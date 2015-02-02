
CC = g++
OPTS = -o
COMPILER_OPTS = -c
SRCS = $(wildcard *.cpp)
AGENTNAME = pvz
OBJS   = $(SRCS:%.cpp=%.o)

all: clean compile

%.o: %.cpp
	@echo Making Object:	$< 	
	@$(CC) $(COMPILER_OPTS) $<

compile: $(OBJS)
	@echo Linking Libraries
	@$(CC) $(OBJS) -lm $(OPTS) $(AGENTNAME)

exe:
	@./$(AGENTNAME)

log:
	@./$(AGENTNAME) > out.log

clean:
	@echo Cleaning
	@rm -rf  $(AGENTNAME) *.o
