// A C++ program to check if a given directed graph is Eulerian and generate random graphs
#include <iostream>
#include <list>
#include <cstdlib>  // For random number generation
#include <ctime>    // For seeding random number generator
#include <getopt.h> // For command-line argument processing
#include <unistd.h> // For getopt function

using namespace std;

// A class that represents a directed graph
class Graph
{
    int V;           // No. of vertices
    list<int> *adj;  // A dynamic array of adjacency lists
    int *in;         // Array to store in-degree of vertices
public:
    // Constructor and destructor
    Graph(int V);
    ~Graph() { delete [] adj; delete [] in; }

    // Function to add an edge to graph
    void addEdge(int v, int w) { adj[v].push_back(w); (in[w])++; }

    // Method to check if this graph is Eulerian or not
    bool isEulerianCycle();

    // Method to check if all non-zero degree vertices are connected
    bool isSC();

    // Function to do DFS starting from v. Used in isConnected()
    void DFSUtil(int v, bool visited[]);

    // Function to get the transpose of the graph
    Graph getTranspose();

    // Function to print the graph
    void printGraph();

    // Static method to generate a random graph
    static Graph generateRandomGraph(int numVertices, int numEdges, unsigned int seed);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
    in = new int[V];
    for (int i = 0; i < V; i++)
        in[i] = 0;
}

/* This function returns true if the directed graph has a eulerian
   cycle, otherwise returns false */
bool Graph::isEulerianCycle()
{
    // Check if all non-zero degree vertices are connected
    if (isSC() == false)
        return false;

    // Check if in-degree and out-degree of every vertex is same
    for (int i = 0; i < V; i++)
        if (adj[i].size() != in[i])
            return false;

    return true;
}

// A recursive function to do DFS starting from v
void Graph::DFSUtil(int v, bool visited[])
{
    // Mark the current node as visited
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFSUtil(*i, visited);
}

// Function that returns reverse (or transpose) of this graph
Graph Graph::getTranspose()
{
    Graph g(V);
    for (int v = 0; v < V; v++)
    {
        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            g.adj[*i].push_back(v);
            (g.in[v])++;
        }
    }
    return g;
}

// This function returns true if all non-zero degree vertices of
// graph are strongly connected
bool Graph::isSC()
{
    // Mark all the vertices as not visited (For first DFS)
    bool visited[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Find the first vertex with non-zero degree
    int n;
    for (n = 0; n < V; n++)
        if (adj[n].size() > 0)
            break;

    // If there are no edges, return true trivially
    if (n == V)
        return true;

    // Do DFS traversal starting from first non-zero degree vertex
    DFSUtil(n, visited);

    // If DFS traversal doesn't visit all vertices with non-zero degree, then return false
    for (int i = 0; i < V; i++)
        if (adj[i].size() > 0 && visited[i] == false)
            return false;

    // Create a reversed graph
    Graph gr = getTranspose();

    // Mark all the vertices as not visited (For second DFS)
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Do DFS for reversed graph starting from the same vertex
    gr.DFSUtil(n, visited);

    // If all vertices are not visited in second DFS, then return false
    for (int i = 0; i < V; i++)
        if (adj[i].size() > 0 && visited[i] == false)
            return false;

    return true;
}

// Function to print the graph
void Graph::printGraph()
{
    for (int v = 0; v < V; v++)
    {
        cout << "Vertex " << v << " -> ";
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            cout << *i << " ";
        cout << endl;
    }
}

// Static method to generate a random graph with specified parameters
Graph Graph::generateRandomGraph(int numVertices, int numEdges, unsigned int seed)
{
    // Initialize random seed
    srand(seed);

    // Create an empty graph
    Graph g(numVertices);

    // Add random edges up to the specified count
    int edgesAdded = 0;

    // We'll try to add edges until we reach the desired number
    // or we've tried too many times (to avoid infinite loops)
    int attempts = 0;
    const int MAX_ATTEMPTS = numVertices * numVertices * 10;

    while (edgesAdded < numEdges && attempts < MAX_ATTEMPTS)
    {
        // Generate random source and destination vertices
        int src = rand() % numVertices;
        int dest = rand() % numVertices;

        // Avoid self-loops for this implementation
        if (src != dest)
        {
            // Check if this edge already exists
            bool edgeExists = false;
            for (auto i = g.adj[src].begin(); i != g.adj[src].end(); ++i)
            {
                if (*i == dest)
                {
                    edgeExists = true;
                    break;
                }
            }

            // If edge doesn't exist, add it
            if (!edgeExists)
            {
                g.addEdge(src, dest);
                edgesAdded++;
            }
        }

        attempts++;
    }

    if (edgesAdded < numEdges)
    {
        cout << "Warning: Could only add " << edgesAdded << " edges out of " << numEdges << " requested." << endl;
    }

    return g;
}

// Function to display usage information
void printUsage(const char* programName)
{
    cout << "Usage: " << programName << " [options]" << endl;
    cout << "Options:" << endl;
    cout << "  -v <num>  Number of vertices (default: 5)" << endl;
    cout << "  -e <num>  Number of edges (default: 10)" << endl;
    cout << "  -s <num>  Random seed (default: current time)" << endl;
    cout << "  -h        Display this help message" << endl;
}

// Main function
int main(int argc, char* argv[])
{
    // Default parameter values
    int numVertices = 5;
    int numEdges = 10;
    unsigned int seed = time(NULL);  // Default to current time

    // Process command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:h")) != -1)
    {
        switch (opt)
        {
            case 'v':  // Number of vertices
                numVertices = atoi(optarg);
                if (numVertices <= 0)
                {
                    cerr << "Number of vertices must be positive." << endl;
                    return 1;
                }
                break;

            case 'e':  // Number of edges
                numEdges = atoi(optarg);
                if (numEdges < 0)
                {
                    cerr << "Number of edges must be non-negative." << endl;
                    return 1;
                }
                break;

            case 's':  // Random seed
                seed = atoi(optarg);
                break;

            case 'h':  // Help
                printUsage(argv[0]);
                return 0;

            default:  // Unknown option
                printUsage(argv[0]);
                return 1;
        }
    }

    // Print the parameters being used
    cout << "Generating random graph with:" << endl;
    cout << "  Vertices: " << numVertices << endl;
    cout << "  Edges: " << numEdges << endl;
    cout << "  Seed: " << seed << endl;

    // Generate a random graph with the specified parameters
    Graph g = Graph::generateRandomGraph(numVertices, numEdges, seed);

    // Print the generated graph
    cout << "\nGenerated Graph:" << endl;
    g.printGraph();

    // Check if the graph has an Eulerian cycle
    if (g.isEulerianCycle())
        cout << "\nThe generated graph has an Eulerian cycle!" << endl;
    else
        cout << "\nThe generated graph does NOT have an Eulerian cycle." << endl;

    return 0;
}