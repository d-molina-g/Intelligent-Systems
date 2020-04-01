EXE = main
CFLAGS = -Wall -O2 -std=c++11
C = g++
all: obj/main.o obj/ACO.o obj/lectura.o
	$(C) $(CFLAGS) -o $(EXE) obj/main.o obj/ACO.o obj/lectura.o

obj/main.o: main.cpp glob.h
	$(C) -c -o obj/main.o main.cpp

obj/ACO.o: ACO.cpp ACO.h glob.h
	$(C) -c -o obj/ACO.o ACO.cpp

obj/lectura.o: lectura.cpp lectura.h glob.h
	$(C) -c -o obj/lectura.o lectura.cpp

clean:
	rm -fr $(EXE)
	rm -fr obj/*

run:
	./$(EXE) Entrada/att48.tsp Salida/BestRute_att48.tsp