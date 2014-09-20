all: rfss

main.o: main.cpp main.h
	g++ -g -std=c++0x -Wall -Wshadow -Wno-write-strings -pthread main.cpp -c -o $@

rfss.o: rfss.cpp rfss.h
	g++ -g -std=c++0x -Wall -Wshadow -pthread rfss.cpp -c -o $@

server.o: server.cpp server.h
	g++ -g -std=c++0x -Wall -Wshadow -pthread server.cpp -c -o $@

client.o: client.cpp client.h
	g++ -g -std=c++0x -Wall -Wshadow -pthread client.cpp -c -o $@

socket_pool.o: socket_pool.cpp socket_pool.h
	g++ -g -std=c++0x -Wall -Wshadow -pthread socket_pool.cpp -c -o $@

rfss: main.o rfss.o server.o client.o socket_pool.o
	g++ -g -std=c++0x -Wall -Wshadow -Wno-write-strings -pthread $^ -o $@

server :
	make server.o
client :
	make client.o
socket_pool:
	make socket_pool.o
clean:
	rm *.o rfss
