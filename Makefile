CXX ?= g++

CXXFLAGS := -c -Wall -ansi -pedantic -std=c++20

webSrv: webServer.o
	$(CXX) -o webSrv webServer.o

webServer.o: webServer.cpp
	$(CC) $(CXXFLAGS) webServer.cpp


clean:
	/bin/rm -f *~ *.o webSrv webServer.o

.PHONY: clean
