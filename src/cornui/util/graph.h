#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

namespace cornui {
    // Returns whether the graph has a cycle containing the given vertex
    template<typename T>
    bool detectCycle(const std::unordered_map<T, std::vector<T>>& graph, const T& start) {
        // BFS
        std::queue<T> toCheck;
        toCheck.push(start);
        while (!toCheck.empty()) {
            T current = toCheck.front();
            toCheck.pop();
            // Skip invalid vertices
            if (!graph.contains(current)) continue;
            // Check if children is starting vertex
            for (const T& next : graph.at(current)) {
                if (next == start) return true;
                toCheck.push(next);
            }
        }

        return false;
    }

    // Reorder the vertices in graph such that vertices cannot connect to other vertices after them in the list
    // Note: It is assumed that no cycles exist
    template<typename T>
    void sortInTopoOrder(const std::unordered_map<T, std::vector<T>>& graph, std::vector<T>& list) {
        list.clear();

        // Graph with reversed edges
        std::unordered_map<T, std::vector<T>> reverseGraph;
        // Count the number of neighbors
        std::unordered_map<T, size_t> neighborCount;
        // Vertices that have no neighbors
        std::queue<T> leaves;

        // Populate them
        for (const auto& [start, neighbors] : graph) {
            neighborCount[start] = neighbors.size();
            if (neighbors.empty()) {
                leaves.push(start);
            }
            for (const T& end : neighbors) {
                reverseGraph[end].push_back(start);
            }
        }

        // Order the vertices
        while (!leaves.empty()) {
            T current = leaves.front();
            leaves.pop();
            list.push_back(current);

            // Update its reverse neighbors
            for (const T& next : reverseGraph[current]) {
                neighborCount[next]--;
                if (neighborCount[next] == 0) {
                    leaves.push(next);
                }
            }
        }
    }
}
