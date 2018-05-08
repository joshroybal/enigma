enigma: main.o enigma.o
	g++ -s -o enigma main.o enigma.o

main.o: main.cpp enigma.hpp
	g++ -O2 -c main.cpp

enigma.o: enigma.cpp enigma.hpp
	g++ -O2 -c enigma.cpp

make clean:
	rm *.o && rm enigma
