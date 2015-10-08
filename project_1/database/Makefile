demo:database.c
	@gcc -lm -lpthread -ldl -I/usr/include/mysql database.c -o demo -L/usr/lib64/mysql -lmysqlclient
clean:
	@ls | grep -v ^makefile$$ | grep -v [.]c$$ | grep -v [.]h$$ | grep -v [.]sql$$ | xargs rm -rf
#makefile结束
