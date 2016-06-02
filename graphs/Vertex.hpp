// graphs.hpp -- "external" header for graphs project
//
#ifndef Vertex_hpp
#define Vertex_hpp

class Vertex
{
public:
    Vertex(int data) : mData(data) {}
    int getData()  const  { return mData; }
private:
    int mData;
};


#endif  // Vertex_hpp
