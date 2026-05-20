todo: main.o tasks.o ui.o
	gcc -o todo main.o tasks.o ui.o -lncurses

main.o: main.c tasks.h
	gcc -c main.c

tasks.o: tasks.c tasks.h
	gcc -c tasks.c

ui.o: ui.c tasks.h
	gcc -c ui.c

clean:
	rm -f *.o todo
