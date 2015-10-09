target: localtest.out

localtest.out: cgic_local.o cgic_local_test.o
	gcc cgic_local.o cgic_local_test.o -o localtest.out

cgic_local.o: cgic_local.c
	gcc -c cgic_local.c -o cgic_local.o

cgic_local_test.o: cgic_local_test.c
	gcc -c cgic_local_test.c -o cgic_local_test.o

.PHONY: clean
clean:
	rm -f *~ *.o *.out
