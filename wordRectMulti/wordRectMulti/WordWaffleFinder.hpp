// wordRectFinder.hpp : find largest dense rectangles of words from a dictionary, multi-threaded
// Sprax Lines, July 2010

#ifndef WordWaffleFinder_hpp
#define WordWaffleFinder_hpp

#include "WordRectFinder.hpp"

/**
 *
 *  Examples of Word Waffles:
 *    A)  Where the minor rows and cols
 *        are also words:
 *
 *      A A R R G H H       A B A S E M E N T S
 *      A   E   R   O       M   E   N   S   A
 *      R E D H E A D       P A R A S I T I S E
 *      R   H   E   A       L   O   U   R   S
 *      G R E E N E D       E M B L E M A T I C
 *      H   A   E   D       R   E   D   Y   E
 *      H O D A D D Y
 */
template <typename T>
class WordWaffleFinder : public WordRectFinder<T>
{
public:
	typedef int (WordWaffleFinder::*rowFinderFn)(int);

    WordWaffleFinder(int wide, int tall, unsigned int options, WordTrie<T> *wordTries[], const WordMap *maps=NULL) 
        : WordRectFinder<T>(wide, tall, options, wordTries, maps)
        , mOddWide((wide + 1) / 2)
        , mOddTall((tall + 1) / 2)
        , mEvnRowTrie(*wordTries[wide])
        , mEvnColTrie(*wordTries[tall])
        , mOddRowTrie(*wordTries[mOddWide])
        , mOddColTrie(*wordTries[mOddTall])
    { 
        if (tall % 2) {	// gcc requires mWantTall to be qualified: WordRectFinder<T>::mWantTall
            mOddRowFinder = &WordWaffleFinder::findOddRowWantingOddTotal;
            mEvnRowFinder = &WordWaffleFinder::findEvnRowWantingOddTotal;
        } else { 
            mOddRowFinder = &WordWaffleFinder::findOddRowWantingEvnTotal;
            mEvnRowFinder = &WordWaffleFinder::findEvnRowWantingEvnTotal;
        }
    }

	WordWaffleFinder(const WordWaffleFinder&);              // Prevent pass-by-value by not defining this copy constructor.
	WordWaffleFinder& operator=(const WordWaffleFinder&);   // Prevent assignment by not defining this operator.
	virtual ~WordWaffleFinder() { }

protected:
	virtual void initRowsAndCols();
private:
	virtual int  findWordRows();
	virtual int  findWordRowsUsingTrieLinks(int haveTall);
	int          findWordColsUsingTrieLinks(int row, int col);

	int			 findWordRowsUsingTrieLinksA(int haveTall);

	int			 findWordRowsUsingTrieLinksOddA(int haveTall);
	int			 findWordRowsUsingTrieLinksEvnA(int haveTall);

	int			 findFirstWordRow();
	int			 findOddRowWantingOddTotal(int haveTall);
	int			 findOddRowWantingEvnTotal(int haveTall);
	int			 findEvnRowWantingOddTotal(int haveTall);
	int			 findEvnRowWantingEvnTotal(int haveTall);

	int			 findNextEvnRow(int haveTall);
	int			 findNextOddRow(int haveTall);
    
	virtual bool isSymmetricSquare() const;
	virtual void printWordRows(const char *wordRows[], int haveTall) const;

protected:
    const int          mOddWide;
    const int          mOddTall;
	const WordTrie<T> &mEvnRowTrie;
	const WordTrie<T> &mEvnColTrie;
	const WordTrie<T> &mOddRowTrie;
	const WordTrie<T> &mOddColTrie;

    rowFinderFn         mOddRowFinder;
    rowFinderFn         mEvnRowFinder;
};

#endif // WordWaffleFinder_hpp