#ifndef BoggleNode_hpp
#define BoggleNode_hpp

#include "WordTrie.hpp"

typedef std::hash_set<const TrieNode *>    WordNodeHashSet;

class    BoggleNode;                    // forward declaration
typedef  BoggleNode * BoggleNodePtr;
typedef  WordMap      BoggleWordMap;    // Sorted keys, based on Map.  WordHashMap would give unsorted keys.

class BoggleNode
{
public:
    static WordTrie *sWordTrie;
    static const int mMaxNumNeighbors = 8;

    BoggleNodePtr  *mNeighbors;     // pointer to any and all neighbor nodes
    int			    mNumNeighbors;	// actual count of neighbor nodes
    char		    mLetter;        // letter value of this node
    WordNodeHashSet mWordNodes;     // the set of all words found on the boggle board, starting at this node

    BoggleNode(char letter) : mNumNeighbors(0), mLetter(letter) {}
    ~BoggleNode()           { delete [] mNeighbors; }

    void clearWords() { mWordNodes.clear(); }

    void setNeighborsFromRowCol(BoggleNode **boardNodes[], int row, int col, int maxRowIndex, int maxColIndex);

    void findWords(BoggleWordMap &boardWords);
    void findWordsRecursive(BoggleWordMap &boardWords, WordNodeHashSet &wordNodes, BoggleNode *path[], const TrieNode *prevNode, char str[], int length); // TODO: don'd need all this
    void findWordsRecursive_test();
};

#endif // BoggleNode_hpp