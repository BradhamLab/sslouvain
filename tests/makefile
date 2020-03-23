CC=gcc
CXX=g++
CPPFLAGS=$(shell pkg-config --cflags igraph) -rdynamic -g -I./../include 
LDLIBS=$(shell pkg-config --libs igraph)
LOUVAIN= ./../src/CPMVertexPartition.cpp ./../src/GraphHelper.cpp ./../src/LinearResolutionParameterVertexPartition.cpp ./../src/ModularityVertexPartition.cpp ./../src/MutableVertexPartition.cpp ./../src/Optimiser.cpp ./../src/RBConfigurationVertexPartition.cpp ./../src/RBERVertexPartition.cpp ./../src/ResolutionParameterVertexPartition.cpp ./../src/SemiSupervisedRBCVertexPartition.cpp ./../src/SignificanceVertexPartition.cpp ./../src/SurpriseVertexPartition.cpp
SRCS= $(shell echo *.cpp)
OBJS= $(SRCS:.cpp=.out)

$(OBJS) : $(SRCS)
	$(CXX) -o $(OBJS) $(SRCS) $(LOUVAIN) $(CPPFLAGS) $(LDLIBS)