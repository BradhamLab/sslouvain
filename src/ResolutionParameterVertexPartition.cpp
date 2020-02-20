#include "ResolutionParameterVertexPartition.h"

// ss modification
ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(
  Graph* graph,
  vector<size_t> membership,
  vector<bool> mutables,
  double resolution_parameter) :
  MutableVertexPartition(graph, membership, mutables) {
    this -> resolution_parameter = resolution_parameter;
  }

ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(Graph* graph,
      vector<size_t> membership, double resolution_parameter) :
        MutableVertexPartition(graph,
        membership)
{ this->resolution_parameter = resolution_parameter; }

ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(Graph* graph,
      vector<size_t> membership) :
        MutableVertexPartition(graph,
        membership)
{ this->resolution_parameter = 1.0; }

// ss Modification
ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(
  Graph* graph,
  vector<size_t> membership,
  vector<bool> mutables) :
    MutableVertexPartition(graph, membership, mutables) {
      this -> resolution_parameter = 1.0;
}

ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(Graph* graph,
  double resolution_parameter) :
        MutableVertexPartition(graph)
{ this->resolution_parameter = resolution_parameter;  }

ResolutionParameterVertexPartition::ResolutionParameterVertexPartition(Graph* graph) :
        MutableVertexPartition(graph)
{ this->resolution_parameter = 1.0;  }

ResolutionParameterVertexPartition::~ResolutionParameterVertexPartition()
{ }
