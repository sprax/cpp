// WordMap.hpp : map C-style strings as words.
// Sprax Lines,  July 2010

#ifndef WordMap_hpp
#define WordMap_hpp

#include "stdafx.h"
#ifndef WIN32
#include <map>
#endif

struct ltstr
{
  inline bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};
typedef std::map<const char*, int, ltstr> WordMap;


#endif // WordMap_hpp