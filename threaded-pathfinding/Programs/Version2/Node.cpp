//
// Created by jack on 10/25/24.
//

#include <cmath>
#include <algorithm>
#include "Node.h"

using namespace std;

int manhattan_distance(const Node& n1, const Node& n2) {
    const int row1 = n1.coord[0];
    const int row2 = n2.coord[0];
    const int col1 = n1.coord[1];
    const int col2 = n2.coord[1];

    return abs(row1 - row2) + abs(col1 - col2);
}

void Node::find_nearest_neighbors(vector<Node>& neighbors, ofstream& outfile) {

    // Calculate distance to each other node
    vector<pair<int, Node*>> distances;
    for (auto& n : neighbors) {
        if (&n == this) { continue; }
        const int dist = manhattan_distance(*this, n);
        distances.emplace_back(dist, &n);
    }

    // Sort the distances shortest to longest
    sort(distances.begin(), distances.end());
    outfile << "\tNeighbors of Node 1 (sorted):";
    for (const auto& dist : distances) {
        outfile << " {" << dist.second->index << ", " << dist.first << "}";
    } outfile << endl;

    // Add nearest neighbor to set of neighbors for nodes on either end
    this->neighbors.insert(distances[0].second->index);
    neighbors[distances[0].second->index - 1].neighbors.insert(this->index);

    // Add 2nd nearest neighbor to set of neighbors for nodes on either end
    this->neighbors.insert(distances[1].second->index);
    neighbors[distances[1].second->index - 1].neighbors.insert(this->index);

    // Add 3rd nearest neighbor to set of neighbors for nodes on either end
    this->neighbors.insert(distances[2].second->index);
    neighbors[distances[2].second->index - 1].neighbors.insert(this->index);

    // If there are distance ties, continue taking neighbors until one is further
    int idx = 3;
    while (get<0>(distances[idx]) == get<0>(distances[idx - 1])) {
        // Add nth nearest neighbor to set of neighbors for nodes on either end
        this->neighbors.insert(distances[idx].second->index);
        neighbors[distances[idx].second->index - 1].neighbors.insert(this->index);
        idx++;
    }
}
