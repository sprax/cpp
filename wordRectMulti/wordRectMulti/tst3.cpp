
#include <stdio.h>

int retson()
{
	
	return 1;
}

template <typename T>
class AA
{
public:
	int mPub;
	T mT;
protected:
	int mPrtA;
	int mPrtB;
	int mPrtC;
	int mPrtD;
	int mPrtE;
private:
	int mPrv;
	
public:
	class BB : public AA<T>
	{
		void printPub() { 
			printf("pub:%d\n", AA<T>::pub);
		}
		
		void setPrt(int a) {
			mPrtA = a;
		}
	};
	
};

template <typename U>
class BB : public AA<U>
{
	void printPub() { 
		printf("pub:%d\n", AA<U>::pub);
	}
	
	void setPrts(int a, int b, int c, int d, int e, U u) 
	{
		AA<U>::mPrtA = a;
		BB<U>::mPrtB = b;
		this->mPrtC = c;
		
#ifdef _MBCS
		using AA<U>::mPrtD;
		mPrtD = d;
		
		using BB<U>::mPrtE;
		mPrtE = e;
#endif
		
		this->mT = u;
		
		
		
		
	}
};