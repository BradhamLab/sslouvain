#include <igraph.h>
#include <stdio.h>
#include "SemiSupervisedRBCVertexPartition.h"
#include "CPMVertexPartition.h"
#include "ModularityVertexPartition.h"
#include "Optimiser.h"
#include "GraphHelper.h"

int main(void) {
    std::cout << "hellloooo!" << std::endl;
    igraph_t graph;

    FILE *ifile = fopen("data/integrated.gml", "r");
    // FILE *ifile = fopen("test_graph.gml", "r");
    if (ifile == 0) {
        return 10;
    }
    // std::cout << "issues" << std::endl;
    int result = igraph_read_graph_graphml(&graph, ifile, 0);
    std::cout << result << std::endl;

    igraph_vs_t vs;
    igraph_vit_t vit;
    //   vector<bool> mutables;
    //   mutables.resize(igraph_vcount(&graph));
    igraph_vs_all(&vs); // all nodes
    igraph_vit_create(&graph, vs, &vit);
    int i = 0;
    while (! IGRAPH_VIT_END(vit)) {
    int node = IGRAPH_VIT_GET(vit);
    printf(" %i, %i", i, node);
    i += 1;
    IGRAPH_VIT_NEXT(vit);
    }
    printf("\n");
    igraph_vit_destroy(&vit);
    igraph_vs_destroy(&vs);

    Graph louvain_graph(&graph);
    //   vector<size_t> init_membership = vector<size_t>({0, 1, 2, 3, 4,
    //                                                    5, 6, 7, 8, 9});
    //   // vector<bool> mutables = vector<bool>({true, true, true, true, true,
    //   //                                       true, true, true, true, true});
    //   vector<bool> mutables = vector<bool>({true, false, false, false, false,
    //                                         false, false, false, false, true});
    vector<bool> mutables = vector<bool>(400, true);
    std::cout << "size: " << mutables.size() << std::endl;
    for (unsigned int i = 0; i < 200; i++) {
        mutables[i] = false;
    }
    vector<size_t> init_membership;
    init_membership.resize(400);
    size_t mem = 4;
    for (unsigned int i = 0; i < 400; i++) {
        if (i < 66) {
            init_membership[i] = 0;
        } else if (i > 66 && i < 133) {
            init_membership[i] = 1;
        } else if (i < 200) {
            init_membership[i] = 2;
        } else {
            init_membership[i] = mem;
            mem += 1;
        }
    }
    std::cout << mutables.size() << std::endl;
    std::cout << init_membership.size() << std::endl;
    CPMVertexPartition test(&louvain_graph, init_membership, mutables);
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