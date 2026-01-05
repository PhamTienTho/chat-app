CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../common -I../database
MYSQL_FLAGS = $(shell mysql_config --cflags --libs)
LDFLAGS = -pthread $(MYSQL_FLAGS)

all: server

server: server.cpp ../database/db_manager.cpp
	$(CXX) $(CXXFLAGS) server.cpp ../database/db_manager.cpp -o server $(LDFLAGS)
	@echo "✓ Build server thành công!"

clean:
	rm -f server *.o

run: server
	./server
