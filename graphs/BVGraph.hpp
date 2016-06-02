// graphsMain.cpp : Defines the entry point for the console application.
//

#if 0
/** Directed Vertex-centric Graph representation */
class BVGraph
{
public:
    typedef set<Vertex*>                VertSet;
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
    * Return first path found from start vertex to end vertex
    * as a vector of edges, or NULL of no such path is found.
    */
    bool findDfsVertexPath(vector<Vertex*> &path, Vertex *start, const Vertex *end) const
    {
        path.clear();
        if (mEdges.find(start) != mEdges.end()) {
            path.push_back(start);
            return findDfsVertexPathRec(path, end);
        }
        return false;
    }

    bool findDfsVertexPathRec(vector<Vertex*> &path, const Vertex *end) const
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
    * Return first path found from start vertex to end vertex
    * as a vector of edges, or NULL of no such path is found.
    */
    bool findDfsVertexPathMarking(vector<Vertex*> &path, Vertex *start, const Vertex *end)
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

    bool findDfsVertexPathMarkingRec(vector<Vertex*> &path, const Vertex *end)
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
    bool findAndShowVertPath(Vertex& vertA, Vertex& vertB, bool bMarking=false)
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

};
#endif

int test_BVGraph();