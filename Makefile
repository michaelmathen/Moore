CXXFLAGS =	-O2 -Wall -fmessage-length=0 -std=c++11

OBJS =		Moore.o MooreGraph.o

LIBS =

TARGET =	Moore 

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

#all:	$(TARGET)

all: MoorePartial.cpp MoorePartial.hpp Moore.cpp CSP.hpp
	g++ $(CXXFLAGS) MoorePartial.cpp MoorePartial.hpp Moore.cpp CSP.hpp -o Moore

clean:
	rm -f $(OBJS) $(TARGET)
