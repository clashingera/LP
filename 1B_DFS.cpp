//1B
#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;        // Maximum number of nodes supported
vector<int> graph[MAX];        // Adjacency list for the graph
bool visited[MAX];             // Visited array to mark visited nodes
omp_lock_t lock[MAX];          // Locks to avoid race conditions on each node

// Function to perform Depth First Search starting from a given node
void dfs(int start_node)
{
    stack<int> s;              // Stack for DFS traversal
    s.push(start_node);        // Push starting node onto stack

    while (!s.empty())
    {
        int curr_node = s.top(); // Get the current node on top of stack
        s.pop();

        // Lock the current node to avoid race condition on visited[]
        omp_set_lock(&lock[curr_node]);
        if (!visited[curr_node])
        {
            visited[curr_node] = true;       // Mark node as visited
            cout << curr_node << " ";        // Print visited node
        }
        omp_unset_lock(&lock[curr_node]);

        // Explore neighbors of the current node in parallel
#pragma omp parallel for shared(s)
        for (int i = 0; i < graph[curr_node].size(); i++)
        {
            int adj_node = graph[curr_node][i]; // Get adjacent node

            // Lock adjacent node before checking its visited status
            omp_set_lock(&lock[adj_node]);
            if (!visited[adj_node])
            {
                // Push unvisited adjacent node to stack (inside critical section to avoid data race)
#pragma omp critical
                {
                    s.push(adj_node);
                }
            }
            omp_unset_lock(&lock[adj_node]);
        }
    }
}

int main()
{
    std::cout << "Name: Girish Raut\tRoll No: 39\tDiv: B\n\n";

    int n, m, start_node;

    // Take number of nodes, edges, and starting node for DFS
    cout << "Enter number of nodes, edges, and the starting node: ";
    cin >> n >> m >> start_node;

    // Input edge list
    cout << "Enter pairs of connected edges (u v):\n";
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        // Construct an undirected graph
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Initialize visited array and OpenMP locks for each node
#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
        omp_init_lock(&lock[i]); // Initialize a lock for each node
    }

    // Start DFS traversal
    cout << "\nDFS Traversal Order:\n";
    dfs(start_node);
    cout << endl;

    // Clean up: destroy all locks after use
    for (int i = 0; i < n; i++)
    {
        omp_destroy_lock(&lock[i]);
    }

    return 0;
}


// Run Commands:
// g++ -fopenmp -o 1B_DFS.cpp
// g++ -fopenmp  1B_DFS.cpp -o parallel_dfs
// ./parallel_dfs

// Output Example:
// Enter number of nodes, edges, and the starting node: 6 5 0
// Enter pairs of connected edges (u v):
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5

// DFS Traversal Order:
// 0 1 4 3 2 5