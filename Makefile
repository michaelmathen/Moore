CXXFLAGS =	-O2 -Wall -fmessage-length=0 -std=c++11

OBJS =		Moore.o MooreGraph.o

LIBS =

TARGET =	Moore 

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

#all:	$(TARGET)

PHONY: check-syntax

check-syntax: 
	g++  $(CXXFLAGS) -Wall -Wextra -fsyntax-only ${CHK_SOURCES}


all: MoorePartial.cpp MoorePartial.hpp Moore.cpp CSP.hpp Moore.cpp
	g++ $(CXXFLAGS) MoorePartial.cpp  Moore.cpp -o Moore

clean:
	rm -f $(OBJS) $(TARGET)
