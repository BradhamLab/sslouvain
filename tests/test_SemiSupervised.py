import unittest
import igraph as ig
import sslouvain

from ddt import ddt, data, unpack

def get_membership_index(membership):
    values = set(membership)
    idx_lookup = {i: set() for i in range(len(values))}
    for i, value in enumerate(membership):
        idx_lookup[i] = set([i for i, x in enumerate(membership) if x == value])
    return idx_lookup


class BaseTest:
    @ddt
    class SemiSupervisedTest(unittest.TestCase):

        def setUp(self):
            sources = [0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4,
                       5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 9]
            targets = [1, 2, 9, 0, 2, 0, 1, 4, 5, 9, 3, 5,
                       3, 4, 7, 8, 9, 6, 8, 6, 7, 0, 3, 6]
            weights = [1] * len(sources)
            g = ig.Graph(directed=True)
            g.add_vertices(len(sources))
            g.add_edges(list(zip(sources, targets)))
            g.es['weight'] = weights
            self.graph = g
            self.expected_y = [[0, 0, 0, 1, 1, 1, 2, 2, 2, 0],
                               [0, 0, 0, 1, 1, 1, 2, 2, 2, 1],
                               [0, 0, 0, 1, 1, 1, 2, 2, 2, 2]]

        def test_all_mutable(self):
            mutables = [True] * self.graph.vcount()
            initial_membership = list(range(self.graph.vcount()))
            optimiser = sslouvain.Optimiser()
            optimiser.consider_comms = sslouvain.MUTABLE_NEIGH_COMMS
            partition = self.partition_type(self.graph,
                                            initial_membership=initial_membership,
                                            mutable_nodes=mutables)
            optimiser.optimise_partition(partition)
            ss_membership = partition.membership
            optimiser.consider_comms = sslouvain.ALL_NEIGH_COMMS
            partition = self.partition_type(self.graph,
                                            initial_membership=initial_membership,
                                            mutable_nodes=mutables)
            optimiser.optimise_partition(partition)
            all_neigh_membership = partition.membership
            ss_idx = get_membership_index(ss_membership)
            all_neigh_idx = get_membership_index(all_neigh_membership)
            test_list = [ss_idx[i] == all_neigh_idx[i]\
                         for i in range(len(ss_idx))]
            self.assertTrue(all(test_list))

        def test_membership_size(self):
            n_nodes = self.graph.vcount()
            partition = self.partition_type(self.graph)
            optimiser = sslouvain.Optimiser()
            optimiser.optimise_partition(partition)
            self.assertEqual(n_nodes, len(partition.membership))

        def test_no_mutables(self):
            n = self.graph.vcount()
            partition = self.partition_type(self.graph,
                                            initial_membership=range(n),
                                            mutable_nodes=[False] * n)
            singleton_dict = {i: set([i]) for i in range(n)}
            optimiser = sslouvain.Optimiser()
            optimiser.optimise_partition(partition)
            member_idx = get_membership_index(partition.membership)
            test_list = [len(x) == 1 for x in member_idx.values()]
            self.assertDictEqual(member_idx, singleton_dict)
            

class ModularityVertexPartitionTest(BaseTest.SemiSupervisedTest):
 def setUp(self):
   super(ModularityVertexPartitionTest, self).setUp();
   self.partition_type = sslouvain.ModularityVertexPartition;

class RBERVertexPartitionTest(BaseTest.SemiSupervisedTest):
 def setUp(self):
   super(RBERVertexPartitionTest, self).setUp();
   self.partition_type = sslouvain.RBERVertexPartition;

class RBConfigurationVertexPartitionTest(BaseTest.SemiSupervisedTest):
 def setUp(self):
   super(RBConfigurationVertexPartitionTest, self).setUp();
   self.partition_type = sslouvain.RBConfigurationVertexPartition;

class CPMVertexPartitionTest(BaseTest.SemiSupervisedTest):
 def setUp(self):
   super(CPMVertexPartitionTest, self).setUp();
   self.partition_type = sslouvain.CPMVertexPartition;

# class SurpriseVertexPartitionTest(BaseTest.SemiSupervisedTest):
#   def setUp(self):
#     super(SurpriseVertexPartitionTest, self).setUp();
#     self.partition_type = sslouvain.SurpriseVertexPartition;

class SignificanceVertexPartitionTest(BaseTest.SemiSupervisedTest):
  def setUp(self):
    super(SignificanceVertexPartitionTest, self).setUp();
    self.partition_type = sslouvain.SignificanceVertexPartition;

#%%
if __name__ == '__main__':
  #%%
  unittest.main(verbosity=3);
  suite = unittest.TestLoader().discover('.');
  unittest.TextTestRunner(verbosity=1).run(suite);