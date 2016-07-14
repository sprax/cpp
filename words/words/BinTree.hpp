#ifndef BinTree_hpp
#define BinTree_hpp

void test_BinLink();

class BinTree 
{
    static int mCount;
    static int setCount(int num) { return mCount = num; }
    static int getCount()        { return mCount; }
    static int addCount(int num) { return mCount += num; }
};


#endif // BinTree_hpp