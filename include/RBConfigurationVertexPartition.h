#ifndef RBCONFIGURATIONVERTEXPARTITION_H
#define RBCONFIGURATIONVERTEXPARTITION_H

#include "LinearResolutionParameterVertexPartition.h"

class RBConfigurationVertexPartition : public LinearResolutionParameterVertexPartition
{
  public:
    // ss modification
    RBConfigurationVertexPartition(Graph* graph,
                                   vector<size_t> membership,
                                   vector<bool> mutables,
                                   double resolution_parameter);
    RBConfigurationVertexPartition(Graph* graph,
                                   vector<size_t> membership,
                                   vector<bool> mutables);
    RBConfigurationVertexPartition(Graph* graph,
          vector<size_t> membership, double resolution_parameter);
    RBConfigurationVertexPartition(Graph* graph,
          vector<size_t> membership);
    RBConfigurationVertexPartition(Graph* graph,
      double resolution_parameter);
    RBConfigurationVertexPartition(Graph* graph);
    virtual ~RBConfigurationVertexPartition();
    virtual RBConfigurationVertexPartition* create(Graph* graph);
    virtual RBConfigurationVertexPartition* create(Graph* graph, vector<size_t> const& membership);

    virtual double diff_move(size_t v, size_t new_comm);
    virtual double quality(double resolution_parameter);

  protected:
  private:
};

#endif // RBCONFIGURATIONVERTEXPARTITION_H
