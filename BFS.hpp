// https://github.com/Sajed262
//
//

#ifndef BFS
#define BFS

#include "Graph.h"
#include "Queue.h"
#include <assert.h>
#include <limits.h>

class BFSInfo
{
public:
    int source;
    int vertices;
    int reached_vertices;   // initialized to 1 because the source is reached from the source with length 0.
    Queue<int> qu;          // each vertex enters the queue exactly once. the diffrence between the head and the last element in the queue is <= 1 all the time.
    int *distance;          // the distance for each vertex from the source.
    int *parent;            // let u->v or u-v an edge if v discovered because of that edge, then u is v's parent.
    int *reached_by_source; // at the end of the algo' the vertices that has a path with the source well be 1, as in "entered the queue". -1 wasn't in the queue.
                            // 1 can also mean that the vertex is in the tree of source.

    BFSInfo(int s, int v) : source(s), vertices(v), reached_vertices(1),
                            distance(new int[vertices]), parent(new int[vertices]),
                            reached_by_source(new int[vertices])
    {
        for (int i = 0; i < vertices; i++)
        {
            distance[i] = INT_MAX;
            parent[i] = reached_by_source[i] = -1;
        }

        // the source is reached from the source with length 0.
        distance[source] = 0;
        reached_by_source[source] = 1;
        qu.enqueue(source, source);
    }

    BFSInfo(const BFSInfo &bfs) : source(bfs.source), vertices(bfs.vertices), reached_vertices(bfs.reached_vertices),
                                  distance(new int[vertices]), parent(new int[vertices]),
                                  reached_by_source(bfs.reached_by_source)
    {
        for (int i = 0; i < vertices; i++)
        {
            distance[i] = bfs.distance[i];
            parent[i] = bfs.parent[i];
            reached_by_source[i] = bfs.reached_by_source[i];
        }
    }

    BFSInfo &operator=(const BFSInfo &bfs)
    {
        if (this == &bfs)
            return *this;

        delete[] distance;
        delete[] parent;
        delete[] reached_by_source;

        source = bfs.source;
        vertices = bfs.vertices;
        reached_vertices = bfs.reached_vertices;
        distance = new int[vertices];
        parent = new int[vertices];
        reached_by_source = new int[vertices];

        for (int i = 0; i < vertices; i++)
        {
            distance[i] = bfs.distance[i];
            parent[i] = bfs.parent[i];
            reached_by_source[i] = bfs.reached_by_source[i];
        }

        return *this;
    }

    ~BFSInfo()
    {
        delete[] distance;
        delete[] parent;
        delete[] reached_by_source;
    }
};


// the bfs array is the output, each element is the the shortest path from source.
// pay attention to delete the bfs array when you finish using it.
BFSInfo breadthFirstSearch(Graph &gr, int source)
{
    assert(source < gr.numOfVertices());
    assert(source >= 0);

    // initializing
    BFSInfo bfs(source, gr.numOfVertices());

    // the algorithm
    while (!bfs.qu.isEmpty())
    {
        int head = bfs.qu.head()->key;
        gr.getEdges()[head].getFirstKey();

        while (gr.getEdges()[head].Iterator)
        {
            int dest = gr.getEdges()[head].Iterator->key;
            if (bfs.reached_by_source[dest] == -1) // that means dest was never in the queue.
            {
                bfs.reached_by_source[dest] = 1;
                bfs.distance[dest] = bfs.distance[head] + 1;
                bfs.qu.enqueue(dest, dest);
                bfs.reached_vertices++;
                bfs.parent[dest] = head;
            }
            gr.getEdges()[head].getNextKey();
        }

        bfs.qu.dequeue();
    }

    return bfs;
}


#endif //BFS