all: main.o Spliter.o
	g++ main.o Spliter.o -o url_fetch
main.o: main.cpp
	g++ -c main.cpp
Spliter.o: Spliter.h Spliter.cpp
	g++ -c Spliter.cpp

clean:
	rm *.o url_fetch