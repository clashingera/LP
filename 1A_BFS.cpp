// 1A - Parallel BFS Traversal using OpenMP
#include <iostream>
#include <queue>       // For using queue data structure in BFS
#include <omp.h>       // Required to use OpenMP for parallel processing
using namespace std;

// Node structure for the binary tree
class Node
{
public:
    Node *left, *right; // Left and right child pointers
    int data;           // Data stored in the node
};

// Class to perform Breadth First Search and node insertion
class BreadthFS
{
public:
    Node *insert(Node *root, int data); // Insert node into the binary tree
    void bfs(Node *root);               // Perform parallel BFS traversal
};

// Function to insert a node into the binary tree using level-order logic
Node *BreadthFS::insert(Node *root, int data)
{
    // If tree is empty, create a new root node
    if (!root)
    {
        root = new Node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }

    // Level-order traversal using queue to find first empty position
    std::queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *current = q.front();
        q.pop();

        // If left child is empty, insert node here
        if (!current->left)
        {
            current->left = new Node;
            current->left->left = NULL;
            current->left->right = NULL;
            current->left->data = data;
            return root;
        }
        else
        {
            q.push(current->left);
        }

        // If right child is empty, insert node here
        if (!current->right)
        {
            current->right = new Node;
            current->right->left = NULL;
            current->right->right = NULL;
            current->right->data = data;
            return root;
        }
        else
        {
            q.push(current->right);
        }
    }
    return root;
}

// Function to perform parallel BFS traversal using OpenMP
void BreadthFS::bfs(Node *root)
{
    if (!root)
        return;

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int level_size = q.size(); // Number of nodes at current level

        // Parallel loop to process all nodes in current level
#pragma omp parallel for
        for (int i = 0; i < level_size; i++)
        {
            Node *current = NULL;

            // Access shared queue in critical section to avoid race conditions
#pragma omp critical
            {
                current = q.front();
                q.pop();
                cout << current->data << "\t";
            }

            // Insert children in queue in a thread-safe way
#pragma omp critical
            {
                if (current->left)
                    q.push(current->left);
                if (current->right)
                    q.push(current->right);
            }
        }
    }
}

// Main function: accepts user input to build the tree and then performs BFS
int main()
{
    BreadthFS bfs;
    Node *root = NULL;
    int data;
    char choice;

    cout << "\n\nName: Girish Raut\nRoll No.39 \t Div.B\n\n";

    // Repeatedly take input from user to insert nodes
    do
    {
        cout << "Enter data: ";
        cin >> data;
        root = bfs.insert(root, data);
        cout << "Insert another node? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    // Perform BFS traversal
    cout << "BFS Traversal:\n";
    bfs.bfs(root);

    return 0;
}
// Run Commands:
// g++ -fopenmp -o parallel_bfs 1A_BFS.cpp
// ./parallel_bfs