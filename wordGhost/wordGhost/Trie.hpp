// WordTrie.hpp : word prefix tree with forward links (optimized for speed at the expense of memory)
// Sprax Lines, July 2010

#ifndef Trie_hpp
#define Trie_hpp

#include "EvOdNode.hpp"
#include "WordNode.hpp"

template <class NodeT> class Trie {
public:
    Trie<NodeT>() : mRoot(new NodeT(NULL, 0, TrieNode::sEmptyStr)), mAlreadyLoaded(false)
    {}
    Trie<NodeT>(Trie<NodeT> & other);                       // No copy constructor: don't define this.
    Trie<NodeT>& operator=(const Trie<NodeT>&);             // Prevent assigment by not defining this.
   ~Trie<NodeT>() {}                                        // This class owns no heap memory.

    inline const NodeT * getRoot()                 const { return mRoot; }
    inline const NodeT * getFirstWordNode()        const { return mRoot->getFirstWordNode(); }
    
//  inline const NodeT * getBranchAtLetter(int letter)       const { return mRoot->getBranchAtLetter(letter); } // not needed
    inline const NodeT * getFirstWordNodeFromLetter(int ic)  const { return mRoot->getFirstWordNodeFromLetter(ic); }

    inline NodeT *  addWord(const char *word, WordLenT wordLen, NodeT *prevWordNode)	{ 
        return mRoot->addWord(word, wordLen, prevWordNode);
    }
    inline       bool       hasWord(const char *key)        const { return mRoot->hasWord(key); }
    inline const NodeT    * subTrie(const char *key)        const { return (NodeT *)mRoot->subTrie(key); }
    inline const TrieNode * subTrix(const char *k, int x)   const { return mRoot->subTrix(k, x); }  // leave return type TrieNode 

    const NodeT * getNextWordNodeAndIndex(const NodeT *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const;	// fromBeg==index of 1st letter/node that must change

    inline void printWordsAll()			const { mRoot->printWordsAll(); }

    template <class NodeT>
    static int initTrieFromSortedDictionaryFile(Trie<NodeT> *trie, WordMap *wordMap, int minWordLen, const char *fname)
    {   
        // This method allocates memory for holding all the words, and that memory is retained until the process 
        // terminates.  It succeeds only once (even if invoked with different template types or params).
        if (trie->getAlreadyLoaded()) {
            return 0;
        }

        FILE *fi;
        if ( ! fopen_s(&fi, fname, "r") ) {
            printf("Opened %s ... " , fname);
        } else {
            printf("Error opening dictionary file: %s\n", fname);
            return 0;
        }

        NodeT *prevNode = { NULL, };
        char word[BUFSIZE];
        int numWordsRead = 0, numWordsKept = 0, maxWordLen = 0;
        WordLenT wordLenM1;
        while (fgets(word, BUFSIZE, fi))  {
            // got next word as a line in the file; now get its length, and remove any trailing newline.
            WordLenT wordLen = (WordLenT)strlen(word);
            wordLenM1   = wordLen - 1;
            if (word[wordLenM1] < TrieNode::sFirstLetter) {
                word[wordLenM1] = NULL;
                wordLen = wordLenM1;
            }
            numWordsRead++;
            if (minWordLen > wordLen) {
                continue;
            }
            if (maxWordLen < wordLen) {
                maxWordLen = wordLen;
            }
            char *saveWord = _strdup(word);		// Allocate word's memory here, for the life of the program.
            prevNode = trie->addWord(saveWord, wordLen, prevNode);
            if (prevNode == NULL) {
                continue;
            }
            numWordsKept++;
            if (wordMap != NULL) {
                assert(wordLen > 0);
                assert(wordLen <= MAXLEN);
                wordMap->insert(WordMap::value_type(saveWord, numWordsKept));   // strictly increasing value preserves the order
            }
        }
        trie->setAlreadyLoaded(true);
        fclose(fi);
        printf("Read %d words, kept %d, %d <= lengths <= %d\n", numWordsRead, numWordsKept, minWordLen, maxWordLen);
        return maxWordLen;
    }

    bool    getAlreadyLoaded()  const   { return mAlreadyLoaded; }
    void    setAlreadyLoaded(bool done) { mAlreadyLoaded = done; }

private:

    NodeT * mRoot;              // The root node has no parent, and its depth == 0.
    bool    mAlreadyLoaded;     // The trie is already initialized (as from inserting every word from a dictionary).
};

typedef Trie<WordNode>  WordNodeTrie;
typedef Trie<EvOdNode>  EvOdNodeTrie;

#endif // Trie_hpp