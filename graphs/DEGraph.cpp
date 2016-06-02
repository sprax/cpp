// DEGraph.cpp -- useless edge-centric representation of directed graphs
// No self-edges
//
#include "stdafx.h"
#include "DEGraph.hpp"
using namespace std;

/** Directed Edge-centric Graph representation: DON'T USE! */

    // Default constructor: construct empty graph
    DEGraph::DEGraph() {}

    DEGraph::DEGraph(Edge *edges[], int nE)
    {
        for (int j = 0; j < nE; j++)
            addEdge(edges[j]->getBeg(), edges[j]->getEnd());
    }

    DEGraph::DEGraph(Edge *edges[], int nE, Vertex *vertices[], int nV)
    {
        for (int j = 0; j < nE; j++)
            addEdge(edges[j]->getBeg(), edges[j]->getEnd());
        for (int j = 0; j < nV; j++)
            addVertex(vertices[j]);
    }
   
    /**
     *  Add an edge if both vertices are non-NULL and distinct
     */
    bool DEGraph::addEdge(Vertex *vertA, Vertex *vertB) 
    {
        if (vertA == NULL || vertB == NULL || vertA == vertB)
            return false;                   // no side effects on failure
        addVertex(vertA);
        addVertex(vertB);
        Edge *edge = new Edge(vertA, vertB);
        return mEdges.insert(edge).second;
    }

    bool DEGraph::addVertex(Vertex *vert) 
    {
        return mVerts.insert(vert).second;
    }

	/**
	 * Return first path found from start vertex to end vertex
	 * as a vector of edges, or NULL of no such path is found.
	 *
	 * Naive algorithm: O(N**2) because it iterates the graph's edge set for each step in the path.
	 */
	bool DEGraph::findEdgePath(vector<Edge*> &path, const Vertex *start, const Vertex *end) const
	{
		for (set<Edge*>::iterator it = mEdges.begin(); it != mEdges.end(); ++it) {
			Edge *edge = *it;
			if (edge->getBeg() == start) {
				path.push_back(edge);
				bool found = findEdgePathRec(path, end);
				if (found)
					return found;
				else
					path.pop_back();
			}
		}
		return false;
	}

    bool DEGraph::findEdgePathRec(vector<Edge*> &path, const Vertex *end) const
    {
        Edge   *last    = path.back();
        Vertex *lastEnd = last->getEnd();
        if (lastEnd == end)
            return  true;

        for (set<Edge*>::iterator it = mEdges.begin(); it != mEdges.end(); ++it) {
            Edge *edge = *it;
            if (edge->getBeg() == lastEnd) {
                path.push_back(edge);
                bool found = findEdgePathRec(path, end);
                if  (found)
                    return found;
                else
                    path.pop_back();
            }
        }
        return false;
    }

    bool DEGraph::findAndShowEdgePath(const Vertex& vertA, const Vertex& vertB) const
    {
        vector<Edge*> path;
        bool found = findEdgePath(path, &vertA, &vertB);
        cout << "DFS Edge Path from " << vertA.getData() << " to " << vertB.getData() << ": ";
        if (path.empty())
            cout << "-none-";
        else
            for (vector<Edge*>::iterator it = path.begin(); it != path.end(); ++it)
                cout << "[" << (*it)->getBeg()->getData() << " " << (*it)->getEnd()->getData() << "> ";
        cout << endl;
        return found;
    }


int test_DEGraph()
{
    cout << "test_DEGraph:" << endl;
    Vertex vexA(0);
    Vertex vexB(2);
    Vertex vexC(4);
    Vertex vexD(6);
    Vertex vexE(8);
    Vertex vexF(1);
    Vertex vexG(3);
    Vertex vexH(5);
    Vertex *vertArray[] = { &vexF, &vexG, &vexH };
    Edge   edgeAB(&vexA, &vexB);
    Edge   edgeBC(&vexB, &vexC);
    Edge  *edgeArray[]  = { &edgeAB, &edgeBC }; 
    DEGraph deg(edgeArray, 2, vertArray, 3);
    deg.addEdge(&vexC, &vexD);
    deg.addVertex(&vexE);
    deg.findAndShowEdgePath(vexA, vexB);
    deg.findAndShowEdgePath(vexB, vexA);
    deg.findAndShowEdgePath(vexA, vexC);
    deg.findAndShowEdgePath(vexB, vexD);
    deg.findAndShowEdgePath(vexB, vexE);
    deg.findAndShowEdgePath(vexD, vexA);
	return 0;
}

