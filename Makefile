all: main.o Spliter.o httplib.o url.o
	g++ main.o Spliter.o httplib.o url.o -lpthread -o url_fetch
main.o: main.cpp Spliter.h httplib.h url.h
	g++ -c main.cpp
Spliter.o: Spliter.h Spliter.cpp
	g++ -c Spliter.cpp
httplib.o: httplib.h httplib.cpp
	g++ -c httplib.cpp
url.o: url.h url.cpp
	g++ -c url.cpp

clean:
	rm *.o url_fetch