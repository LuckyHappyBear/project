cgi_backup:cgi_backup.o public_handle.o
	gcc -o cgi_backup cgi_backup.o public_handle.o
cgi_backup.o:cgi_backup.c
	gcc -c cgi_backup.c
public_handle.o:../socket_public/public_handle.c
	gcc -c ../socket_public/public_handle.c
