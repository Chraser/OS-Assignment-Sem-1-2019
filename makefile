CC = gcc
CFLAGS = -Wall -std=c99
OBJ = scheduler.o linked_list.o
EXEC = scheduler

$(EXEC) : clean $(OBJ) 
	$(CC) $(OBJ) -o $(EXEC) -lpthread

scheduler.o : scheduler.c scheduler.h linked_list.h task_info.h read_info.h
	$(CC) -c scheduler.c $(CFLAGS)

linked_list.o : linked_list.c linked_list.h
	$(CC) -c linked_list.c $(CFLAGS)

clean :
	rm -f $(EXEC) $(OBJ)
