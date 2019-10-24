# Makefile for compiling the tests.




all: ConfigFile.o configTest.o Error.o ErrorManager.o
	g++ -o configTest ConfigFile.o configTest.o Error.o ErrorManager.o

ConfigFile.o: ConfigFile.hpp ConfigFile.cpp
	g++ -c ConfigFile.cpp -I../ErrorManagement -std=c++0x

configTest.o: configTest.cpp
	g++ -c configTest.cpp -std=c++0x

Error.o:
	g++ -c ../ErrorManagement/Error.cpp -std=c++0x

ErrorManager.o:
	g++ -c ../ErrorManagement/ErrorManager.cpp -std=c++0x

clean:
	rm -f *.o
	rm -f configTest
	rm -f ../ErrorManagement/*.o
