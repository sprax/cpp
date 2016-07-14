#ifndef BoggleBoard_hpp
#define BoggleBoard_hpp

#include "BoggleNode.hpp"

class BoggleBoard
{
protected:
    static WordTrie   sWordTrie;
    static int        sWordTrieInitialized;
    static const bool sbUseMalloc = false;

public:
    int mNumRows;
    int mNumCols;
    BoggleNode ***mNodes;
    BoggleWordMap mWords;

    BoggleBoard(const char *rows[], int nrows, int ncols, const char *fileName="WORD.LST.txt");
   ~BoggleBoard();

    void printBoard();
    void findWordsAtNode(int row, int col);
    void showWordsAtNode(int row, int col);
    void showWordsAll(void);

    static int  initWordTrieFromFile(WordTrie *wordTrie, int minWordLen, const char *fname);
    static int findAndPrintAllWords(const char *rows[], int numRows, int numCols);
    static int BoggleBoard::unit_test();

protected:
    void setNeighborsFromRowCol(int row, int col);  // called only by constructor

private:
    BoggleNode **mMem;
};

class BoggleBoard4x4 : public BoggleBoard
{
public:
    BoggleBoard4x4(const char *rows[], const char *fileName) 
        : BoggleBoard(rows, 4, 4, fileName) {}
};

#endif // BoggleBoard_hpp