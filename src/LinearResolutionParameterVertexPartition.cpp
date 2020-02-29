#include "LinearResolutionParameterVertexPartition.h"

LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(Graph* graph,
      vector<size_t> membership, double resolution_parameter) :
        ResolutionParameterVertexPartition(graph,
        membership, resolution_parameter)
{ }

// ss modification
LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(
  Graph* graph,
  vector<size_t> membership,
  vector<bool> mutables,
  double resolution_parameter) : ResolutionParameterVertexPartition(graph,
                                                                    membership,
                                                                    mutables,
                                                                    resolution_parameter)
{ }

LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(Graph* graph,
      vector<size_t> membership) :
        ResolutionParameterVertexPartition(graph,
        membership)
{ }

// ss modification
LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(
  Graph* graph,
  vector<size_t> membership,
  vector<bool> mutables) : ResolutionParameterVertexPartition(graph,
                                                              membership,
                                                              mutables)
{ }

LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(Graph* graph,
  double resolution_parameter) :
        ResolutionParameterVertexPartition(graph, resolution_parameter)
{ }

LinearResolutionParameterVertexPartition::LinearResolutionParameterVertexPartition(Graph* graph) :
        ResolutionParameterVertexPartition(graph)
{ }

LinearResolutionParameterVertexPartition::~LinearResolutionParameterVertexPartition()
{ }
