#ifndef CPMVERTEXPARTITION_H
#define CPMVERTEXPARTITION_H

#include <LinearResolutionParameterVertexPartition.h>

class CPMVertexPartition : public LinearResolutionParameterVertexPartition
{
  public:
    // ss modifications
    CPMVertexPartition(Graph* graph,
                       vector<size_t> membership,
                       vector<bool> mutables,
                       double resolution_paramter);
    CPMVertexPartition(Graph* graph,
                       vector<size_t> membership,
                       vector<bool> mutables);
    CPMVertexPartition(Graph* graph,
          vector<size_t> membership, double resolution_parameter);
    CPMVertexPartition(Graph* graph,
          vector<size_t> membership);
    CPMVertexPartition(Graph* graph,
      double resolution_parameter);
    CPMVertexPartition(Graph* graph);
    virtual ~CPMVertexPartition();
    virtual CPMVertexPartition* create(Graph* graph);
    virtual CPMVertexPartition* create(Graph* graph, vector<size_t> const& membership);

    virtual double diff_move(size_t v, size_t new_comm);
    virtual double quality(double resolution_parameter);

  protected:
  private:
};

#endif // CPMVERTEXPARTITION_H
