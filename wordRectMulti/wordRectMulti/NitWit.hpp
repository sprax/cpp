// NitWit.hpp : unfinished pseudo iterator for pairs of ints
// Sprax Lines,  July 2010

#ifndef NitWit_hpp
#define NitWit_hpp

#include <assert.h>
#include <map>

class Nit   // final
{
public:
    typedef unsigned int uint;
    typedef std::pair<uint,uint> NitPair;



    Nit(uint minTall, uint maxTall, uint minWide, uint maxWide, bool asc=true) : ascend(asc)
    {
        if (ascend) {
            begTall = curTall = minTall;  endTall = maxTall + 1;
            begWide = curWide = minWide;  endWide = maxWide + 1;
        } else {
            begTall = curTall = maxTall;  endTall = minTall - 1;
            begWide = curWide = maxWide;  endWide = minWide - 1;
        }
    }

    inline const Nit& beg()  const { return *this; }
    inline const NitPair cur()  const { return NitPair(curTall, curWide); }
    inline const Nit end()  const { return *this; }

    Nit & operator++() {
        if (curWide < endWide) {
          ++curWide;
        }
        else if (curTall < endTall) {
        }
        return *this; 
    }

#if 0
    void test () {
        Nit nit(4, 5, 7, 8);
        for (Nit np = nit.beg()
            ; np != nit.end()
            ; ++np) {
            int t = nit.mCur.first;
            int w = nit.mCur.second;
        }
    }
#endif

// private: // zoid
    uint begTall, curTall, endTall, begWide, curWide, endWide;
    int duh;
    const bool  ascend;
    const NitPair mBeg;
    const NitPair mCur;
    const NitPair mEnd;
};


#endif // NitWit_hpp