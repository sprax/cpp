#include "stdafx.h"


// for_each_stl.cpp
// compile with: /EHsc
#include <map>
#include <iostream>
using namespace std;



#include <map>
using namespace std;
 
struct ltstro
{
  bool operator()(const char* s1, const char* s2) const
  {
      return strcmp(s1, s2) < 0;
  }
};
 
int test_multimap()
{
    multimap<const char*, int, ltstro> m;
 
    m.insert(pair<const char* const, int>("a", 1));
    m.insert(pair<const char* const, int>("c", 2));
    m.insert(pair<const char* const, int>("b", 3));
    m.insert(pair<const char* const, int>("b", 4));
    m.insert(pair<const char* const, int>("a", 5));
    m.insert(pair<const char* const, int>("b", 6));
 
    cout << "Number of elements with key a: " << m.count("a") << endl;
    cout << "Number of elements with key b: " << m.count("b") << endl;
    cout << "Number of elements with key c: " << m.count("c") << endl;
 
    cout << "Elements in m: " << endl;
 
    typedef multimap<const char*, int, ltstro>::iterator itro;
 
    for (itro it = m.begin(); it != m.end(); ++it) {
        cout << "  [" << (*it).first << ", " << (*it).second << "]" << endl;
    }
 
    cout << "  Yes, again!" << endl;
 
    pair<itro, itro> pairit = m.equal_range("b");
    for (itro beg = pairit.first, end = pairit.second; beg != end; ++beg) {
            cout << "  [" << (*beg).first << ", " << (*beg).second << "]" << endl;
 
    }
    return 0;
 
}
 

 

 

 

int test_for_each() 
{
   int retval  = 0;
   map<const char*, int> months;
   
   months["january"] = 31;
   months["february"] = 28;
   months["march"] = 31;
   months["april"] = 30;
   months["may"] = 31;
   months["june"] = 30;
   months["july"] = 31;
   months["august"] = 31;
   months["september"] = 30;
   months["october"] = 31;
   months["november"] = 30;
   months["december"] = 31;
   
   map<const char*, int> months_30;
   
   for each( pair<const char*, int> c in months )
      if ( c.second == 30 )
         months_30[c.first] = c.second;

   for each( pair<const char*, int> c in months_30 )
      retval++;
   
   cout << "Months with 30 days = " << retval << endl;
    return 0;
}

#if 0

class concordance {

  typedef
    std::multimap<std::string,int,
                  std::less<std::string>,
                  std::allocator<std::pair<const std::string,
                                           int> > >
    wordDictType;


public:
  void addWord (std::string, int);
  void readText (std::istream &);
  void printConcordance (std::ostream &);
private:
  wordDictType wordMap;
};
The creation of the concordance is divided into two steps: the program generates the concordance by reading lines from an input stream, then prints the result on the output stream. This is reflected in the two member functions readText() and printConcordance(). The first of these, readText(), is written as follows:

void concordance::readText (std::istream & in) {

  std::string line;

  for (int i = 1; std::getline (in, line); i++)
  {
    allLower (line);
    std::list<std::string, std::allocator<std::string> > words;
    split (line, " , .;:", words);
    std::list<std::string,
              std::allocator<std::string> >::iterator wptr;
    for (wptr = words.begin (); wptr != words.end (); ++wptr)
      addWord (*wptr, i);
  }
}
Lines are read from the input stream one by one. The text of the line is first converted into lower case, then the line is split into words using the function split() described in Section 12.3. Each word is then entered into the concordance. The method used to enter a value into the concordance is as follows:

void concordance::addWord (std::string word, int line) {

  // First get range of entries with same key.
  wordDictType::iterator low = wordMap.lower_bound (word);
  wordDictType::iterator high = wordMap.upper_bound (word);

  // Loop over entires, see if any match current line.
  for ( ; low != high; ++low)
    if ( (*low).second == line)
      return;

  // Didn't occur, add now.
  wordMap.insert (wordDictType::value_type (word, line));
}

The major portion of addWord() is concerned with ensuring that values are not duplicated in the word map if the same word occurs twice on the same line. To assure this, the range of values matching the key is examined, each value is tested, and if any match the line number then no insertion is performed. It is only if the loop terminates without discovering the line number that the new word/line number pair is inserted.

The final step is to print the concordance. This is performed in the following fashion:

void concordance::printConcordance (std::ostream & out) {
  std::string            lastword ("");
  wordDictType::iterator pairPtr;
  wordDictType::iterator stop = wordMap.end ();

  for (pairPtr = wordMap.begin (); pairPtr != stop; ++pairPtr)
    // If word is same as previous, just print line number.
    if (lastword == (*pairPtr).first)
      out << " " << (*pairPtr).second;
    else
    {
      // First entry of word.
      lastword = (*pairPtr).first;
      std::cout << std::endl << lastword << ": "
                << (*pairPtr).second;
    }
  std::cout << std::endl;
}
An iterator loop is used to cycle over the elements being maintained by the word list. Each new word generates a new line of output; thereafter, line numbers appear separated by spaces. For example, if the input was the text:

It was the best of times, 
it was the worst of times.
The output, from best to worst, would be:




#endif