CFLAGS = -Wall -pedantic -ansi
OBJ = main.o game.o newSleep.o terminal.o random.o LinkedList.o
EXEC = treasure

ifdef DARK
    CFLAGS += -DDARKMODE
endif

treasure: main.o game.o newSleep.o terminal.o random.o LinkedList.o
	gcc $(OBJ) -o $(EXEC)

main.o: main.c
	gcc -c main.c $(CFLAGS)

game.o: game.c game.h
	gcc -c game.c $(CFLAGS)

newSleep.o: newSleep.c newSleep.h
	gcc -c newSleep.c $(CFLAGS)

terminal.o: terminal.c terminal.h
	gcc -c terminal.c $(CFLAGS)

random.o: random.c random.h
	gcc -c random.c $(CFLAGS)

LinkedList.o: LinkedList.c LinkedList.h
	gcc -c LinkedList.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)



