// DEGraph.cpp -- useless edge-centric representation of directed graphs
// No self-edges.
//
#ifndef DEGraph_hpp
#define DEGraph_hpp
#include "stdafx.h"
#include "Vertex.hpp"

// non-public Edge class
class Edge 
{
public:
    Edge(Vertex *beg, Vertex *end) : mBeg(beg), mEnd(end) {}
    Vertex *getBeg()    { return mBeg; }
    Vertex *getEnd()    { return mEnd; }

private:
    Vertex *mBeg;
    Vertex *mEnd;
};

/** Directed Edge-centric Graph representation: DON'T USE! */
class DEGraph
{
public:
    // Default constructor: construct empty graph
    DEGraph();
    DEGraph(Edge *edges[], int nE);
    DEGraph(Edge *edges[], int nE, Vertex *vertices[], int nV);

    /**
     *  Add an edge if both vertices are non-NULL and distinct
     */
    bool addEdge(Vertex *vertA, Vertex *vertB);
    bool addVertex(Vertex *vert);

    /**
     * Return first path found from start vertex to end vertex
     * as a vector of edges, or NULL of no such path is found.
     */
    bool findEdgePath(std::vector<Edge*> &path, const Vertex *start, const Vertex *end) const;
    bool findEdgePathRec(std::vector<Edge*> &path, const Vertex *end) const;
    bool findAndShowEdgePath(const Vertex& vertA, const Vertex& vertB) const;

private:
    std::set<Edge*>      mEdges;
    std::set<Vertex*>    mVerts;
};


#endif
