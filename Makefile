CXX ?= g++

CXXFLAGS := -c -Wall -ansi -pedantic -std=c++20

webServer: webServer.o
	$(CXX) -o webServer webServer.o

webServer.o: webServer.cpp
	$(CC) $(CXXFLAGS) webServer.cpp


clean:
	/bin/rm -f *~ *.o webServer webServer.o

.PHONY: clean
