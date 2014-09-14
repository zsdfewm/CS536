all: rfss

rfss.o: rfss.cpp rfss.h
	g++ -std=c++0x -Wall -Wshadow -pthread rfss.cpp -c -o $@

server.o: server.cpp server.h
	g++ -std=c++0x -Wall -Wshadow -pthread server.cpp -c -o $@

client.o: client.cpp client.h
	g++ -std=c++0x -Wall -Wshadow -pthread client.cpp -c -o $@

rfss: rfss.o server.o client.o
	g++ -std=c++0x -Wall -Wshadow -pthread $^ -o $@

server :
	make server.o
client :
	make client.o
clean:
	rm *.o rfss
