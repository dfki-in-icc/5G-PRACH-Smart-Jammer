compi1 = g++
compi2 = gcc

all: cu_function.o libCU.so cp

angle: angle.o libAN.so cp1
	@echo angle ok

########## cu_function ##########
cu_function.o: cu_function.cpp 
	$(compi1) -c -std=c++14 -fPIC cu_function.cpp

libCU.so: cu_function.o
	$(compi2) -shared -o libCU.so cu_function.o
cp:
	sudo cp libCU.so /usr/lib
##############################


########## angle #############
angle.o: angle.cpp
	$(compi1) -c -std=c++14 -fPIC angle.cpp

libAN.so: angle.o
	$(compi2) -shared -o libAN.so angle.o

cp1:
	sudo cp libAN.so  /usr/lib

##############################

clean:
	rm *.o *.so


