#ifndef SEMISUPERVISEDRBCVERTEXPARTITION_H
#define SEMISUPERVISEDRBCVERTEXPARTITION_H

#include "RBConfigurationVertexPartition.h"

class SemiSupervisedRBCVertexPartition : public RBConfigurationVertexPartition
{
  public:
    inline bool mutables(size_t v) { return this -> _mutables[v];};
    inline vector<bool> const& mutables() const {return this -> _mutables;};

    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     vector<size_t> const& membership,
                                     double resolution_parameter,
                                     vector<bool> const& mutables);
    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     vector<size_t> const& membership,
                                     vector<bool> const& mutables);
    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     vector<size_t> const& membership);
    SemiSupervisedRBCVertexPartition(Graph* graph,
                                     double resolution_parameter);
    SemiSupervisedRBCVertexPartition(Graph* graph);
    virtual ~SemiSupervisedRBCVertexPartition();
    virtual SemiSupervisedRBCVertexPartition* create(Graph* graph);
    virtual SemiSupervisedRBCVertexPartition* create(Graph* graph,
                                                     vector<size_t> const& membership);
    // perhaps unnecessary if we're not modifying behavior
    virtual double diff_move(size_t v, size_t new_comm);
    virtual double quality();
    virtual double quality(double resolution_parameter);

    void set_mutable(vector<bool> const& mutables);
    void set_mutable();

    // renumber communities overwrite  -- for some reason 
    void renumber_communities();
    void renumber_communities(vector<size_t> const& new_membership);
    vector<size_t> static renumber_communities(vector<SemiSupervisedRBCVertexPartition*> partitions);

    vector<bool> collapse_mutables();
  protected:
    vector<bool> _mutables;
  private:
};

#endif // SEMISUPERVISEDRBCVERTEXPARTITION_H
