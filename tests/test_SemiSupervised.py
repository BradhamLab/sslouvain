import unittest
from itertools import compress
import os

import igraph as ig
import sslouvain
import csv

from ddt import ddt, data, unpack

FILEDIR = os.path.dirname(__file__)

def get_membership_index(membership):
    values = set(membership)
    idx_lookup = {i: set() for i in range(len(values))}
    for i, value in enumerate(membership):
        idx_lookup[i] = set([i for i, x in enumerate(membership) if x == value])
    return idx_lookup

class BaseTest:
    @ddt
    class KnownPartitions(unittest.TestCase):

        def setUp(self):
            sources = [0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4,
                       5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 9]
            targets = [1, 2, 9, 0, 2, 0, 1, 4, 5, 9, 3, 5,
                       3, 4, 7, 8, 9, 6, 8, 6, 7, 0, 3, 6]
            weights = [1] * len(sources)
            g = ig.Graph(directed=True)
            g.add_vertices(10)
            g.add_edges(list(zip(sources, targets)))
            g.es['weight'] = weights
            self.graph = g
            self.expected_y = [[0, 0, 0, 1, 1, 1, 2, 2, 2, 0],
                               [0, 0, 0, 1, 1, 1, 2, 2, 2, 1],
                               [0, 0, 0, 1, 1, 1, 2, 2, 2, 2]]

        def test_all_mutable(self):
            """
            Test making all nodes mutable returns same output as ALL_NEIGH_COMMS.
            """
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
            # the last node can be in either of the 3 communities, ignore when
            # testing
            ss_idx = get_membership_index(ss_membership[:-1])
            all_neigh_idx = get_membership_index(all_neigh_membership[:-1])
            test_list = [ss_idx[i] == all_neigh_idx[i]\
                         for i in range(len(ss_idx))]
            self.assertTrue(all(test_list),
                            msg=f"Mutable membership {ss_membership}\n" 
                                f"Neighbor membership {all_neigh_membership}\n"
                                f"Expected: {test_list}")

        def test_membership_size(self):
            """Test expected membership size."""
            n_nodes = 10
            partition = self.partition_type(self.graph)
            optimiser = sslouvain.Optimiser()
            optimiser.optimise_partition(partition)
            self.assertEqual(n_nodes, len(partition.membership))

        def test_no_mutables(self):
            """
            Ensure setting all nodes as immutable returns starting membership.
            """
            n = self.graph.vcount()
            singleton_dict = {i: set([i]) for i in range(n)}
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=range(n),
                                                 mutable_nodes=[False] * n)
            member_idx = get_membership_index(partition.membership)
            print(self.partition_type, partition.membership)
            test_list = [len(x) == 1 for x in member_idx.values()]
            self.assertDictEqual(member_idx, singleton_dict)

# class LargeTest:
    @ddt
    class UnknownPartitions(unittest.TestCase):
    
        def setUp(self):
            graph = ig.read(os.path.join(FILEDIR, 'data/integrated.gml'),
                            'graphml')
            labels = [None] * graph.vcount()
            mutables = [None] * graph.vcount()
            filename = os.path.join(FILEDIR, 'data/integrated_meta.csv')
            with open(filename, 'r') as csvfile:
                csvreader = csv.reader(csvfile, delimiter=',')
                # skip header
                next(csvreader)
                for i, row in enumerate(csvreader):
                    labels[i] = int(row[1])
                    mutables[i] = row[2] == 'True'
            immutable_idxs = {}
            for x in [0, 1, 2]:
                immutable_idxs[x] = [i for i, each in enumerate(labels) if each == x]
            self.graph = graph
            self.labels = labels
            self.mutables = mutables
            self.idx_lookup = immutable_idxs
            self.maxDiff = 5

        def test_immutable_0(self):
            """
            Test nodes with label 0 set to immutable share the same label.
            """
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=self.labels,
                                                 mutable_nodes=self.mutables)
            new_labels_0 = [partition.membership[i] for i in self.idx_lookup[0]]
            expected = [new_labels_0[0]] * len(new_labels_0)
            self.assertListEqual(expected, new_labels_0)

        def test_immutable_1(self):
            """
            Test nodes with label 1 set to immutable share the same label.
            """
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=self.labels,
                                                 mutable_nodes=self.mutables)
            new_labels_1 = [partition.membership[i] for i in self.idx_lookup[1]]
            expected = [new_labels_1[0]] * len(new_labels_1)
            self.assertListEqual(expected, new_labels_1)

        def test_immutable_2(self):
            """
            Test nodes with label 2 set to immutable share the same label.
            """
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=self.labels,
                                                 mutable_nodes=self.mutables)
            new_labels_2 = [partition.membership[i] for i in self.idx_lookup[2]]
            expected = [new_labels_2[0]] * len(new_labels_2)
            self.assertListEqual(expected, new_labels_2)


class BaseModularityVertexPartitionTest(BaseTest.KnownPartitions):
  def setUp(self):
      super(BaseModularityVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.ModularityVertexPartition;

class LargeModularityVertexPartitionTest(BaseTest.UnknownPartitions):
  def setUp(self):
      super(LargeModularityVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.ModularityVertexPartition;

class BaseRBERVertexPartitionTest(BaseTest.KnownPartitions):
  def setUp(self):
      super(BaseRBERVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.RBERVertexPartition;

class LargeRBERVertexPartitionTest(BaseTest.UnknownPartitions):
  def setUp(self):
      super(LargeRBERVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.RBERVertexPartition;

# class RBConfigurationVertexPartitionTest(BaseTest.KnownPartitions):
#  def setUp(self):
#    super(RBConfigurationVertexPartitionTest, self).setUp();
#    self.partition_type = sslouvain.RBConfigurationVertexPartition;

class BaseCPMVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseCPMVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.CPMVertexPartition;

class LargeCPMVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeCPMVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.CPMVertexPartition;

class BaseSurpriseVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseSurpriseVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SurpriseVertexPartition;

class LargeSurpriseVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeSurpriseVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SurpriseVertexPartition;

class BaseSignificanceVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseSignificanceVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SignificanceVertexPartition;

class LargeSignificanceVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeSignificanceVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SignificanceVertexPartition;

#%%
if __name__ == '__main__':
  #%%
    unittest.main(verbosity=3);
    suite = unittest.TestLoader().discover('.');
    unittest.TextTestRunner(verbosity=1).run(suite);