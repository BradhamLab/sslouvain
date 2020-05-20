import csv
import json
import os
import unittest
from itertools import compress
from random import shuffle

import igraph as ig
import sslouvain
from ddt import data, ddt, unpack

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
            filename = os.path.join(FILEDIR, "data/louvain_partitions.json")
            with open(filename, "r") as f:
                self.louvain_partitions = json.load(f)
            self.name = None
            self.partition_type = None

        def test_all_mutable(self):
            """
            Test making all nodes mutable returns expect louvain output.
            """
            mutables = [True] * self.graph.vcount()
            initial_membership = list(range(self.graph.vcount()))
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=initial_membership,
                                                 mutable_nodes=mutables)
            louvain_dict = get_membership_index(self.louvain_partitions[self.name])
            member_dict = get_membership_index(partition.membership)
            self.assertDictEqual(louvain_dict, member_dict)

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
            self.assertDictEqual(member_idx, singleton_dict)

        def test_empty_post_movement(self):
            weights = [11428, 220, 2, 420, 7048,
                        1618, 36, 722, 1520]
            edges = [(0, 0), (0, 1), (0, 2), (0, 1), (1, 1),
                     (1, 2), (0, 2), (1, 2), (2, 2)]
            g = ig.Graph()
            g.add_vertices(3)
            g.add_edges(edges)
            g.es['weight'] = weights

            part = sslouvain.find_partition(g,
                                            sslouvain.RBConfigurationVertexPartition,
                                            initial_membership=[0, 1, 2],
                                            mutable_nodes=[False, False, True],
                                            weights=weights)
            self.assertListEqual(part.membership, [1, 0, 0])

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
                    label = row[1]
                    if 'Perturbed' in row[1]:
                        label = 4
                    labels[i] = int(label)
                    mutables[i] = row[2] == 'True'
            immutable_idxs = {}
            for x in [1, 2, 3]:
                immutable_idxs[x] = [i for i, each in enumerate(labels) if each == x]
            self.graph = graph
            self.labels = labels
            self.mutables = mutables
            self.idx_lookup = immutable_idxs
            self.partition_type = None

        def test_immutable_0(self):
            """
            Test nodes with label 0 set to immutable share the same label.
            """
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=self.labels,
                                                 mutable_nodes=self.mutables)
            new_labels_0 = [partition.membership[i] for i in self.idx_lookup[1]]
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
            new_labels_1 = [partition.membership[i] for i in self.idx_lookup[2]]
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
            new_labels_2 = [partition.membership[i] for i in self.idx_lookup[3]]
            expected = [new_labels_2[0]] * len(new_labels_2)
            self.assertListEqual(expected, new_labels_2)

        def test_shuffled(self):
            """
            Shuffle labels and check immutability. 
            """
            combined = list(zip(self.labels, self.mutables))
            shuffle(combined)
            labels, mutables = zip(*combined)
            labels = list(labels)
            mutables = list(mutables)
            immutable_idxs = {}
            for x in [1, 2, 3]:
                immutable_idxs[x] = [i for i, each in enumerate(labels) if each == x]
            partition = sslouvain.find_partition(self.graph,
                                                 self.partition_type,
                                                 initial_membership=labels,
                                                 mutable_nodes=mutables)
            expected = {}
            actual = {}
            for x in [1, 2, 3]:
                actual[x] = [partition.membership[i] for i in immutable_idxs[x]]
                expected[x] = [actual[x][0]] * len(actual[x])
            msg = f"Label 1:\n\tExpected: {actual[1]}\n\tActual: {actual[1]}\n"\
                + f"Label 2:\n\tExpected: {actual[2]}\n\tActual: {actual[2]}\n"\
                + f"Label 3:\n\tExpected: {actual[3]}\n\tActual: {actual[3]}"
            self.assertDictEqual(expected, actual,
                                 msg=msg)

             



class BaseModularityVertexPartitionTest(BaseTest.KnownPartitions):
  def setUp(self):
      super(BaseModularityVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.ModularityVertexPartition;
      self.name = 'Modularity'

class LargeModularityVertexPartitionTest(BaseTest.UnknownPartitions):
  def setUp(self):
      super(LargeModularityVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.ModularityVertexPartition;
      

class BaseRBERVertexPartitionTest(BaseTest.KnownPartitions):
  def setUp(self):
      super(BaseRBERVertexPartitionTest, self).setUp();
      self.partition_type = sslouvain.RBERVertexPartition;
      self.name = 'RBER'

class LargeRBERVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeRBERVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.RBERVertexPartition;

class BaseRBConfigurationVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseRBConfigurationVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.RBConfigurationVertexPartition;
        self.name = 'RBConfiguration'

class LargeRBConfigurationVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeRBConfigurationVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.RBConfigurationVertexPartition;

class BaseCPMVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseCPMVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.CPMVertexPartition;
        self.name = "CPM"

class LargeCPMVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeCPMVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.CPMVertexPartition;

# class BaseSurpriseVertexPartitionTest(BaseTest.KnownPartitions):
#     def setUp(self):
#         super(BaseSurpriseVertexPartitionTest, self).setUp();
#         self.partition_type = sslouvain.SurpriseVertexPartition;
#         self.name = "Surprise"

class LargeSurpriseVertexPartitionTest(BaseTest.UnknownPartitions):
    def setUp(self):
        super(LargeSurpriseVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SurpriseVertexPartition;

class BaseSignificanceVertexPartitionTest(BaseTest.KnownPartitions):
    def setUp(self):
        super(BaseSignificanceVertexPartitionTest, self).setUp();
        self.partition_type = sslouvain.SignificanceVertexPartition;
        self.name = "Significance"

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
