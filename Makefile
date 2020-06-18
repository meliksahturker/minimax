SRC=src/p2.cpp src/Util.cpp src/AvoidEnemyProblem.cpp src/Agent.cpp

INCLUDES=include/Util.h include/AvoidEnemyProblem.h include/Agent.h

CC=g++

CFLAGS=-g -Iinclude

p2: $(SRC) $(INCLUDES)
ifeq ($(NO_COLOR),1)
	$(CC) -D NO_COLORED_OUTPUT $(CFLAGS) -o p2 $(SRC)
else
	$(CC) $(CFLAGS) -o p2 $(SRC)
endif 

clean:
	rm p2
