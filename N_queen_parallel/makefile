objects=main.o ThreadPool.o
test:$(objects)
	g++  -o test $(objects) -lpthread
main.o:main.c ThreadPool.h
	gcc -c main.c -std=c++11
ThreadPool.o:ThreadPool.h
	gcc -c ThreadPool.cpp  
clean :
	rm edit $(objects)
