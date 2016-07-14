// BoggleBoard.cpp : 

#include "StdAfx.h"
#include "BoggleBoard.hpp"

WordTrie BoggleBoard::sWordTrie;
int      BoggleBoard::sWordTrieInitialized = 0;

BoggleBoard::BoggleBoard(const char *rows[], int nrows, int ncols, const char *fileName) 
    : mNumRows(nrows), mNumCols(ncols)
{
    // initialized statics
    initWordTrieFromFile(&sWordTrie, 3, fileName);
    BoggleNode::sWordTrie = &sWordTrie;

    // 1st pass: create all tne nodes.
    if (sbUseMalloc) {
        mMem   = (BoggleNodePtr*) malloc(mNumRows*mNumCols*sizeof(BoggleNode*));
        mNodes = (BoggleNodePtr**)malloc(mNumRows*sizeof(BoggleNode**));
    } else {
        mMem   = new BoggleNode* [mNumRows*mNumCols];
        mNodes = new BoggleNode**[mNumRows];
    }
    for (int row = 0; row < mNumRows; row++) {
        mNodes[row] = &mMem[row*mNumCols];
        for (int col = 0; col < mNumCols; col++) {
            mNodes[row][col] = new BoggleNode(rows[row][col]);
        }
    }
    // 2nd pass: fill in all the nodes' neighbor arrays with pointers to the created nodes.
    for (int row = 0; row < mNumRows; row++) {
        for (int col = 0; col < mNumCols; col++) {
            setNeighborsFromRowCol(row, col);
        }
    }
}

BoggleBoard::~BoggleBoard() {
    for (int j = mNumRows*mNumCols; --j >= 0; )
        delete mMem[j];
    if (sbUseMalloc) {
        free(mNodes);
        free(mMem);
    } else {
        delete [] mNodes;
        delete [] mMem;
    }
}

void BoggleBoard::setNeighborsFromRowCol(int row, int col)
{
    int maxRowIndex = mNumRows - 1;
    int maxColIndex = mNumCols - 1;
    int minRow = (row == 0) ? 0 : row-1;
    int maxRow = (row == maxRowIndex) ? maxRowIndex : row+1;
    int minCol = (col == 0) ? 0 : col-1;
    int maxCol = (col == maxColIndex) ? maxColIndex : col+1;
    BoggleNode *node = mNodes[row][col];
    node->mNumNeighbors = (maxRow - minRow + 1)*(maxCol - minCol + 1) - 1;

    // Allocate the array of neighbors; make it just big enough to hold the actual neighbors
    if (sbUseMalloc) {
        node->mNeighbors = (BoggleNodePtr*)malloc(node->mNumNeighbors*sizeof(BoggleNodePtr));
    } else {
        node->mNeighbors = new BoggleNodePtr[node->mNumNeighbors];
    }
    int numNeighbors = 0;
    for (row = minRow; row <= maxRow; row++) {
        for (col = minCol; col <= maxCol; col++) {
            BoggleNode *neighbor = mNodes[row][col];
            if (neighbor != node) {
                node->mNeighbors[numNeighbors++] = neighbor;
            }
        }
    }
}

int BoggleBoard::initWordTrieFromFile(WordTrie *wordTrie, int minWordLen, const char *fname)
{
    if (sWordTrieInitialized)
        return 0;     // TODO wordTrie->getNumWords();
    sWordTrieInitialized = true;

    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r" )) {
        printf("Opened %s\n" , fname);
    } else {
        printf( "Error opening %s; exiting.\n", fname );
        return 0;
    }

    char word[BUFSIZE];
    int numWords;
    for (numWords = 0; fgets(word, BUFSIZE, fi); numWords++)  {
        // get next word as a line in the file, get its length while terminating it on any non-alphabetic char

        int length = strlen(word) - 1;
        if (length < minWordLen)
            continue;
        word[length] = NULL;            // Remove the newline (replace it with NULL)
        char *saveWord = _strdup(word);
        wordTrie->insertWord(saveWord);
    }
    printf("Loaded %d words from dictionary: %s\n", numWords, fname);
    return numWords;
}




void BoggleBoard::printBoard()
{
    for (int row = 0; row < mNumRows; row++) {
        for (int col = 0; col < mNumCols; col++) {
            printf(" %c", mNodes[row][col]->mLetter);
        }
        printf("\n");
    }
}

void BoggleBoard::findWordsAtNode(int row, int col)
{ 
    BoggleNode &boggleNode = *mNodes[row][col];
    boggleNode.findWords(mWords);
    showWordsAtNode(row, col);
}

void BoggleBoard::showWordsAtNode(int row, int col)
{
    BoggleNode &boggleNode = *mNodes[row][col];
    printf( "\nFound %2d words originating from node %d at (%d, %d):\n", boggleNode.mWordNodes.size()
        , (row * mNumCols + col + 1), row, col);
    int num = 0;
    for each( const TrieNode *wordNode in boggleNode.mWordNodes ) {
        printf( "\t%s", wordNode->getWord());
        if (++num % 5 == 0)
            printf("\n");
    }
    printf("\n");
}

int BoggleBoard::findAndPrintAllWords(const char *rowChars[], int numRows, int numCols)
{
    BoggleBoard board(rowChars, numRows, numCols);  // TODO: should not create new board
    printf("The Boggle Board:\n");
    board.printBoard();

    //BoggleNode &node = *board.mNodes[0][0];
    //node.clearWords();

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            board.findWordsAtNode(row, col);    // TODO: this call should take in sWordTrie
        }
    }
    printf("\n\n");
    board.printBoard();
    printf("All %d unique words on the board:\n", board.mWords.size());
    int num = 0;
    for each(std::pair<const char*, unsigned int> pair in board.mWords ) {
        printf( "\t%8s", pair.first);      // TODO: 8 + TABLENGHT should == maxFoundWordLen
        if ( ! strcmp(pair.first, "bap")) {
            static int zoid = 1;
            zoid++;
        }
        if (pair.second > 1)
            printf("(%d)", pair.second);
        if (++num % 5 == 0)
            printf("\n");
    }
    printf("\n");
    return 0;
}

static const int sgMaxLength = 32;
static const int sgNumRows = 4, sgNumCols = 4;
static const char *sgRows[sgNumRows] = 
{ "dibe"
, "iaml"
, "nbpe"
, "cors"
};

 

int BoggleBoard::unit_test() 
{
    findAndPrintAllWords(sgRows, sgNumRows, sgNumCols);
    return 0;
}