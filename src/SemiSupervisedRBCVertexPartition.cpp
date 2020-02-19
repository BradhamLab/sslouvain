#include "SemiSupervisedRBCVertexPartition.h"

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
 *                  nodes' labels are mutable and which are immutable
 */
SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership,
                                                                   double resolution_parameter,
                                                                   vector<bool> const& mutables) :
  RBConfigurationVertexPartition(graph, membership, resolution_parameter) {
    this -> _mutables = vector<bool>(graph -> vcount(), true);
    if (mutables.size() != graph -> vcount()) {
      throw Exception("Mutable vector has incorrect size.");
    }
    this -> set_mutable(mutables);
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership,
                                                                   vector<bool> const& mutables) :
  RBConfigurationVertexPartition(graph, membership) {
    this -> _mutables = vector<bool>(graph -> vcount(), true);
    if (mutables.size() != graph -> vcount()) {
      throw Exception("Mutable vector has incorrect size.");
    }
    this -> set_mutable(mutables);
}

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   vector<size_t> const& membership) :
  RBConfigurationVertexPartition(graph, membership) {
    this -> _mutables = vector<bool>(graph -> vcount(), true);
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph,
                                                                   double resolution_parameter) :
  RBConfigurationVertexPartition(graph, resolution_parameter) {
    this -> _mutables = vector<bool>(graph -> vcount(), true); 
    this -> set_mutable();
  }

SemiSupervisedRBCVertexPartition::SemiSupervisedRBCVertexPartition(Graph* graph) : 
  RBConfigurationVertexPartition(graph) {
    this -> _mutables = vector<bool>(graph -> vcount(), true);
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