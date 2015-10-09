listen_and_handle:listen_and_handle.o public_handle.o database.o
	gcc listen_and_handle.o public_handle.o database.o -o listen_and_handle -I /usr/local/mysql/include/ -L /usr/local/mysql/lib/ -l mysqlclient
listen_and_handle.o:listen_and_handle.c
	gcc -c listen_and_handle.c
public_handle.o:../socket_public/public_handle.c
	gcc -c ../socket_public/public_handle.c
database.o:../database/database.c
	gcc -c ../database/database.c
clean:
	rm -f *~
	rm -f *.o
	rm -f listen_and_handle
