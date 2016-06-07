// DVGraph.hpp -- vertex-centric representation of directed graphs
// No self-edges.
//
#ifndef DVGraph_hpp
#define DVGraph_hpp
#include "stdafx.h"
#include "Vertex.hpp"
using namespace std;

/** Directed Vertex-centric Graph representation */
class DVGraph
{
public:
    typedef set<Vertex*>                VertSet;
    typedef map<Vertex*, set<Vertex*>*> EdgeMap;

    /** Default constructor: construct empty graph */
    DVGraph();
    DVGraph(Vertex *vertices[], int nV);

    bool addVertex(Vertex *vert);

    /**
    *  Add an edge if both vertices are non-NULL and distinct
    */
    bool addEdge(Vertex *vertA, Vertex *vertB);


    /**
    * Return first path found from start vertex to end vertex
    * as a vector of edges, or NULL of no such path is found.
    */
    bool findDfsVertexPath(vector<Vertex*> &path, Vertex *start, const Vertex *end) const;
    bool findDfsVertexPathRec(vector<Vertex*> &path, const Vertex *end) const;

    /**
    * Return first path found from start vertex to end vertex
    * as a vector of edges, or NULL of no such path is found.
    */
    bool findDfsVertexPathMarking(vector<Vertex*> &path, Vertex *start, const Vertex *end);
    bool findDfsVertexPathMarkingRec(vector<Vertex*> &path, const Vertex *end);

    /**
     *  Beware of loops; this method only works on directed acyclic graphs
     */
    bool findAndShowVertPath(Vertex& vertA, Vertex& vertB, bool bMarking=false);

private:
    VertSet mVerts;
    VertSet mMarks;
    EdgeMap mEdges;

};


int test_DVGraph();

#endif  // DVGraph_hpp