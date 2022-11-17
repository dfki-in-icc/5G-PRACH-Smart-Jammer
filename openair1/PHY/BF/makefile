compi1 = g++
compi2 = gcc

all: angle.o libAN.so

########## angle #############
angle.o: angle.cpp
	$(compi1) -c -std=c++14 -fPIC angle.cpp

libAN.so: angle.o
	$(compi2) -shared -o libAN.so angle.o

##############################

clean:
	rm *.o *.so


