#ifndef RESOLUTIONPARAMETERVERTEXPARTITION_H
#define RESOLUTIONPARAMETERVERTEXPARTITION_H

#include <MutableVertexPartition.h>

class ResolutionParameterVertexPartition : public MutableVertexPartition
{
  public:
    // ss modification
    ResolutionParameterVertexPartition(Graph *graph,
                                       vector<size_t> membership,
                                       vector<bool> mutables,
                                       double resolution_parameter);
    ResolutionParameterVertexPartition(Graph* graph,
                                       vector<size_t> membership,
                                       double resolution_parameter);
    ResolutionParameterVertexPartition(Graph* graph,
                                       vector<size_t> membership);
    // ss modification
    ResolutionParameterVertexPartition(Graph* graph,
                                       vector<size_t> membership,
                                       vector<bool> mutables);
    ResolutionParameterVertexPartition(Graph* graph, double resolution_parameter);
    ResolutionParameterVertexPartition(Graph* graph);
    virtual ~ResolutionParameterVertexPartition();

    double resolution_parameter;

    virtual double quality()
    {
      return this->quality(this->resolution_parameter);
    };

    virtual double quality(double resolution_parameter)
    {
      throw Exception("Function not implemented. This should be implented in a derived class, since the base class does not implement a specific method.");
    };

  private:

};

#endif // RESOLUTIONPARAMETERVERTEXPARTITION_H
