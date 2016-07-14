
#include "stdafx.h"
#include "Field.hpp"
using namespace std;

//////////// FIELD<T> //////////////////////////////////////////////////
/**
 * Field<T>  Field of non-packed, not necessarily connected points.
 */
class FieldInt : public Field<int>
{
public:
    static const size_t mDim = 2;

    class PointInt : public Point<int>
    {
    public:
        double * mCoords;
        PointInt(double *coords, int val) // generic constructor
        {
            mCoords = new double[mDim];
            for (int dim = mDim; --dim >= 0; )
                mCoords[dim] = coords[dim];
            mData = val;
        };
    };

	PointInt ** mPoints; // Declare as much as possible in terms of the generic Point, not Point<?>

	FieldInt(int numPoints)
	{
	    mNumPoints = numPoints;
        createPoints();
	}

    static double randomDouble()
    {
        return ((double)rand()/(double)RAND_MAX);
    }


    // TODO: replace with something better
	void createPoints() 
    {
		mPoints = new PointInt*[mNumPoints];

		for (int j = 0; j < mNumPoints; j++) {
			
		    //double coords[] = new double[2];
		    double coords[] = { randomDouble(), randomDouble() };
			mPoints[j] = new PointInt( coords, 1 );
		}
	}


	void setNeighbors() {
		// TODO Auto-generated method stub
	}


	PointInt * createPoint(double *coords, int val) {
		return new PointInt(coords, val);
	}
	
	static int unit_test()
	{

		//Sx.puts(FieldInt.class.getName() + ".unit_test");
        string className = "Field";
        cout << className << ".unit_test" << endl;

		FieldInt *pFI = new FieldInt(5);
		PointInt *pPI = pFI->mPoints[0];

		// Sx.puts(pin);
        cout << pPI << endl;


		return 0;
	}

	//static void main(int argc, char *argv[]) {
	//	unit_test();
	//}
};

