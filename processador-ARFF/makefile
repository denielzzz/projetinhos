all : arff.o log.o main.o
	$(CC) -o main arff.o log.o main.o

arff.o : arff.c arff.h
	$(CC) -c -Wall -Wextra -g arff.c

log.o : log.c log.h
	$(CC) -c -Wall -Wextra -g log.c

main.o : main.c arff.h
	$(CC) -c -Wall -Wextra -g main.c

clean :
	rm -f *.o main *.txt *.bl
