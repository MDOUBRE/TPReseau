client: client.o
	gcc -o client client.o

client.o: client.c
	gcc -o client.o -c client.c -W -Wall -pedantic
	
serv: serv.o
	gcc -o serv serv.o

serv.o: serv.c
	gcc -o serv.o -c serv.c -W -Wall -pedantic
	
clean :
	rm -rf *.o client serv
