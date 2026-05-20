todo: main.o tasks.o
	gcc -o todo main.o tasks.o

main.o: main.c tasks.h
	gcc -c main.c

tasks.o: tasks.c tasks.h
	gcc -c tasks.c

clean:
	rm -f *.o todo
