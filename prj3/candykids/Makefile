CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -Wall -Werror -pthread -lrt
	
EXE = candykids


%.o : %.c
	$(CC) -c $(CCFLAGS) $<
	
candykids: candykids.o bbuff.o stats.o
	$(CC) $(CCFLAGS) -o $(EXE) candykids.o bbuff.o stats.o
		
clean:
	rm -f core *.o


