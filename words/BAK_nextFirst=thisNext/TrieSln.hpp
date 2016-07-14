#ifndef triesln_hpp
#define triesln_hpp

#define USE_LAST 1

const int MAXLEN = 32;


struct ltstr
{
  inline bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

typedef std::map<const char*, int, ltstr> MapSL;
#define USE_BRANCHES_ON_STACK 1

class TrieSln		// final, for now.  No destructor needed, for now.
{
public:
    static const unsigned int numLetters  =  26;
    static const unsigned int firstLetter = 'a';
    static const unsigned int lastLetter  = 'z';

#if USE_BRANCHES_ON_STACK
	TrieSln  *branches[numLetters]; // all possible continuations...
#else
    TrieSln **branches; // all possible continuations, or branches (call null continuations "buds", non-null ones "branches") // zoid: protect
#endif

protected:
    const TrieSln* firstBranch;	// First non-NULL branch is the first node in the branches as a linked list
    const TrieSln* nextBranch;	// Assumes fixed length; otherwise, the next branch depends on the desired word length
    TrieSln* lastBranch;	    // Last non-NULL branch is the last node in the branches as a linked list (no sentinal; non-const because it changes during init)
	TrieSln* parent;            // One level up in the trie tree is the node representing this stem's "prefix" (the string w/o last this one's last letter)

    const char *word;   // The word, if this is a word node; NULL, if it is not a word node.  Used as a boolean and for printing.
    const char *stem;   // Some word that completes this stem; only the first N chars matter, where N == depth.
	const int  depth;   // level in the tree.  Note that stem[depth] is this node's letter.  
    int   words;        // number of words that include this node in their stems    // zoid: don't need this.
  //int   index;	    // index of letter in the implicit char array               // zoid
    TrieSln * firstWordNode;	// node of first word that completes this one's stem (this, if this is a word node)
    TrieSln * nextStemNode;	    // node of first stem after this one 
    TrieSln * nextWordNode;	    // node of first word that does not start with this one's stem.  Examples: 
								// For stem a, first word is aah, next stem is b, and next word is baa (not aba or aal); 
								// For ac, first word is ace, next stem is ad, and next word is add (not act);
								// For stem ace, first word is ace (itself), and the next stem & next word are the same: act.
								// zoid: which is more useful, nextWordNode, or nextStemNode?  Or do we really want nextStemNode(idx)?
public:
    TrieSln(TrieSln *parent, int depth) : firstBranch(NULL), nextBranch(NULL), lastBranch(NULL), parent(parent)
		, word(NULL), stem(NULL), depth(depth), words(0), firstWordNode(NULL), nextStemNode(NULL), nextWordNode(NULL)
    {
        // Beware of platform-dependent behavior performance: 
        // typedef int * pint;
        // int **arrayA = new pint[numLetters];  memset(arrayA, 0, numLetters*sizeof(int*));   // may reduce cache misses
        // int **arrayB = (pint*)calloc(numLetters, sizeof(int*));                             // may enable lazy memory mapping
#if USE_BRANCHES_ON_STACK
		memset(branches, 0, numLetters*sizeof(TrieSln*));
#else
        branches = static_cast<TrieSln**>(calloc(numLetters, sizeof(TrieSln*)));
#endif
    };

    inline const char    * getWord()			const { return word; }		// NULL, if this is not a word-node
	inline const TrieSln * getFirstWordNode()	const { return firstWordNode; }	
	inline const TrieSln * getNextWordNode()	const { return nextWordNode; }
	inline const TrieSln * getNextBranchFromLetter(char letter) const { return branches[letter - firstLetter]->nextBranch; }
//	inline const TrieSln * getNextBranch()      const { return nextBranch; }
 	inline void  setNextBranch(const TrieSln* next)   { nextBranch = next; }
	inline TrieSln * getParent()				const { return parent; }

	inline const TrieSln * getNextStemNode()    const {		// Note: from the next stem, you can get the depth; then get its firstWordNode == the desired nextWordNode
        if (nextStemNode != NULL) {
            return nextStemNode;
        }
		if (word != NULL) {			// if this is a word node,
			return nextWordNode;	// then the next stem is the next word, which is always known.
		}
		for (TrieSln *parent = getParent(); parent != NULL; parent = parent->getParent()) {
			for (int iLetter = 1 + stem[parent->depth] - firstLetter; iLetter < numLetters; iLetter++) {
				if (parent->branches[iLetter] != NULL) {
					return parent->branches[iLetter];
				}
			}
		}
		return NULL;
	}

    inline bool hasWord(const char *key) const
    {
        // Traverse down the trie until the end of string is found
        const TrieSln *node = this;
        for (const char *p = key; *p != NULL; p++) {
            node = node->branches[*p - firstLetter];
            if (node == NULL) {
                return  false;			// Not found - reached end of branch
            }
        }
        return (node->word != NULL);
    }

    TrieSln* subTrie(const char *nullTerminatedKey);	// deprecated: needs null-termination, loses info.
	TrieSln* subTrix(const char *key, int idx);			// deprecated: loses info of where the key fails.
    bool isSubKeyStem(const char *subKey, int subKeyLen, const TrieSln* lastStemNode) const;	// deprecated: flexible but tricky to call

//  TrieSln * insertWord(const char *word);        					// insert word with no link to prev or next word
	TrieSln * insertWord(const char *word, TrieSln *prevWordNode);	// insert word and set prev word node's nextWord link to this word's node
		
	void printWordsAll() const;
	void printNextTree() const;
	void printNextWordMemo();
	void printWordsOfLength(int length) const;
};

#endif // triesln_hpp