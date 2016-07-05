// 
//
/****************************************
#if  _MSC_VER
    // Microsoft C/C++ does not support C++ keyword "export" for template
    // class member functions, so do nothing here
#endif
*****************************************/


#include "stdafx.h"
#include "EvtCtr.h"


const double EvtCtr::DBL_MIN = -1.0e308; // better than -(double)INT_MIN
const double EvtCtr::DBL_MAX =  1.0e308; // better than  (double)INT_MAX

const unsigned int EvtCtr::DefaultNumHeld  = 512;   // default number of events to hold

const unsigned int EvtCtrMgr::DefaultNumAlloc = 256; // default number of pointers to EvtCtr's;



#ifdef EXPORT_TEMPLATES

// If the compiler supports the "export" keyword, it's generally
// better to define exportable methods in the .cpp source files,
// rather than in the .h header.  MS VC does not support "export,"
// so for now, the templatized methods are defined in the header.
// ComputeMinHeld is here only as an example of using "export."

#error move (or copy) exportable methods from EvtCtr.h to EvtCtr.cpp
   

// To use the "separation" model for compiling template functions only once,
// define the template functions here.  The export keyword here is optional.
// For example:
export template <class T> double EvtCtrT<T>::ComputeMinHeld() {
    minValHeld = DBL_MAX;
    for(SizeT j = numHeld; j--; ) { 
        if (minValHeld > (double) events[j].value) {
            minValHeld = (double) events[j].value;
        }
    }
    return minValHeld;
}

#endif // EXPORT_TEMPLATES




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


SizeT EvtCtrMgr::Add( EvtCtr * const pec )
{   // allocate ctrs storage here, NOT in constructor
    size_t idx = numHeld;
    if(++numHeld > maxHeld ) { // ctrs is not yet allocated or is out of space
        maxHeld += numGrow;
        ctrs = (EvtCtr **)realloc(ctrs, maxHeld * sizeof( EvtCtr * ));
        if (! ctrs) {
            printError("EvtCtrMgr::Add -- failed to alloc data size %u\n"
                , maxHeld*sizeof(EvtCtr *));
            return (SizeT)-1;   // 0xFFFFFFFF signals error
        }
    }
    ctrs[idx] = pec;
    return idx;   // index of last-added entry
}


void   EvtCtrMgr::ComputeRatesNow()
{   // not const blistEvtCtruse it may change a EvtCtr's minRate & maxRate
    TimeT timeNow = EvtCtr::GetTime();
    for (SizeT j = 0; j < numHeld; j++) {
        ((EvtCtr *)ctrs[j])->RateNow(timeNow);
    }
}


void   EvtCtrMgr::ComputeRatesDuration(TimeT duration, TimeT end)
{   // not const blistEvtCtruse it may change a EvtCtr's minRate & maxRate
    end = end ? end : EvtCtr::GetTime();     // time now
    for (SizeT j = 0; j < numHeld; j++) {
        ((EvtCtr *)ctrs[j])->RateDuration(duration, end);
    }
}


void   EvtCtrMgr::ComputeRatesBegEnd(TimeT beg, TimeT end)
{   // not const blistEvtCtruse it may change a EvtCtr's minRate & maxRate
    end = end ? end : EvtCtr::GetTime();     // time now
    for (SizeT j = 0; j < numHeld; j++) {
        ((EvtCtr *)ctrs[j])->RateBegEnd(beg, end);
    }
}


void   EvtCtrMgr::PrintCsvAll(FILE *fp, TimeT end)
{
    TimeT endTime = end ? end : EvtCtr::GetTime();
    EvtCtr::PrintCsvHead(fp);
    for (SizeT j = 0; j < numHeld; j++) {
        if (ctrs[j]->GetEndTime() < endTime)
            ctrs[j]->UpdateMinMaxAndRate(endTime);
        ctrs[j]->PrintCsvData(fp);
    }
}


void   EvtCtrMgr::ResetAll(TimeT newBegTime)
{
    TimeT beg = newBegTime ? newBegTime : EvtCtr::GetTime();
    for (SizeT j = 0; j < numHeld; j++) {
        ctrs[j]->Reset(beg);
    }
}

