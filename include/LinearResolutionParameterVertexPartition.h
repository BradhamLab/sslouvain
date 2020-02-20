#ifndef LINEARRESOLUTIONPARAMETERVERTEXPARTITION_H
#define LINEARRESOLUTIONPARAMETERVERTEXPARTITION_H

#include <ResolutionParameterVertexPartition.h>

class LinearResolutionParameterVertexPartition : public ResolutionParameterVertexPartition
{
  public:
    LinearResolutionParameterVertexPartition(Graph* graph,
                                             vector<size_t> membership,
                                             double resolution_parameter);
    // ss modification
    LinearResolutionParameterVertexPartition(Graph* graph,
                                             vector<size_t> membership,
                                             vector<bool> mutables,
                                             double resolution_parameter);
    LinearResolutionParameterVertexPartition(Graph* graph,
                                             vector<size_t> membership);
    // ss modification
    LinearResolutionParameterVertexPartition(Graph* graph,
                                             vector<size_t> membership,
                                             vector<bool> mutables);
    LinearResolutionParameterVertexPartition(Graph* graph,
                                             double resolution_parameter);
    LinearResolutionParameterVertexPartition(Graph* graph);
    virtual ~LinearResolutionParameterVertexPartition();

  private:

};

#endif // RESOLUTIONPARAMETERVERTEXPARTITION_H
