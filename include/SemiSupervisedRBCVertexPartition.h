#ifndef SEMISUPERVISEDRBCVERTEXPARTITION_H
#define SEMISUPERVISEDRBCVERTEXPARTITION_H

#include "RBConfigurationVertexPartition.h"

class SemiSupervisedRBCVertexPartition : public RBConfigurationVertexPartition
{
  public:
    inline size_t mutable(size_t v) { return this -> _mutable[v]}
    inline vector<size_t> const& mutable() const {return this -> _mutable[v]}

    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     vector<size_t> const& membership,
                                     double resolution_parameter);
    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     vector<size_t> const& membership);
    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     double resolution_parameter);
    SemiSupervisedRBCVertexPartition(Graph* graph);
    virtual ~SemiSupervisedRBCVertexPartition();
    virtual SemiSupervisedRBCVertexPartition* create(Graph* graph);
    virtual SemiSupervisedRBCVertexPartition* create(Graph* graph,
                                                     vector<size_t> const& membership);

    virtual double diff_move(size_t v, size_t new_comm);
    virtual double quality(double resolution_parameter);

  protected:
    vector<size_t> _mutable;
  private:
};

#endif // SEMISUPERVISEDRBCVERTEXPARTITION_H
