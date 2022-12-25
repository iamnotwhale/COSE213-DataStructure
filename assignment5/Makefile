CC = gcc

.c.o: 
	$(CC) -c $<

all: name5

name5: name5.o adt_dlist.o
	$(CC) -o $@ name5.o adt_dlist.o
	
clean:
	rm -f *.o
	rm -f name5
