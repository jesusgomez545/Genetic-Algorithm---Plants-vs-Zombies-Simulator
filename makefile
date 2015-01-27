
CC = g++
OPTS = -o
SRCS = $(wildcard *.cpp)
AGENTNAME = pvz
OBJS   = $(SRCS:%.cpp=%.o)

all: clean compile

%.o: %.cpp
	@echo Making Object:	$< 
	@$(CC) -c $<

compile: $(OBJS)
	@echo Linking Libraries
	@$(CC) $(OBJS) -lm $(OPTS) $(AGENTNAME)

exe:
	@./$(AGENTNAME)

clean:
	@echo Cleaning
	@rm -rf  $(AGENTNAME) *.o
