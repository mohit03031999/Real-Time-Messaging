# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g -pthread -lrt

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
rideshare: io.o producer.o consumer.o broker.o main.o
	$(CXX) $(CXXFLAGS) -o rideshare $^

io.o: io.h io.cpp

broker.o : broker.h broker.cpp

producer.o: producer.h producer.cpp

consumer.o: consumer.h consumer.cpp

main.o : io.h ridesharing.h producer.h consumer.h main.cpp

clean :
	rm *.o