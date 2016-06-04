// graphsMain.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <queue>

#include "graphs.hpp"
#include "Sx.h"

using namespace std;


/** Directed Vertex-centric Graph representation */
class BVGraph
{
public:
    typedef set<Vertex*>                VertSet;
    typedef map<Vertex*, Vertex*>       VertMap;
    typedef map<Vertex*, set<Vertex*>*> EdgeMap;

private:
    VertSet mVerts;
    VertSet mMarks;
    EdgeMap mEdges;

public:

    // Default constructor: construct empty graph
    BVGraph() {}

    BVGraph(Vertex *vertices[], int nV)
    {
        for (int j = 0; j < nV; j++)
            addVertex(vertices[j]);
    }

    bool addVertex(Vertex *vert)
    {
        return mVerts.insert(vert).second;
    }

    /**
    *  Add an edge if both vertices are non-NULL and distinct
    */
    bool addEdge(Vertex *vertA, Vertex *vertB)
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
    * Breadth-first search for a path from start vertex to end vertex.
    * If found, the first such path is placed in the path argument (a vector
    * of vertices), and the function returns true.
    * If no such path is found, the functions returns false, and the supplied
    * path will be empty.
    */
    bool findBfsVertexPath(vector<Vertex*> &path, Vertex *start, Vertex *end) const
    {
        bool found = false;
        path.clear();
        queue<Vertex*> vertQ;
        VertMap preVerts;
        if (mEdges.find(start) != mEdges.end()) {
            vertQ.push(start);
            while (!vertQ.empty()) {
                Vertex *vert = vertQ.back();
                vertQ.pop();
                if (mEdges.find(vert) != mEdges.end()) {
                    VertSet *nexts = mEdges.at(vert);
                    for (auto it = nexts->begin(); it != nexts->end(); ++it) {
                        preVerts.insert(VertMap::value_type(*it, vert));
                        if (*it == end) {
                            found = true;
                            break;
                        }
                        vertQ.push(*it);
                    }
                    if (found) {
                        path.push_back(end);
                        for (Vertex *vert = preVerts.at(end); vert != start; vert = preVerts.at(vert))
                            path.push_back(vert);
                        path.push_back(start);
                        return true;
                    }
                }

            }
        }
        return false;
    }


    /**
* Return first path found from start vertex to end vertex
* as a vector of edges, or NULL of no such path is found.
*/



    bool findBfsVertexPathMarking(vector<Vertex*> &path, Vertex *start, const Vertex *end)
    {
        mMarks.clear();
        path.clear();
        //queue<Vertex*, vector> vertQ;
        if (mEdges.find(start) != mEdges.end()) {
            mMarks.insert(start);
            path.push_back(start);
            return findBfsVertexPathMarkingRec(path, end);
        }
        return false;
    }

    bool findBfsVertexPathMarkingRec(vector<Vertex*> &path, const Vertex *end)
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
                    bool found = findBfsVertexPathMarkingRec(path, end);
                    if (found)
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
    bool findAndShowVertPath(Vertex& vertA, Vertex& vertB, bool bMarking = false)
    {
        cout << "DFS Vert Path from " << vertA.getData() << " to " << vertB.getData() << ": ";
        vector<Vertex*> path;
        bool found = false;
        if (bMarking)
            found = findBfsVertexPathMarking(path, &vertA, &vertB);
        else
            found = findBfsVertexPath(path, &vertA, &vertB);
        if (found) {
            cout << "YES  ";
            if (!path.empty()) {
                auto it = path.rbegin();
                cout << (*it)->getData();
                ++it;
                for (; it != path.rend(); ++it) {
                    cout << " : " << (*it)->getData();
                }
            }
        }
        else {
            cout << "-none-";
        }
        cout << endl;
        return found;
    }

};


int test_BVGraph()
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
    BVGraph bvg(vertArray, 3);
    bvg.addEdge(&vexA, &vexB);
    bvg.addEdge(&vexB, &vexC);
    bvg.addEdge(&vexC, &vexD);
    bvg.addEdge(&vexD, &vexE);
    bvg.addVertex(&vexE);
    bvg.findAndShowVertPath(vexA, vexB);
    bvg.findAndShowVertPath(vexB, vexA);
    bvg.findAndShowVertPath(vexA, vexC);
    bvg.findAndShowVertPath(vexB, vexD);
    bvg.findAndShowVertPath(vexB, vexF);
    bvg.findAndShowVertPath(vexD, vexA);

    cout << "BFS search in the presennce of cycles?" << endl;
    bool bMarking = true;
    bvg.addEdge(&vexC, &vexA);
    bvg.findAndShowVertPath(vexA, vexE, bMarking);
    bvg.findAndShowVertPath(vexB, vexF, bMarking);

    return 0;
}

int testGridGuards();

int main(int argc, char* argv[])
{
    vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(2);
    int last = vec.back();
    cout << "last: " << last << endl;
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
        cout << *it << endl;

    //test_DEGraph();
    test_DVGraph();
    //test_BVGraph();

    testGridGuards();
    Sx::unit_test();

    return 0;
}

