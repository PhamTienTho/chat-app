CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../common
LDFLAGS = -pthread

all: client

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client $(LDFLAGS)
	@echo "✓ Build client thành công!"

clean:
	rm -f client *.o

run: client
	./client
