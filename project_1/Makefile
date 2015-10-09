#Makefile

vpath %.h socket_h
vpath %.c socket_client
vpath %.c socket_public

vpath % main_func:cJSON:local_func

ObjClient = cgi_check.o cgi_getlist.o cgi_backup.o cgi_recover.o cgi_delete.o \
            public_handle.o
ObjLocal = cgic_local.o
ObjMain = cgic.o cgic_auxil.o

target:backup.cgi

backup.cgi:$(ObjClient) $(ObjLocal) $(ObjMain) cJSON.o
	gcc $(ObjClient) $(ObjLocal) $(ObjMain) cJSON.o -lm -o $@
	rm -f *.o

$(ObjClient):%.o:%.c
	gcc -c $< -o $@

$(ObjLocal):%.o:%.c
	gcc -c $< -o $@

$(ObjMain):%.o:%.c
	gcc -c $< -o $@

cJSON.o:cJSON.c 
	gcc -c $< -o $@

.PHONY:clean
clean:
	rm -f *.o *.out *cgi *~

.PHONY:test_cgic_local
test_cgic_local:test_cgic_local.o cgic_local.o
	gcc test_cgic_local.o cgic_local.o -o test_cgic_local.out

test_cgic_local.o:test_cgic_local.c
	gcc -c test_cgic_local.c -o test_cgic_local.o
