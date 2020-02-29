#include <igraph.h>
#include <stdio.h>
#include "SemiSupervisedRBCVertexPartition.h"
#include "Optimiser.h"
#include "GraphHelper.h"

int main(void)
{
    igraph_t graph;

    FILE *ifile = fopen("test_graph.gml", "r");
    if (ifile ==0) {
        return 10;
    }
    int result = igraph_read_graph_graphml(&graph, ifile, 0);
    std::cout << result << std::endl;

    Graph louvain_graph(&graph);
    std::cout << 'hi' << std::endl;
    vector<size_t> init_membership = vector<size_t>({0, 1, 2, 3, 4,
                                                     5, 6, 7, 8, 9});
    // vector<bool> mutables = vector<bool>({true, true, true, true, true,
    //                                       true, true, true, true, true});
    vector<bool> mutables = vector<bool>({false, false, false, false, false,
                                          false, false, false, false, false});
    std::cout << mutables.size() << std::endl;
    std::cout << init_membership.size() << std::endl;
    RBConfigurationVertexPartition test(&louvain_graph, init_membership,
                                        mutables);
    Optimiser opt = Optimiser();
    opt.optimise_partition(&test);
    std::cout << "Optimized membership" << std::endl;
    for (int i = 0; i < test.membership().size(); i++) {
        std::cout << test.membership(i) << ", ";
    }
    std::cout << endl;
    igraph_destroy(&graph);
    return 0;
}