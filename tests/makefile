CC=gcc
CXX=g++
CPPFLAGS=$(shell pkg-config --cflags igraph) -I./../include
LDLIBS=$(shell pkg-config --libs igraph)
LOUVAIN=$(shell echo ./../src/*.cpp)

SRCS= $(shell echo *.cpp)
OBJS= $(SRCS:.cpp=.out)

$(OBJS) : $(SRCS)
	$(CXX) -o $(OBJS) $(SRCS) $(LOUVAIN) $(CPPFLAGS) $(LDLIBS)