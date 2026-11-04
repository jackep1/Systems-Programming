//
// Created by jack on 10/25/24.
//

#pragma once
#include <vector>
#include <array>
#include <set>
#include <fstream>

using Coordinate = std::array<int, 2>;
using Neighbors = std::set<int>;

class Node
{
  public:
    Node() : index(0), coord{0, 0} {};
    ~Node() = default;
    Node(const int idx, const int row, const int col) : index(idx), coord{row, col} {};


    int index;
    Coordinate coord;
    Neighbors neighbors;

    void find_nearest_neighbors(std::vector<Node>& neighbors, std::ofstream& outfile);
    bool operator==(const Node& rhs) const {
        return index == rhs.index && coord[0] == rhs.coord[0] && coord[1] == rhs.coord[1];
    }
};

