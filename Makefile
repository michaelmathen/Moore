CXXFLAGS =	-O3 -Wall -fmessage-length=0 -std=c++11

OBJS =		Moore.o MooreGraph.o

LIBS =

TARGET =	Moore 

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

#all:	$(TARGET)

all: MooreGraph.cpp MooreGraph.hpp Moore.cpp LocalSearch.hpp
	g++ $(CXXFLAGS) MooreGraph.cpp Moore.cpp LocalSearch.hpp -o Moore

clean:
	rm -f $(OBJS) $(TARGET)
