
//////////// FIELD<T> //////////////////////////////////////////////////
/**
 * Field<T>  Field of non-packed, not necessarily connected points.
 */
/*
template <class T> class Evt 
{
public:
    // public data members: We have no compelling reason to protect them:
    TimeT timestamp;    // event timestamp (default is milliseconds)
    T     value;        // event value

    Evt(TimeT ts, T ev) : timestamp(ts), value(ev) {};  // no other constructor

    ~Evt(){};   // not virtual (don't want overhead of virtual function table)
};
*/

template <class T> class Field
{
public:
   
    template <class T> class Point
    {
    public:
        T                   	mData;
        int           	mNumNeighbors;

    protected:
        Point<T> 	 * mNeighbors;

    public:

  
  //Field(int numPoints)  // template constructor
  //{
	 // mNumPoints = numPoints;
  //    createPoints();
  //    setNeighbors();
  //}
  
        std::string toString() {         // generic toString: data only
            return mData.toString();
        }

        T             getData()      { return mData; }
        void         	setData(T t)   { mData = t; }

        Point<T>    * getNeighbors() { return mNeighbors; }

        void        	setNeighbors(Point<T> neighbors[]) {
            mNeighbors = neighbors; }

        Point<T>	  	getNeighbor(int idx)  { return mNeighbors[idx]; }
        void           setNeighbor(int idx, Point<T> fieldPoint) { 
            mNeighbors[idx] = fieldPoint;
        }
    };


public:
	int mNumPoints;  //PointT[][]  mPoints; // Declare as much as possible in terms of the generic Point, not Point<?>


  Point<T> createPoint(double coords[], T val); // return type must be the generic node
  void createPoints();    // calls createPoint(row, col)
  void setNeighbors();   // sets N <= sMaxNumNeighbors neighbors on each node

};

