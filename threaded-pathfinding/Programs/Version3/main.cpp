//
// Created by jack on 10/25/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <queue>
#include <map>
#include <random>
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "Node.h"

using namespace std;

bool ensure_directory(const string& path)
{
    struct stat directory_info;
    if (stat(path.c_str(), &directory_info) != 0) {
        if (errno != ENOENT) {
            perror(("Cannot access scrap directory " + path).c_str());
            return false;
        }
        if (mkdir(path.c_str(), 0777) != 0 && errno != EEXIST) {
            perror(("Cannot create scrap directory " + path).c_str());
            return false;
        }
    }

    if (stat(path.c_str(), &directory_info) != 0) {
        perror(("Cannot access scrap directory " + path).c_str());
        return false;
    }
    if (!S_ISDIR(directory_info.st_mode)) {
        cerr << "Scrap path is not a directory: " << path << endl;
        return false;
    }
    return true;
}

// Opens a file for reading with error reporting
ifstream vin_open(const string& file_path)
{
    ifstream file(file_path);
    if (!file.is_open()) {
        cout << "Error opening file: " << file_path << endl;
        exit(2);
    }
    return file;
}

vector<vector<float>> build_weight_grid(ifstream& grid_file)
{
    // Get and validate dimensions
    string cols, rows;
    int num_cols, num_rows;
    getline(grid_file, cols, ' ');
    getline(grid_file, rows);
    try
    {
        num_cols = stoi(cols);
        num_rows = stoi(rows);
    }
    catch (...)
    {
        cout << "Number of rows and columns must be integers" << endl;
        exit(3);
    }
    if (num_cols <= 0 || num_rows <= 0)
    {
        cout << "Number of rows and columns must be positive" << endl;
        exit(4);
    }

    // Build grid
    vector<vector<float>> weight_grid(num_rows, vector<float>(num_cols, 0));
    string line;
    for (int i = 0; i < num_rows; i++)
    {
        try
        {
            getline(grid_file, line);
        }
        catch (...)
        {
            cout << "Dimensions given larger than file" << endl;
            exit(5);
        }
        istringstream iss(line);
        string location;
        for (int j = 0; j < num_cols; j++)
        {
            iss >> location;
            try
            {
                weight_grid[i][j] = stof(location);
            }
            catch (...)
            {
                cout << "Grid must contain numerical values" << endl;
                exit(6);
            }
        }
    }

    if (getline(grid_file, line))
    {
        cout << "Dimensions given smaller than file" << endl;
        exit(7);
    }

    return weight_grid;
}

vector<Node> build_nodes(ifstream& node_file)
{

    string s_nodes;
    int num_nodes;
    getline(node_file, s_nodes);
    try
    {
        num_nodes = stoi(s_nodes);
    }
    catch (...)
    {
        cout << "Number of nodes must be an integer" << endl;
        exit(10);
    }
    if (num_nodes <= 0)
    {
        cout << "Number of nodes must be positive" << endl;
        exit(11);
    }

    vector<Node> nodes(num_nodes);
    for (int i = 1; i <= num_nodes; i++)
    {
        string node_row, node_col;
        getline(node_file, node_row, ' ');
        getline(node_file, node_col, ' ');
        try
        {
            const Node n(i, stoi(node_row), stoi(node_col));
            nodes[i - 1] = n;
        }
        catch (...)
        {
            cout << "Node " << i << " given invalid indices" << endl;
            exit(12);
        }
    }

    return nodes;
}

vector<int> get_child_node_path(ifstream& file)
{
    vector<int> path;

    string node_path_s;
    getline(file, node_path_s);
    istringstream iss(node_path_s);

    int node;
    while (iss >> node)
    {
        path.push_back(node);
    }

    return path;
}

float get_child_path_cost(ifstream& file)
{
    string cost_s;
    getline(file, cost_s);

    return stof(cost_s);
}

vector<Coordinate> get_child_grid_path(ifstream& file)
{
    vector<Coordinate> path;

    string line;
    while(getline(file, line))
    {
        if (line.empty()) { continue; }
        istringstream ss(line);
        int row, col;
        ss >> row >> col;
        path.push_back({row, col});
    }

    return path;
}

// Breadth-first search to find all paths between two nodes which
// are at least min_length steps and no longer than max_length steps
vector<vector<int>> find_all_paths(int start_idx, const int end_idx, const size_t min_length, const size_t max_length, const vector<Node> &nodes)
{
    vector<vector<int>> all_paths;

    // Queue of discovered but unvisited nodes, begins only with the start node
    queue<vector<int>> to_visit;
    to_visit.push({start_idx});

    while (!to_visit.empty()) {

        // Get a path from queue
        vector<int> current_path = to_visit.front();
        to_visit.pop();

        // Get the latest node in the path
        const int current_node = current_path.back();

        // If the path is long enough and the end node is reached, add the path to the list of all paths
        if (current_node == end_idx && current_path.size() >= min_length) {
            all_paths.push_back(current_path);
        }

        // If the path is not too long, add all neighbors of the current node to the queue
        if (current_path.size() < max_length) {

            // Index offset by 1
            const set<int>& current_neighbors = nodes[current_node - 1].neighbors;

            // For each neighbor
            for (const int neighbor : current_neighbors) {
                // Not already in the path
                if (find(current_path.begin(), current_path.end(), neighbor) == current_path.end()) {
                    vector<int> new_path = current_path;
                    // Add it to the path
                    new_path.push_back(neighbor);
                    // Add the new path to the queue
                    to_visit.push(new_path);
                }
            }
        }
    }

    return all_paths;
}

// Breadth-first search to find the cheapest path between two nodes
// which are overlaid on the grid of costs.
vector<Coordinate> find_cheapest_path(const int start_node_idx, const int end_node_idx, const vector<vector<float>> &weight_grid, const vector<Node> &nodes, float& cost)
{
    // Map {current, predecessor}
    map<Coordinate, Coordinate> visited;
    // Map {current, cost}
    map<Coordinate, float> accum_cost;
    // Queue of discovered but unvisited locations on the grid
    queue<Coordinate> to_visit;

    const Coordinate start_location = nodes[start_node_idx - 1].coord;
    const Coordinate end_location = nodes[end_node_idx - 1].coord;

    // Add first location with no previous
    visited[start_location] = Coordinate{-1, -1};
    accum_cost[start_location] = 0;
    to_visit.push(start_location);

    // While there are paths to check
    while(!to_visit.empty()) {
        // Get a location from the queue
        Coordinate current_location = to_visit.front();
        to_visit.pop();

        // If we've reached our goal and the weight/cost of this path
        // is the lowest possible, return this path
        if (current_location == end_location) {
            break;
        }

        // If we have further to go, try all 8-adjacent neighbors
        for (const auto& move : {Coordinate{-1, -1}, {-1, 0}, {-1, 1},
                                                        {0, -1},                {0, 1},
                                                        {1, -1}, {1, 0}, {1, 1}}) {
            Coordinate next_location = {current_location[0] + move[0], current_location[1] + move[1]};

            // Check bounds
            if (next_location[0] >= 1 && static_cast<size_t>(next_location[0]) <= weight_grid.size() &&
                next_location[1] >= 1 && static_cast<size_t>(next_location[1]) <= weight_grid[0].size()) {

                // Get weight to this new location
                const float prev_weight = accum_cost[current_location];
                const float new_weight = prev_weight + weight_grid[next_location[0] - 1][next_location[1] - 1];

                // If we haven't been here, or this new path is cheaper than the previous
                if (accum_cost.find(next_location) == accum_cost.end() || new_weight < accum_cost[next_location]) {

                    // Add this location to visited set
                    visited[next_location] = current_location;
                    accum_cost[next_location] = new_weight;
                    to_visit.push(next_location);
                }
            }
        }
    }
    // Once all directions have been searched, reconstruct path from predecessor set
    vector<Coordinate> path;
    Coordinate current_location = end_location;

    while (current_location != Coordinate{-1, -1}) {
        path.push_back(current_location);
        current_location = visited[current_location];
    }
    reverse(path.begin(), path.end());
    cost = accum_cost[end_location];

    return path;
}

// Function to be used by grandchild processes
void find_optimal_grid_path(const int start_node_idx, const int end_node_idx, const vector<vector<float> > &weight_grid, const vector<Node> &nodes, const string &scrap_path, int child_count, int gc_count)
{
    string gs_file_path = scrap_path + "/grandchild_" + to_string(child_count) + "_" + to_string(gc_count) + ".txt";
    ofstream gs_file(gs_file_path);
    if (!gs_file.is_open()) { cout << "Couldn't open " << gs_file_path << endl; exit(25); }

    float sub_cost;
    vector<Coordinate> step = find_cheapest_path(start_node_idx, end_node_idx, weight_grid, nodes, sub_cost);

    gs_file << start_node_idx << " " << end_node_idx << endl;
    gs_file << sub_cost << endl;
    for (const Coordinate& coord : step) {
        gs_file << coord[0] << " " << coord[1] << endl;
    } gs_file << endl << flush;

    gs_file.close();
}

// Function to be used by child processes
void find_optimal_path(const vector<int> &path, const vector<vector<float>> &weight_grid, const vector<Node> &nodes, const string &scrap_path, int child_count) {

    string scrap_file_path = scrap_path + "/child_" + to_string(child_count) + ".txt";
    ofstream scrap_file(scrap_file_path);
    if (!scrap_file.is_open()) { cout << "Couldn't open " << scrap_file_path << endl; exit(25); }
    for (int i : path) {
        scrap_file << i;
        if (i != path.back()) {
            scrap_file << " ";
        }
    } scrap_file << endl;

    // Find optimal path between each pair of nodes along the way
    vector<Coordinate> grid_path;

    int grandchild_count = 0;
    for (size_t i = 0; i + 1 < path.size(); i++) {
        grandchild_count++;
        pid_t g_pid = fork();

        if (g_pid == -1) { exit(50); }

        if (g_pid == 0) { //* GRANDCHILD PROCESS *//
            find_optimal_grid_path(path[i], path[i + 1], weight_grid, nodes, scrap_path, child_count, grandchild_count);
            exit(0);
        } //* END GRANDCHILD PROCESS *//


        int g_status;
        while (waitpid(-1, &g_status, 0) > 0)
        { /* wait for grandchild */ }
    }



    vector<Coordinate> shortest_grid_path;

    // Check the first grandchild file
    string gs_file_path = scrap_path + "/grandchild_" + to_string(child_count) + "_1.txt";
    ifstream gs_file(gs_file_path);
    if (!gs_file.is_open()) {
        cout << gs_file_path;
        exit(13);
    }
    // Holders for reading in the file
    string start_node, end_node, sub_cost, coord_line;
    getline(gs_file, start_node, ' ');
    getline(gs_file, end_node);
    getline(gs_file, sub_cost);
    // Initial cost
    float cost = stof(sub_cost);
    // Initial path
    while(getline(gs_file, coord_line)) {
        istringstream coord_ss(coord_line);
        string coord_row, coord_col;
        int row, col;
        while(getline(coord_ss, coord_row, ' ')) {
            getline(coord_ss, coord_col);
            try {
                row = stoi(coord_row);
                col = stoi(coord_col);
                shortest_grid_path.push_back({row, col});
                // cout << "ROW AND COL: " << row << " " << col << endl;
            } catch (...) {
                cout << "FUCK";
                cout << coord_row << " " << coord_col << endl;
            }
        }
    }

    // For each other grandchild file, check if the cost is lower than the current lowest
    for (int i = 1; i <= grandchild_count; i++) {
        gs_file_path = scrap_path + "/grandchild_" + to_string(child_count) + "_" + to_string(i) + ".txt";
        ifstream next_scrap(gs_file_path);
        if (!next_scrap.is_open()) {
            cout << "Error opening scrap file" << endl;
            exit(13);
        }
        // Get the path and the cost
        getline(gs_file, start_node, ' ');
        getline(gs_file, end_node);
        getline(gs_file, sub_cost);
        // If the new cost is lower, update the shortest path
        if (stof(sub_cost) < cost) {
            cost = stof(sub_cost);
            shortest_grid_path.clear();
            while(getline(gs_file, coord_line)) {
                istringstream coord_ss(coord_line);
                string coord_row, coord_col;
                int row, col;
                while (getline(coord_ss, coord_row, ' ')) {
                    getline(coord_ss, coord_col);
                    try {
                        getline(gs_file, coord_col);
                        row = stoi(coord_row);
                        col = stoi(coord_col);
                        cout << "ROW AND COL: " << row << " " << col << endl;

                        shortest_grid_path.push_back({row, col});
                    } catch (...) {
                        cout << coord_row << " " << coord_col << endl;
                    }
                }
            }
        }
    }



    scrap_file << cost << endl;
    cout << "COST: " << cost << endl;
    for (const auto& coord : grid_path) {
        cout << "COORD: " << coord[0] << " " << coord[1] << endl;
        scrap_file << coord[0] << " " << coord[1] << endl;
    } scrap_file << endl << flush;

    scrap_file.close();
}



int main(const int argc, char *argv[])
{
    // Check for proper use of the program
    if (argc != 7)
    {
        cout << "Program usage: ./prog04v1 <grid_file> <node_file> <start_node_index> <end_node_index> <scrap_folder> <output_file>" << endl;
        return 1;
    }
    auto scrap_path = string(argv[5]);
    if (!ensure_directory(scrap_path)) {
        return 2;
    }
    auto output_path = string(argv[6]);
    ofstream outfile(output_path);


    /////* GRID CONSTRUCTION */////
    ifstream grid_file = vin_open(argv[1]); // First arg points to the grid's file path
    vector<vector<float> > weight_grid = build_weight_grid(grid_file); // Build the grid
    grid_file.close(); // Done with grid file


    /////* NODE CREATION */////
    ifstream node_file = vin_open(argv[2]); // Second argument points to the nodes' file path
    vector<Node> nodes = build_nodes(node_file); // Build list of nodes
    node_file.close(); // Done with node file

    // Write initial node info to file
    outfile << "Initial Node List" << endl << "-----------------" << endl;
    for (const auto& n : nodes) {
        outfile << "\tNode " << n.index << ":" << endl;
        outfile << "\t\trow: " << n.coord[0] << ",  col: " << n.coord[1] << "\n";
        outfile << "\t\tConnect: " << endl;
    } outfile << endl;

    outfile << "Building 3-closest graph {node index, distance}" << endl;
    outfile << "-----------------------------------------------" << endl;

    for (auto& n1 : nodes) {
        n1.find_nearest_neighbors(nodes, outfile); // Give each node its neighbors
    } outfile << endl;

    outfile << "Node List with 3+ Connect Graph Computed" << endl;
    outfile << "----------------------------------------" << endl;

    for (auto &n : nodes) {
        outfile << "\tNode " << n.index << ":" << endl;
        outfile << "\t\trow: " << n.coord[0] << ",  col: " << n.coord[1] << "\n";
        outfile << "\t\tConnect: ";
        for (const auto &neighbor : n.neighbors) {
            outfile << neighbor << " ";
        } outfile << endl;
    } outfile << endl;


    /////* NODE INDEX CHECKING */////
    int start_index = stoi(argv[3]);
    int end_index = stoi(argv[4]);
    if (start_index < 1 || end_index < 1 ||
        static_cast<size_t>(start_index) > nodes.size() || static_cast<size_t>(end_index) > nodes.size()) {
        cout << "Invalid endpoint indices" << endl;
        return 13;
    }


    /////* CREATE PATHS BETWEEN START AND END */////
    vector<vector<int>> all_paths = find_all_paths(start_index, end_index, 3, 5, nodes);

    outfile << "List of paths from Node " << start_index << " to Node " << end_index << " :" << endl;
    outfile << "--------------------------------------" << endl;
    for (const auto& path : all_paths) {
        outfile << "\t";
        for (const auto& node : path) {
            outfile << node;
            if (node != path.back()) {
                outfile << "-";
            }
        } outfile << endl;
    }



    /////* PASSING NODE PATHS TO CHILD PROCESSES */////
    int child_count = 0;

    for (const auto& path : all_paths)
    {
        child_count++;
        pid_t pid = fork();
        if (pid == -1) { exit(50); }

        if (pid == 0)
        { //* CHILD PROCESS *//
            find_optimal_path(path, weight_grid, nodes, scrap_path, child_count);
            exit(0);
            // Create scrap file for child
            string scrap_file_path = scrap_path + "/child_" + to_string(++child_count) + ".txt";
            ofstream scrap_file(scrap_file_path);

            for (int i : path) {
                scrap_file << i;
                if (i != path.back()) {
                    scrap_file << " ";
                }
            } scrap_file << endl;

            // Find optimal path between each pair of nodes along the way
            // Each pair of nodes will be a grandchild process
            vector<Coordinate> grid_path;
            float cost = 0;
            for (size_t i = 0; i + 1 < path.size(); i++) {

                pid_t g_pid = fork();
                // If fork fails, exit
                if (g_pid == -1) { exit(51); }

                //* GRANDCHILD PROCESS *//
                if (g_pid == 0) {

                    // Create scrap file for grandchild
                    string gs_file_path = scrap_path + "/grandchild_" + to_string(child_count) + "_" + to_string(i + 1) + ".txt";
                    ofstream gs_file(gs_file_path);

                    float sub_cost;
                    // Find the optimal path between this pair of nodes
                    vector<Coordinate> step = find_cheapest_path(path[i], path[i + 1], weight_grid, nodes, sub_cost);
                    // First line of grandchild's file is the pair of nodes
                    gs_file << path[i] << " " << path[i + 1] << endl;
                    // Second line of grandchild's file is the cost of the path
                    gs_file << sub_cost << endl;
                    // Write the optimal path to the grandchild's file
                    for (const auto& coord : step) {
                        gs_file << coord[0] << " " << coord[1] << endl;
                    }
                    exit(0);
                }

                int g_status;
                while (waitpid(-1, &g_status, 0) > 0)
                { /* wait for grandchild to finish */ }

                // Collect grandchild data
                string gs_file_path = scrap_path + "/grandchild_" + to_string(child_count) + "_" + to_string(i + 1) + ".txt";
                ifstream gs_file(gs_file_path);
                if (!gs_file.is_open()) {
                    cout << "Error opening grandchild file" << endl;
                    return 14;
                }
                string start_node_s, end_node_s, sub_cost_s;
                getline(gs_file, start_node_s, ' ');
                getline(gs_file, end_node_s);
                getline(gs_file, sub_cost_s);
                // Add the cost to the total cost
                cost += stof(sub_cost_s);
                // Add the optimal path to the overall path
                vector<Coordinate> step;
                string coord_s;
                while(getline(gs_file, coord_s)) {
                    istringstream coord_ss(coord_s);
                    string row_string, col_string;
                    int row, col;
                    while(getline(coord_ss, row_string, ' ')) {
                        getline(coord_ss, col_string);
                        row = stoi(row_string);
                        col = stoi(col_string);
                        step.push_back({row, col});
                    }
                }
                // If not the last step, remove the last coordinate
                if (i + 2 < path.size()) {
                    step.pop_back();
                }
                grid_path.insert(grid_path.end(), step.begin(), step.end());


            }
            // Write the grid path in coordinates to scrap file
            scrap_file << cost << endl;
            for (const auto& coord : grid_path) {
                scrap_file << coord[0] << " " << coord[1] << endl;
            }

            scrap_file << endl << flush;
            scrap_file.close();
            exit(0);
        } //* END CHILD PROCESS *//

        int status;
        while (waitpid(-1, &status, 0) > 0)
        {/* wait for child to finish */ }
    }


    /////* READING CHILD PROCESS SCRAP FILES */////
    string scrap_file_path = scrap_path + "/child_1.txt";
    ifstream scrap_file = vin_open(scrap_file_path);

    float cost = get_child_path_cost(scrap_file);
    vector<int> node_path = get_child_node_path(scrap_file);
    vector<Coordinate> grid_path = get_child_grid_path(scrap_file);

    // For each other child file, check if the cost is lower than the current lowest
    for (int i = 2; i <= child_count; i++) {
        scrap_file_path = scrap_path + "/child_" + to_string(i) + ".txt";
        scrap_file = vin_open(scrap_file_path);
        float new_cost = get_child_path_cost(scrap_file);
        if (new_cost < cost)
        {
            node_path = get_child_node_path(scrap_file);
            grid_path = get_child_grid_path(scrap_file);
        }
    }


    /////* WRITE TO OUTPUT FILE */////
    // Print the length of the best node path found, and a list of the nodes that make up this path
    outfile << "Lowest cost path found:" << endl;
    outfile << "\t" << node_path.size() << " nodes: ";
    for (const auto& node : node_path) {
        outfile << node;
        if (node != node_path.back()) {
            outfile << " ";
        }
    } outfile << endl;

    // Print the length of the best grid path found, and a list of the coordinates of all the grid points that this path goes through
    outfile << "\t" << grid_path.size() << " grid points {row, col}: " << endl << "\t" << "\t";
    for (const auto& coord : grid_path) {
        outfile << "{" << coord[0] << ", " << coord[1] << "}";
        if (coord != grid_path.back()) {
            outfile << ", ";
        }
    } outfile << endl;

    // Print the total cost of that best path.
    outfile << "\tTotal cost: " << cost << endl;

    // Print coordinates of optimal path to standard output
    for (const auto& coord : grid_path) {
        cout << coord[0] << " " << coord[1];
        if (coord != grid_path.back()) {
            cout << endl;
        }
    }

    return 0;
}