
CXXFLAGS =	-O2 -Wall -fmessage-length=0 -std=c++11

LIBS = -lgmpxx -lgmp

CC = g++

SOURCES_CPP = MoorePartial.cpp Moore.cpp Moore.cpp Permutation.cpp
SOURCES_HPP = MoorePartial.hpp Permutation.hpp 

check-syntax: 
	$(CC)  $(CXXFLAGS) -Wall -Wextra -fsyntax-only ${CHK_SOURCES}


objects: $(SOURCES_CPP)
	$(CC) $(CXXFLAGS) -c $^ 


all: $(SOURCES_HPP) objects
	$(CC) $(CXXFLAGS) *.o ${LIBS} -o Moore 

clean: 	
	rm *.o
