//
// @file     EvtCtr.h
// @author   Steven Lines
// @date     9/22/2005
// @brief    Events, Event Counters, and growing arrays of Event Counters.
//
//  Declarations and most of the definitions for three template classes:
//  Evt :   An event, with a timestamp and a value
//  EvtCtr: An event counter, which maintains an extensible array of events
//      along with minima, maxima, rates of occurence and running averages.
//  EvtCtrMgr: Event counter manager, which maintains an array of event 
//      counters and can trigger operations on all of them (e.g., reset)


#ifndef EVT_CTR_H_INCLUDED
#define EVT_CTR_H_INCLUDED


// typedef SizeT & TimeT: ////////////////////////////////////////////////////

typedef size_t SizeT;   // type used for indexing counter arrays -- unsigned

#define TIME_IS_MILLISECONDS

#ifdef  TIME_IS_MILLISECONDS
typedef DWORD  TimeT;   // if wrap-around is possible, type must be unsigned
#else
#define TIME_IS_UNIX_SECONDS
typedef time_t TimeT;   // no wrap around possible, so use built-in type (long)
#endif

//////////////////////////////////////////////////////////////////////////////

// Microsoft C++ 6 compiler does not support "export" declarator for template 
// functions or template class member functions.  If we're using MSC++, we have
// no choice but to include all the templatized definitions in every source 
// file that uses them.  It's then up to the compiler to instantiate only what
// it needs, and not to keep more than one copy of each needed function.
#ifdef    _MSC_VER
#undef EXPORT_TEMPLATES
#endif

template <class T> class Evt 
{
public:
    // public data members: We have no compelling reason to protect them:
    TimeT timestamp;    // event timestamp (default is milliseconds)
    T     value;        // event value

    Evt(TimeT ts, T ev) : timestamp(ts), value(ev) {};  // no other constructor

    ~Evt(){};   // not virtual (don't want overhead of virtual function table)
};



// Bool can be dangerously ambiguous, so we typedef it
typedef bool        Bool; // MSVC5.0: sizeof(bool)==1; v4.2, sizeof(bool)==4
typedef Evt<Bool>   EvtBool;
typedef Evt<int>    EvtInt;
typedef Evt<float>  EvtFlt;
typedef Evt<double> EvtDbl;



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define EvtCtrMaxNameSize 32   // old-style max length of an event counter's name


// abstract base class; constructor & all data are protected
class EvtCtr
{
    // TODO/FIXME
private: // private assignment operator and copy constructor prevent anyone
    // from copying an instance of a derived calss (or passing by it value)
//    EvtCtr& operator = (EvtCtr& ec) { return ec; }; // definition doesn't matter
//    EvtCtr( const EvtCtr& ec) {};   // actual definition doesn't matter

public: // instantiable derived classes must use one of the enum'd value types
    typedef enum EvtCtrValType {
        eUNK  = 0,
        eInt  = 1,
        eFlt  = 2,
        eDbl  = 3,
        eBool = 4,
    } EvtCtrValType;

    // static constants
    static const unsigned int DefaultNumHeld;   // default number of events to hold
    static const double DBL_MIN;
    static const double DBL_MAX;

protected:
    SizeT  numHeld; // actual number of events
    SizeT  maxHeld; // num events that will fit in allocated space
    SizeT  numEver; // total number of events ever counted, held now or not
    SizeT  idxLast; // index of newest event; idxLast+1==index of oldest value
    SizeT  idxTime; // index of oldest event within chrTime of newest event
    TimeT  begTime; // time initialized (default is when constructed)
    TimeT  endTime; // time of last full update (not of last event added)
    TimeT  chrTime; // characteristic time (if > 0, use it for updating rates)
    double minValHeld; // minimum value currently held (not always up to date)
    double maxValHeld; // maximum value currently held (not always up to date)
    double sumValHeld; // sum of all values held now
    double minValEver; // min value ever seen
    double maxValEver; // max value ever seen
    double sumValEver; // sum of all values ever added
    double aveValHeld; // average of current vals (don't need aveValEver)
    double nowRate;    // rate for the shorter of chrTime or interval held
    double minRate;    // not guaranteed to be updated uniformly
    double maxRate;    // not guaranteed to be updated uniformly
    char   name[EvtCtrMaxNameSize]; // for printing only; we don't really need a 
           // string class here, and we do want to avoid any assumptions about a
           // string class definition wherever this header may be included.
    
    // base class constructor is protected: instantiate only derived classes
    EvtCtr(const char *nm, TimeT t0, TimeT ct, SizeT nAlloc=DefaultNumHeld)
          : numHeld(0), maxHeld(nAlloc), numEver(0), idxLast((SizeT)-1)
          , idxTime((SizeT)-1), begTime(t0), endTime(t0), chrTime(ct)
          , minValHeld(DBL_MAX), maxValHeld(DBL_MIN), sumValHeld(0)
          , minValEver(DBL_MAX), maxValEver(DBL_MIN), sumValEver(0)
          , aveValHeld(0), nowRate(0), minRate(DBL_MAX), maxRate(DBL_MIN)
    {
        strncpy_s(name, nm, EvtCtrMaxNameSize);
    };

public: 
    // destructor
    virtual ~EvtCtr() {}

    // static methods
#ifdef TIME_IS_MILLISECONDS
    static TimeT  GetTime( void ) {  return timeGetTime(); } // milliseconds
#else
    static TimeT  GetTime( void ) {  return time(0); } // unix seconds since 1969
#endif

    // base methods

    inline SizeT  GetNumHeld() const { return numHeld; }
    inline SizeT  GetNumEver() const { return numEver; }
    inline double GetMinEver() const { return minValEver; }
    inline double GetMaxEver() const { return maxValEver; }
    inline double GetAveEver() const { return sumValEver / numEver; }
    inline double GetAveHeld() const { return aveValHeld; } // get running ave
    inline double GetMinRate() const { return minRate; } // min, no updating
    inline double GetMaxRate() const { return maxRate; } // max, no updating
    inline double GetNowRate() const { return nowRate; } // now, no updating
    inline TimeT  GetEndTime() const { return endTime; } // when fully updated
    inline TimeT  SetBegTime(TimeT beg) { // only set begTime if ctr is empty
        return begTime = numHeld ? begTime : beg;
    }
    inline TimeT  SetChrTime(TimeT ct) { return chrTime = ct; }

    // pure virtual methods

    virtual EvtCtr* Clone() const = 0; // public virtual replacement for private copy constructor

    virtual EvtCtrValType GetValType() = 0;

    virtual double  ComputeMinHeld() = 0;
    virtual double  ComputeMaxHeld() = 0;
    virtual double  ComputeAveHeld() = 0; // (re)compute ave to reduce round-off errors
    virtual double  RateNow( TimeT timeNow = 0) = 0; // fast computation of rate
    virtual double  RateDuration( TimeT duration = 0, TimeT end = 0) = 0; // defaults: chrTime, now
    virtual double  RateBegEnd( TimeT beg, TimeT end ) const = 0; // no defaults


    // other (final) methods -- do not overload

    void Reset(TimeT newBegTime) {  // start over, but keep memory
        numHeld = numEver = chrTime = 0;    // But don't reset maxHeld
        idxLast = idxTime = (SizeT)(-1);    // indices initialized to bogus 
        endTime = begTime = newBegTime ? newBegTime : EvtCtr::GetTime();
        minValHeld = minValEver = minRate = DBL_MAX;
        maxValHeld = maxValEver = maxRate = DBL_MIN;
        sumValHeld = sumValEver = aveValHeld = nowRate = 0.0;
    }

    void UpdateMinMaxAndRate( TimeT now ) {
        endTime = ( now && now != begTime) ? now : EvtCtr::GetTime();
        RateNow( endTime );
        if (numHeld == maxHeld) { // recompute only if cyclic buffer has wrapped
            ComputeMinHeld();
            ComputeMaxHeld();
        }
    }

    static inline int PrintCsvHead(FILE *fp = stdout)  {
        return fprintf( fp, 
        "name,numHeld,numEver,begTime,endTime,chrTime,minValHeld,minValEver\
        ,maxValHeld,maxValEver,aveValHeld,aveValEver,nowRate,minRate,maxRate");
    }
    inline int PrintCsvData(FILE *fp = stdout) const {
        return fprintf( fp, "%s,%u,%u,%u,%u,%u,%g,%g,%g,%g,%g,%g,%g,%g,%g\n"
        ,name,numHeld,numEver,begTime,endTime,chrTime,minValHeld,minValEver
        ,maxValHeld,maxValEver,aveValHeld,((double)sumValEver/numEver)
        ,nowRate,minRate,maxRate);
    }
};

//////////////////////////////////////////////////////////////////////////////
// Template base class EvtCtrT: constructor & all data are protected
//////////////////////////////////////////////////////////////////////////////

template <class T> class EvtCtrT : public EvtCtr
{
protected:
    Evt<T> * events;   // allocated array for holding events

    // protected constructor (cannot instantiate)
    EvtCtrT(const char *name, TimeT t0, TimeT scale=0, SizeT nAlloc=EvtCtrDefaultNumHeld) 
        : EvtCtr(name, t0, scale, nAlloc), events(0)
    { /* note: storage for events is allocated in first Add, not here */ }

public:
    ~EvtCtrT()      // virtual destructor
    {
        if (events) {
            free( events );
            events = 0;
        }
    }

    // methods replacing virtual base class methods
    virtual EvtCtr* Clone()  const;

    double ComputeMinHeld(); // recompute min value & update the running min
    double ComputeMaxHeld(); // recompute max value & update the running max
    double ComputeAveHeld(); // running average should be almost as accurate
    double RateNow( TimeT timeNow ); // Fast: no search, no GetTime()
    double RateDuration( TimeT duration = 0, TimeT timeNow = 0); // duration 0 means for all time
    double RateBegEnd( TimeT beg, TimeT end ) const;// no defaults

    inline double RateEver( TimeT timeNow) const
    { return (double) numEver / (timeNow - begTime); } // average rate since begTime

    // other methods (type-dependent)
    inline TimeT  GetTs( SizeT idx) const { 
        return (idx < numHeld) ? events[idx].timestamp : (TimeT)(-1);
    }
    
    inline T      GetVal(SizeT idx) const { 
        return (idx < numHeld) ? events[idx].value : (T)(-1);
    }
    
    SizeT  Add( const Evt<T> &evt );
};


//////////////////////////////////////////////////////////////////////////////
// Instantiable classes EvtCtr*, for each enum type EvtCtrType.
// Use these derived classes for actual event counters (public constructors)
//////////////////////////////////////////////////////////////////////////////

class EvtCtrInt : public EvtCtrT<int> 
{
public: // public constructor
    EvtCtrInt(const char *nm, TimeT bt, TimeT ct=0, SizeT na=DefaultNumHeld)
        : EvtCtrT<int>(nm,bt,ct,na) {};

    EvtCtr* Clone(void)
    {
        EvtCtr *clone = new EvtCtrInt(this->name, this->begTime, this->chrTime, this->numHeld); 
        return  clone;
    }
   
        // public virtual replacement for private copy constructor
    inline EvtCtrValType GetValType() { return eInt; }
};





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class EvtCtrFlt : public EvtCtrT<float> 
{
public:
    EvtCtrFlt(const char *nm, TimeT bt, TimeT ct=0, SizeT na=DefaultNumHeld)
        : EvtCtrT<float>(nm,bt,ct,na) {};
    inline EvtCtrValType GetValType() { return eFlt; }
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class EvtCtrDbl : public EvtCtrT<double> 
{
public:
    EvtCtrDbl(const char *nm, TimeT bt, TimeT ct=0, SizeT na=DefaultNumHeld)
        : EvtCtrT<double>(nm,bt,ct,na) {};
    inline EvtCtrValType GetValType() { return eDbl; }
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class EvtCtrBool : public EvtCtrT<Bool>
{
public:
    EvtCtrBool(const char *nm, TimeT t0, TimeT ct=0, SizeT na=DefaultNumHeld) 
        : EvtCtrT<Bool>(nm, t0, ct, na) {}
    
    inline EvtCtrValType GetValType() { return eBool; }

    // methods replacing base class methods
    double ComputeMinHeld() { 
        for(SizeT j = numHeld; j--; )
            if (events[j].value == false)
                return false;
        return true;
    }
    
    double ComputeMaxHeld() { 
        for(SizeT j = numHeld; j--; )
            if (events[j].value == true)
                return true;
        return true;
    }
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class EvtCtrMgr
{
protected:
    SizeT  numHeld; // number of EvtCtr's currently held
    SizeT  maxHeld; // number of EvtCtr's that will fit in allocated space
    SizeT  numGrow; // number of ptrs to EvtCtr's by which to increment storage
    TimeT  begTime; // time initialized (default is when constructed)
    TimeT  chrTime; // default characteristic time (used only if > 0)
    EvtCtr **ctrs;  // pointer to array of pointers to EvtCtr's (not const)
    
public: 
static const unsigned int DefaultNumAlloc;    // default number of pointers to EvtCtr's;
            // -- but if a EvtCtrMgr runs out of space, it realloc's to get more.


    EvtCtrMgr(TimeT t0, TimeT scale=0, SizeT nGrow=DefaultNumAlloc)
          : numHeld(0), maxHeld(0),  numGrow(nGrow)
          , begTime(t0), chrTime(scale), ctrs(0)
    {
        // note: storage for ctrs is allocated in Add, not here in constructor
    };
    
    virtual ~EvtCtrMgr() {    // virtual destructor
        if (ctrs) {
            free( ctrs );
            ctrs = 0;
        }
    }

    // static methods
        // none

    // base methods
    inline SizeT  GetNum()     const { return numHeld; }
    inline TimeT  GetBegTime() const { return begTime; }
    inline TimeT  GetChrTime() const { return chrTime; }
    
    // other (final) methods -- do not overload
    SizeT  Add( EvtCtr * const pec );
    void   ComputeRatesNow();
    void   ComputeRatesDuration(TimeT duration = 0, TimeT end = 0);
    void   ComputeRatesBegEnd(TimeT beg = 0, TimeT end = 0);

    void   PrintCsvAll(FILE *fp = stdout, TimeT end = 0);
    void   ResetAll(TimeT newBegTime = 0);
};




#ifdef  EXPORT_TEMPLATES
    // The compiler supports the keyword "export," 
    // so the following methods are defined in EvtCtr.cpp

    // Note: Some compilers may be happier if these are declared "export" 
    // inside their class declarations, just like "inline".

export template <class T> SizeT  EvtCtrT<T>::Add( const Evt<T> &evt );
export template <class T> double EvtCtrT<T>::RateNow( TimeT timeNow );
export template <class T> double EvtCtrT<T>::RateDuration( TimeT duration, TimeT timeNow );
export template <class T> double EvtCtrT<T>::RateBegEnd( TimeT beg, TimeT end ) const; 
export template <class T> double EvtCtrT<T>::ComputeMinHeld();
export template <class T> double EvtCtrT<T>::ComputeMaxHeld();
export template <class T> double EvtCtrT<T>::ComputeAveHeld();

#else // template definitions must be included, not exported from obj files

template <class T> SizeT EvtCtrT<T>::Add( const Evt<T> &evt )
{
    TimeT  beg = begTime, evtTime = evt.timestamp;
    double value = evt.value;
    numEver++;
    if (minValEver > value)       // The new value is also the new minimum
        minValEver = value;
    if (maxValEver < value)       // The new value is also the new maximum
        maxValEver = value;
    sumValEver    += value;       //
    if (numHeld < maxHeld) {
        if(!events) { // events has not yet been allocated
            // allocate events storage here, NOT in constructor
            events = (Evt<T> *)malloc(maxHeld * sizeof( Evt<T> ));
            if (! events) {
                printError("EvtCtrT::Add -- failed to alloc data size %u\n", maxHeld*sizeof(Evt<T>));
                return (SizeT)-1;   // 0xFFFFFFFF signals error
            }
            idxTime = 0;
        }
        else if(chrTime) {
            if (chrTime >= evtTime - events[0].timestamp)
                idxTime = 0;
            else {   //  chrTime < evtTime - events[0].timestamp; idxTime starts at 0
                do { ++idxTime; 
                } while (chrTime < evtTime - events[idxTime].timestamp);
            }
        }
        minValHeld = minValEver;
        maxValHeld = maxValEver;
        idxLast    = numHeld++;
    }
    else {
        idxLast = (idxLast + 1) % maxHeld; // index of event to be replaced
        beg     = events[idxLast].timestamp;     // timestamp of outgoing event
        sumValHeld -= events[idxLast].value; // subtract value of event to be replaced
    }
    events[idxLast] = evt;           // Store the new event
    sumValHeld += value;               // gain the value of the added event
    aveValHeld  = sumValHeld / numHeld;     // update running average
    if (0 < chrTime && chrTime < evtTime - events[idxTime].timestamp) {
        do {
            idxTime = (idxTime + 1) % maxHeld;
        } while (chrTime < evtTime - events[idxTime].timestamp);
        nowRate = (double)((idxLast -  idxTime + 1) % numHeld) / chrTime;
    }
    else {
        nowRate = (double) numHeld / (evtTime - beg);
    }
    return idxLast;
}


// This is the fast method for computing the current Rate.
// It uses stored information to avoid any searching,
// and requires a valid timeNow argument (to avoid calling GetTime())
template <class T> double EvtCtrT<T>::RateNow( TimeT timeNow )
{
    if (! numHeld)
        return nowRate = 0.0;

    TimeT  allTime = timeNow - begTime;
    if (chrTime > 0 && allTime > chrTime) {
        nowRate = (double)((idxLast - idxTime + 1) % numHeld) / chrTime;
    }
    else {
        // compute rate using all held events
        nowRate = (double) numHeld / allTime;
    }
    if (minRate > nowRate)
        minRate = nowRate;
    if (maxRate < nowRate)
        maxRate = nowRate;

    return nowRate;
}


template <class T> double EvtCtrT<T>::RateDuration( TimeT duration, TimeT timeNow )
{
    if (! numHeld)
        return 0.0;

    TimeT end = timeNow ? timeNow : GetTime();     // time now

    if (duration <= 0)
        duration  = chrTime;    // 1st default

    TimeT beg = begTime;        // 2nd default (duration may still == 0)
    if (duration > 0 && duration < end - beg)
        beg = end - duration;   // really use duration

    return RateBegEnd( beg, end );
}


template <class T> double EvtCtrT<T>::RateBegEnd( TimeT beg, TimeT end ) const 
{
    TimeT dif, duration = end - beg;
    if (! numHeld || duration <= 0)
        return 0.0;

    // Only differences between timestamps are meaningful
    SizeT num = 0;
	for(SizeT j = numHeld; j--; ) {
        dif = end - events[j].timestamp;
        if (dif <= duration) {
            num++;
        }
    }
    return (double) num / duration;
}



template <class T> double EvtCtrT<T>::ComputeMinHeld() 
{
    minValHeld = DBL_MAX;
    for(SizeT j = numHeld; j--; ) { 
        if (minValHeld > (double) events[j].value) {
            minValHeld = (double) events[j].value;
        }
    }
    return minValHeld;
}


template <class T> EvtCtr* EvtCtrT<T>::Clone() const
{
//  FIXME -- WRONG
    return (EvtCtr*)(this);
}


template <class T> double EvtCtrT<T>::ComputeMaxHeld() 
{
    maxValHeld = DBL_MIN;
    for(SizeT j = numHeld; j--; ) {                     // FIXME -- off by 1?
        if (maxValHeld < (double) events[j].value) {
            maxValHeld = (double) events[j].value;
        }
    }
    return maxValHeld;
}


// Usually the running average will be accurate enough,
// but computing it afresh will remove any round-off
// errors thay may have accumulated if numHeld is large.
template <class T> double EvtCtrT<T>::ComputeAveHeld() 
{
    sumValHeld = 0.0;
    for(SizeT j = numHeld; j--; ) { 
        sumValHeld += events[j].value;
    }
    aveValHeld = sumValHeld / numHeld;
    return aveValHeld;
}


#endif // ! defined(EXPORT_TEMPLATES)

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


#endif // EVT_CTR_H_INCLUDED
