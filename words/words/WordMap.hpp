// WordMap.hpp : map C-style strings as words.
// Sprax Lines,  July 2010

#ifndef WordMap_hpp
#define WordMap_hpp

#include "stdafx.h"
#ifndef WIN32
#include <map>
#include <unordered_map>
#include <unordered_set>
#endif

struct ltstr
{
  inline bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};
struct lessStringStrCmp
{
    inline bool operator()(const std::string& s1, const std::string& s2) const
    {
        return strcmp(s1.c_str(), s2.c_str()) < 0;
    }
};

struct lessStringLen
{
    inline bool operator()(const std::string& s1, const std::string& s2) const
    {
        return s1.length() < s2.length();
    }
};

struct lessCharStrCmp
{
    inline bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) < 0;
    }
};

typedef std::map<const char*, int,      ltstr> WordMap;
typedef std::unordered_map<const char *, unsigned int, std::hash_compare<const char *, lessCharStrCmp>> WordHashMap; 
typedef std::unordered_set<const char*,      ltstr> WordHashSet;



#endif // WordMap_hpp