// triesl.cpp : trie w/ single links 

#include "TrieTree.hpp"

const TrieNode* TrieTree::getNextWordNodeAndIndex(const TrieNode *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const // fromBeg can change
{
   // We could memoize lazily using nextWordNode.  That is, either change this 
	// to start with the offending node and just set/get its nextWordNode (if the 
	// disqualified node is passed in!), or else replace the word member with a pointer to basically
    // an array of nextWordNodes for each length, so we just set/get/return nextWordNodes[fromBeg],
    // where fromBeg is rationalized to be in [0, wordLength).  But if we already have the offending
	// node, so we don't have to traverse (as below) to find it (or rather, its parent), then getting 
	// the next different stem is so simple that we don't really need to make a separate method for it.  
	// As it is, we don't memoize lazily, we just do it once and for all in insertWord.

	// Anyway, here is the mon-memoized code, used as a reference implementation before optimizing
	// all needed traversal using links:

	// Get the parent of the node containing the first 'bad' letter (if we already know it, we don't need to use this method).
    const TrieNode *parent = NULL;
    if (fromBeg < fromEnd) {
        // Start at the root and find the starting parent by going deeper through children:
        parent = mRoot;
        for (int j = 0; j < fromBeg; j++) {
            parent = parent->getBranchAtLetter(keyWord[j]);
        }
    } else {
        // Start at the word and find the starting parent by going up through parents:
        parent = keyNode;
        while (--fromEnd >= 0) {
            parent = parent->getParent();
        }
    }
    assert(parent);

    while (fromBeg >= 0) {
        const TrieNode *next = parent->getNextBranchFromLetter(keyWord[fromBeg]);
        if (next != NULL) {
            return next->getFirstWordNode();
        }
        fromBeg--;
        parent = parent->getParent();
    }
	fromBeg = 0;
    return NULL;
}

static void initFromFile(TrieTree tries[], WordMap maps[], const char *fname)
{
    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r" )) {
        printf("Opened %s\n" , fname);
    } else {
        printf( "Error opening %s; exiting.\n", fname );
        return;
    }

    char word[BUFSIZE];
    for (int q = 0; fgets(word, BUFSIZE, fi); q++)  {
        // get next word as a line in the file, get its length while terminating it on any non-alphabetic char

        int length = strlen(word);
        if (word[length - 1] < TrieNode::firstLetter) {
            word[length - 1] = NULL;
            length = length - 1;
        }
        char *saveWord = _strdup(word);
        tries[length].insertWord(saveWord);
        maps[length].insert(WordMap::value_type(saveWord, length + q));
    }

    //// Iterate over the map and print out all key/value pairs.
    //for(WordMap::const_iterator it = maps[6].begin(); it != maps[6].end(); ++it)
    //{
    //    cout << " key = first): " << it->first;
    //    cout << " (value = second): " << it->second << '\n';
    //}

    return;
}


void test_trie_insert_print()
{
    //TrieNode *root = new TrieNode();
    TrieTree aTrie;
    char *sc;

    aTrie.insertWord("an");
    aTrie.insertWord("a");
    aTrie.insertWord("and");
    aTrie.insertWord("agro");
    aTrie.insertWord("ant");
    aTrie.insertWord("answer");
    aTrie.insertWord("arbitrary");
    aTrie.insertWord("anded");
    aTrie.insertWord("nand");
    aTrie.insertWord("nothing");
    aTrie.insertWord("nope");
    aTrie.insertWord("nag");
    aTrie.insertWord("zoo");
    aTrie.insertWord("zoology");
    aTrie.insertWord("zoetrope");
    sc = "and";     printf("isWord(%s) =>\t %s\n", sc, aTrie.hasWord(sc) ? "true" : "false");
    sc = "andy";    printf("isWord(%s) =>\t %s\n", sc, aTrie.hasWord(sc) ? "true" : "false");
    sc = "nand";    printf("isWord(%s) =>\t %s\n", sc, aTrie.hasWord(sc) ? "true" : "false");
    sc = "nan";     printf("isWord(%s) =>\t %s\n", sc, aTrie.hasWord(sc) ? "true" : "false");
    const TrieNode *node = aTrie.subTrie("an");
    node->printWordsAll();

    TrieTree tries[MAXLEN];
    WordMap  maps[MAXLEN];
    initFromFile(tries, maps, "WORD.LST.txt");

    aTrie.subTrie("n")->printWordsAll();
    unsigned int len = 4;
    printf("====Words of length %d====\n", len);
    aTrie.subTrie("")->printWordsOfLength(len);

    len = 11;
    printf("====Words of length %d==== printNext:\n", len);
    tries[len].subTrie("")->printNextWordMemo();
    tries[len].subTrie("")->printNextTree();
    tries[len].subTrie("")->printNextWordMemo();
}
