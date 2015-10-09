test:test_name.o public_handle.o
	gcc test_name.o public_handle.o -o test
test_name.o:
	gcc -c test_name.c
public_handle.o:
	gcc -c public_handle.c
clean:
	rm -f *.o
	rm -f test
