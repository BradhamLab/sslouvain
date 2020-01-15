#include "SemiSupervisedRBCVertexPartition.h"

// TODO: If membership is passed, mark mutables and re-label -1 to incremented 
// label 

// TODO: implement CREATE methods

/**
 * @brief 
 * Create new semi-supervised partition
 * 
 * @param graph: The igraph.Graph on which this partition is defined.
 * @param membership:The membership vector of this partition, i.e. an
 *                   community number for each node. So membership[i] = c
 *                   implies that node i is in community c. If None, it is
 *                   initialised with each node in its own community.
 * @param resolution: resolution parameter for community size
 * @param mutables: A boolean vector for each node in the graph, dictating which
 *                  nodes' labels are mutabl and which are immutable
 */
SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership,
                                                                   double resolution_parameter,
                                                                   vector<bool> const& mutables) :
  RBConfigurationVertexPartition(graph, membership, resolution_parameter) {
    this -> set_mutable(mutables);
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership,
                                                                   vector<bool> const& mutables) :
  RBConfigurationVertexPartition(graph, membership) {
    this -> set_mutable(mutables);
}

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership) :
  RBConfigurationVertexPartition(graph, membership) {
    this -> set_mutable();
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   double resolution_parameter) :
  RBConfigurationVertexPartition(graph, resolution_parameter) {
    this -> set_mutable();
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph) : 
  RBConfigurationVertexPartition(graph) {
    this -> set_mutable();
  }

SemiSupervisedRBCVertexPartition::~SemiSupervisedRBCVertexPartition()
{ }

/**
 * @brief Create new SemiSupervisedRBCVertexPartition object
 * 
 * @param graph 
 * @return SemiSupervisedRBCVertexPartition* 
 */
SemiSupervisedRBCVertexPartition*  SemiSupervisedRBCVertexPartition::create(Graph* graph) {
  return new SemiSupervisedRBCVertexPartition(graph);
}

SemiSupervisedRBCVertexPartition* SemiSupervisedRBCVertexPartition::create(Graph* graph,
                                                                           vector<size_t> const& membership) {
  return new SemiSupervisedRBCVertexPartition(graph, membership);
}

/**
 * @brief Set vertex mutability for each vertex in the graph
 * 
 * @param mutables: vector defining which nodes have mutable labels 
 */
void SemiSupervisedRBCVertexPartition::set_mutable(vector<bool> const& mutables) {
  for (size_t i = 0; i < this -> graph -> vcount(); i++) {
    this -> _mutables[i] = mutables[i];
  }
}

void SemiSupervisedRBCVertexPartition::set_mutable() {
  for (size_t i = 0; i < this -> graph -> vcount(); i++) {
    this -> _mutables[i] = true;
  }
}

double SemiSupervisedRBCVertexPartition::diff_move(size_t v, size_t new_comm) {
  RBConfigurationVertexPartition::diff_move(v, new_comm);
}

double SemiSupervisedRBCVertexPartition::quality(double resolution_parameter) {
  RBConfigurationVertexPartition::quality(resolution_parameter);
}

double SemiSupervisedRBCVertexPartition::quality() {
  RBConfigurationVertexPartition::quality(this -> resolution_parameter);
}


vector<bool> SemiSupervisedRBCVertexPartition::collapse_mutables() {
  set<size_t> immutables;
  vector<bool> collapsed(this -> nb_communities(), true);
  for (size_t i = 0; i < this -> graph -> vcount(); i++) {
    if (! (this -> mutables(i))) {
      immutables.insert(this -> membership(i));
    }
  }
  for (size_t c = 0; c < this -> nb_communities(); c++) {
    if (immutables.find(c) != immutables.end()) {
      collapsed[c] = false;
    }
  }
  return(collapsed);
}

void SemiSupervisedRBCVertexPartition::renumber_communities()
{
  vector<SemiSupervisedRBCVertexPartition*> partitions(1);
  partitions[0] = this;
  this->renumber_communities(SemiSupervisedRBCVertexPartition::renumber_communities(partitions));
}

void SemiSupervisedRBCVertexPartition::renumber_communities(vector<size_t> const& membership)
{
  this->set_membership(membership);
}


vector<size_t> SemiSupervisedRBCVertexPartition::renumber_communities(vector<SemiSupervisedRBCVertexPartition*> partitions)
{
  size_t nb_layers = partitions.size();
  size_t nb_comms = partitions[0]->nb_communities();
  size_t n = partitions[0]->graph->vcount();

  #ifdef DEBUG
    for (size_t layer; layer < nb_layers; layer++)
    {
      for (size_t v = 0; v < n; v++)
      {
        if (partitions[0]->membership(v) != partitions[layer]->membership(v))
          cerr << "Membership of all partitions are not equal";
      }
    }
  #endif
  // First sort the communities by size
  // Csizes
  // first - community
  // second - csize
  // third - number of nodes (may be aggregate nodes), to account for communities with zero weight.
  vector<size_t*> csizes;
  for (size_t i = 0; i < nb_comms; i++)
  {
      size_t csize = 0;
      for (size_t layer = 0; layer < nb_layers; layer++)
        csize += partitions[layer]->csize(i);

      size_t* row = new size_t[3];
      row[0] = i;
      row[1] = csize;
      row[2] = partitions[0]->community[i]->size();
      csizes.push_back(row);
  }
  sort(csizes.begin(), csizes.end(), orderCSize);


  // Then use the sort order to assign new communities,
  // such that the largest community gets the lowest index.
  vector<size_t> new_comm_id(nb_comms, 0);
  for (size_t i = 0; i < nb_comms; i++)
  {
    size_t comm = csizes[i][0];
    new_comm_id[comm] = i;
    delete[] csizes[i];
  }

  vector<size_t> membership(n, 0);
  for (size_t i = 0; i < n; i++)
    membership[i] = new_comm_id[partitions[0]->_membership[i]];

  return membership;
}