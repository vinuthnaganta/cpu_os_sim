CC = gcc
CFLAGS = -Wall

p3:	main.o disk.o memory.o cpu.o scheduler.o smm.o
	$(CC) $(CFLAGS) -o p3 main.o disk.o memory.o cpu.o scheduler.o smm.o
main.o:	main.c disk.h memory.h cpu.h scheduler.h smm.h
	$(CC) $(CFLAGS) -c main.c
disk.o:	disk.c disk.h smm.h
	$(CC) $(CFLAGS) -c disk.c
memory.o:	memory.c memory.h smm.h
	$(CC) $(CFLAGS) -c memory.c
cpu.o:	cpu.c cpu.h memory.h
	$(CC) $(CFLAGS) -c cpu.c
scheduler.o:	scheduler.c scheduler.h cpu.h memory.h
	$(CC) $(CFLAGS) -c scheduler.c
smm.o:	smm.c smm.h
	$(CC) $(CFLAGS) -c smm.c
clean:
	rm -f *.o p3
