all : main.o avl-tree.o
	$(CC) main.o avl-tree.o -o busca 

main.o : main.c 
	$(CC) -c -Wall -Wextra -g main.c

avl-tree.o : avl-tree.c avl-tree.h
	$(CC) -c -Wall -Wextra -g avl-tree.c

clean :
	rm -f *.o busca