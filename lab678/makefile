GPP = g++
GPPFLAGS = -std=c++11 -g -O2
SERVNANE = server
CLIENTNAME = client

all: server.o client.o f_server.o tree.o
	$(GPP) $(GPPFLAGS) server.o f_server.o tree.o -o $(SERVNANE) -lzmq
	$(GPP) $(GPPFLAGS) client.o f_server.o -o $(CLIENTNAME) -lzmq

server.o: f_server.h tree.h
	$(GPP) $(GPPFLAGS) -c server.cpp -o server.o

client.o: f_server.h
	$(GPP) $(GPPFLAGS) -c client.cpp -o client.o

f_server.o: f_server.h
	$(GPP) $(GPPFLAGS) -c f_server.cpp -o f_server.o

tree.o: tree.h
	$(GPP) $(GPPFLAGS) -c tree.cpp -o tree.o

clean:
	rm *.o $(CLIENTNAME) $(SERVNANE)