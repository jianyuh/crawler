WORKROOT = $(HOME)/

ifeq ($(MAC),64)
	ULIB = $(WORKROOT)lib2-64
else
	ULIB = $(WORKROOT)lib2
endif

CC = g++

DICT = -I$(ULIB)/dict/include
ULLIB = -I$(ULIB)/ullib/include
HTTP = -I$(ULIB)/http/include

LDFLAGS =  -L$(ULIB)/http/lib -lnewhttp \
	-L$(ULIB)/dict/lib -luldict \
	-L$(ULIB)/ullib/lib -lullib -lpthread -lcrypto -lm 

all: main.o http_handle.o dict.o config.o
	$(CC) main.o http_handle.o dict.o config.o $(LDFLAGS) -o crawler
main.o: main.cpp dict.h http_handle.h config.h
	$(CC) -c main.cpp $(ULLIB)
dict.o: dict.cpp dict.h
	$(CC) -c dict.cpp $(DICT) $(ULLIB)
http_handle.o: http_handle.cpp http_handle.h
	$(CC) -c http_handle.cpp $(HTTP) $(ULLIB)
config.o: config.cpp config.h
	$(CC) -c config.cpp $(ULLIB)

clean:
	rm *.o crawler
