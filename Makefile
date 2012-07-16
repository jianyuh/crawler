all: main.o Spliter.o httplib.o
	g++ main.o Spliter.o httplib.o -o url_fetch
main.o: main.cpp Spliter.h
	g++ -c main.cpp
Spliter.o: Spliter.h Spliter.cpp
	g++ -c Spliter.cpp
httplib.o: httplib.h httplib.cpp
	g++ -c httplib.cpp

clean:
	rm *.o url_fetch