CXX      = g++
CXXFLAGS = -std=c++17 -Wall -O1 -pthread

all: shop_server shop_console

shop_server: server.cpp include/shop.h include/demos.h include/httplib.h
	$(CXX) $(CXXFLAGS) server.cpp -o shop_server

shop_console: console/console_main.cpp include/shop.h include/demos.h
	$(CXX) $(CXXFLAGS) console/console_main.cpp -o shop_console

run: shop_server
	./shop_server

clean:
	rm -f shop_server shop_console
