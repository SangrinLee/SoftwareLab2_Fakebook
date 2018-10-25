fake : main.c a.c a.h
	g++ -o fake main.c -lncursesw -lmysqlclient -lpthread
