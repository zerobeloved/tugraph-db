/* Copyright (c) 2022 AntGroup. All Rights Reserved. */

#include "lgraph/lgraph_olap.h"
#include "./algo.h"

using namespace lgraph_api;
using namespace lgraph_api::olap;

size_t BFSCore(Graph<Empty>& graph, size_t root_vid, ParallelVector<size_t>& parent) {
    size_t root = root_vid;
    auto active_in = graph.AllocVertexSubset();
    active_in.Add(root);
    auto active_out = graph.AllocVertexSubset();
    parent.Fill((size_t)-1);
    parent[root] = root;

    size_t num_activations = 1;
    size_t discovered_vertices = 0;
    for (int ii = 0; num_activations != 0; ii++) {
        printf("activates(%d) <= %lu\n", ii, num_activations);
        discovered_vertices += num_activations;
        active_out.Clear();
        num_activations = graph.ProcessVertexActive<size_t>(
            [&](size_t vi) {
                size_t num_activations = 0;
                for (auto& edge : graph.OutEdges(vi)) {
                    size_t dst = edge.neighbour;
                    if (parent[dst] == (size_t)-1) {
                        auto lock = graph.GuardVertexLock(dst);
                        if (parent[dst] == (size_t)-1) {
                            parent[dst] = vi;
                            num_activations += 1;
                            active_out.Add(dst);
                        }
                    }
                }
                return num_activations;
            },
            active_in);
        active_in.Swap(active_out);
    }
    return discovered_vertices;
}
