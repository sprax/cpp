// DVGraph.cpp -- vertex-centric representation of directed graphs
// No self-edges.
//
#include "stdafx.h"
#include "DVGraph.hpp"
using namespace std;


/** Directed Vertex-centric Graph representation */

    // Default constructor: construct empty graph
    DVGraph::DVGraph() {}

    DVGraph::DVGraph(Vertex *vertices[], int nV)
    {
        for (int j = 0; j < nV; j++)
            addVertex(vertices[j]);
    }   

    bool DVGraph::addVertex(Vertex *vert) 
    {
        return mVerts.insert(vert).second;
    }
    /**
    *  Add an edge if both vertices are non-NULL and distinct
    */
    bool DVGraph::addEdge(Vertex *vertA, Vertex *vertB) 
    {
        if (vertA == NULL || vertB == NULL || vertA == vertB)
            return false;                   // no side effects on failure
        addVertex(vertA);
        addVertex(vertB);
        if (mEdges.find(vertA) == mEdges.end()) {
            mEdges.insert(EdgeMap::value_type(vertA, new VertSet()));
        }
        VertSet *nexts = mEdges.at(vertA);
        return   nexts->insert(vertB).second;
    }

    /**
    * Return first path found from start vertex to end vertex
    * as a vector of vertices, or NULL of no such path is found.
    */
    bool DVGraph::findDfsVertexPath(vector<Vertex*> &path, Vertex *start, const Vertex *end) const
    {
        path.clear();
        if (mEdges.find(start) != mEdges.end()) {
            path.push_back(start);
            return findDfsVertexPathRec(path, end);
        }
        return false;
    }

    bool DVGraph::findDfsVertexPathRec(vector<Vertex*> &path, const Vertex *end) const
    {
        Vertex *lastVert = path.back();
        if (lastVert == end)
            return  true;

        if (mEdges.find(lastVert) != mEdges.end()) {
            VertSet *nexts = mEdges.at(lastVert);
            for (auto it = nexts->begin(); it != nexts->end(); ++it) {                
                path.push_back(*it);
                bool found = findDfsVertexPathRec(path, end);
                if ( found )
                    return found;
                else
                    path.pop_back();
            }
        }
        return false;
    }


    /**
    * Depth-first search for a path from start vertex to end vertex.
    * If found, the first such path is placed in the path argument (a vector
    * of vertices), and the function returns true.
    * If no such path is found, the functions returns false, and the supplied 
    * path will be empty.
    */
    bool DVGraph::findDfsVertexPathMarking(vector<Vertex*> &path, Vertex *start, const Vertex *end)
    {
        mMarks.clear();
        path.clear();
        if (mEdges.find(start) != mEdges.end()) {
            mMarks.insert(start);
            path.push_back(start);
            return findDfsVertexPathMarkingRec(path, end);
        }
        return false;
    }

    bool DVGraph::findDfsVertexPathMarkingRec(vector<Vertex*> &path, const Vertex *end)
    {
        Vertex *lastVert = path.back();
        if (lastVert == end)
            return  true;

        if (mEdges.find(lastVert) != mEdges.end()) {
            VertSet *nexts = mEdges.at(lastVert);
            for (VertSet::iterator it = nexts->begin(); it != nexts->end(); ++it) {
                Vertex *vert = *it;
                if (mMarks.find(vert) == mMarks.end()) {
                    mMarks.insert(vert);
                    path.push_back(vert);
                    bool found = findDfsVertexPathMarkingRec(path, end);
                    if ( found )
                        return found;
                    else
                        path.pop_back();
                }
            }
        }
        return false;
    }

    /**
    *  Beware of loops; this method only works on directed acyclic graphs
    */
    bool DVGraph::findAndShowVertPath(Vertex& vertA, Vertex& vertB, bool bMarking)
    {
        cout << "DFS Vert Path from " << vertA.getData() << " to " << vertB.getData() << ": ";
        vector<Vertex*> path;
        bool found = false;
        if (bMarking)
            found = findDfsVertexPathMarking(path, &vertA, &vertB);
        else
            found = findDfsVertexPath(path, &vertA, &vertB);
        if ( found ) {
            vector<Vertex*>::iterator it = path.begin();
            cout << (*it)->getData();
            ++it;
            for (; it != path.end(); ++it) {
                cout << " : " << (*it)->getData();
            }
        } else {
            cout << "-none-";
        }
        cout << endl;
        return found;
    }



int test_DVGraph()
{
    cout << "test_DVGraph:" << endl;
    Vertex vexA(0);
    Vertex vexB(2);
    Vertex vexC(4);
    Vertex vexD(6);
    Vertex vexE(8);
    Vertex vexF(1);
    Vertex vexG(3);
    Vertex vexH(5);
    Vertex *vertArray[] = { &vexF, &vexG, &vexH };
    DVGraph deg(vertArray, 3);
    deg.addEdge(&vexA, &vexB);
    deg.addEdge(&vexB, &vexC);
    deg.addEdge(&vexC, &vexD);
    deg.addEdge(&vexD, &vexE);
    deg.addEdge(&vexF, &vexG);
    deg.addEdge(&vexF, &vexH);
    deg.addEdge(&vexG, &vexH);
    deg.addVertex(&vexE);
    deg.findAndShowVertPath(vexA, vexB);
    deg.findAndShowVertPath(vexB, vexA);
    deg.findAndShowVertPath(vexA, vexC);
    deg.findAndShowVertPath(vexB, vexD);
    deg.findAndShowVertPath(vexB, vexE);
    deg.findAndShowVertPath(vexD, vexA);

    cout << "DFS search in the presennce of cycles?" << endl;
    bool bMarking = true;
    deg.addEdge(&vexC, &vexA);
    deg.findAndShowVertPath(vexA, vexE, bMarking);
    deg.findAndShowVertPath(vexB, vexF, bMarking);

    return 0;
}
