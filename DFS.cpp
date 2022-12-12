// https://github.com/Sajed262
//
//

#include "Graph.h"
#include "Queue.h"
#include <limits.h>
#include <assert.h>

// making a type to return all the info from the algorithm.
class DFSInfo;
DFSInfo &depthFirstSearch(Graph &graph, int source = 0);
void visit(Graph &g, int u, DFSInfo &dfs, int *s);

// the dfs choses the vertices in visit you by order.(from the small index to the big)
// by default the dfs starts from the first vertex 0.
DFSInfo &depthFirstSearch(Graph &graph, int source = 0) // THE ALGORITHM.
{
    assert((source < 0 || source >= graph.numOfVertices()));

    // initializing
    int status[graph.numOfVertices()] = {0}; // 0 for unvisited vertices,1 for visited.
    DFSInfo info(graph.numOfVertices());

    // the algorithm
    visit(graph, source, info, status); // visiting the source in case the source is not 0.
    for (int u = 0; u < graph.numOfVertices(); u++)
        if (status[u] == 0)
            visit(graph, u, info, status);

    return info;
}

// the time is in refrence so that we work on the real variable time and not a copy of it.
void visit(Graph &g, int u, DFSInfo &dfs, int *s)
{
    dfs.discovery[u] = dfs.time++;   // u gets the current time, and we add the time by 1.
    s[u] = 1;                        // u marked as visited.
    g.getEdges()[u].getFirstKey();   // sets the iterator to the first edge.
    while (g.getEdges()[u].Iterator) // assume the edge is u->v for each v that forms an edge with u:
    {
        int v = g.getEdges()[u].Iterator->key;
        if (s[v] == 0) // if v is not visited
        {
            dfs.parent[v] = u;   // thats where the depth comes from, cause we go deep in the vertices and by the length from the source (like BFS).
            visit(g, v, dfs, s); // visit v
        }
    }
    dfs.finish[u] = dfs.time++;
}

class DFSInfo
{
public:
    int vertices;
    int time;
    int *discovery; // discovery time for each vertex.
    int *finish;    // finish time for each vertex.
    int *parent;    // let u->v an edge if v discovered because of that edge, then u is v's parent.

    DFSInfo(int vertices) : vertices(vertices), time(0), discovery(new int[vertices]),
                            finish(new int[vertices]), parent(new int[vertices])
    {
        for (int i = 0; i < vertices; i++)
            discovery[i] = finish[i] = parent[i] = -1;
    }

    DFSInfo(const DFSInfo &dfs) : vertices(dfs.vertices), time(dfs.time), discovery(new int[vertices]),
                                  finish(new int[vertices]), parent(new int[vertices])
    {
        for (int i = 0; i < vertices; i++)
        {
            discovery[i] = dfs.discovery[i];
            finish[i] = dfs.finish[i];
            parent[i] = dfs.parent[i];
        }
    }

    DFSInfo &operator=(const DFSInfo &dfs)
    {
        if (this == &dfs)
            return *this;

        delete[] discovery;
        delete[] finish;
        delete[] parent;

        vertices = dfs.vertices;
        time = dfs.time;
        discovery = new int[vertices];
        finish = new int[vertices];
        parent = new int[vertices];

        for (int i = 0; i < vertices; i++)
        {
            discovery[i] = dfs.discovery[i];
            finish[i] = dfs.finish[i];
            parent[i] = dfs.parent[i];
        }

        return *this;
    }

    ~DFSInfo()
    {
        delete[] discovery;
        delete[] finish;
        delete[] parent;
    }
};
