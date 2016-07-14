// triesl.cpp : trie w/ single links 

#include "stdafx.h"

const TrieSln* TrieSL::getNextWordNodeRef(const TrieSln *keyNode, const char *keyWord, int & fromBeg, int fromEnd) const	// zoid: use pointer instead of reference for clarity?  fromBeg will be changed!
{
#if 0	// zoid: dumb optimization?  It does not set fromBeg.  We'd need to use n=getNextStemNode, fromBeg = n->depth(), return n->getFirstWordNode.
    if (fromEnd <= 1) {		// 1 means we failed for last column/letter of keyWord; 0 means we failed in a later row...
        assert(keyNode->getWord());			// ...either way, we need the very next word node after keyNode (which is a word node),
        return keyNode->getNextWordNode();	// and word nodes always have their next word node set.
    }
#endif

    // Get the parent of the node containing the first 'bad' letter (zoid: it would be nice if it had been passed in!)
    const TrieSln *parent = NULL;
    if (fromBeg < fromEnd) {
        // Start at the root and find the starting parent by going deeper through children:
        parent = mRoot;
        for (int j = 0; j < fromBeg; j++) {
            parent = parent->branches[keyWord[j] - TrieSln::firstLetter];
        }
    } else {
        // Start at the word and find the starting parent by going up through parents:
        parent = keyNode;
        while (--fromEnd >= 0) {
            parent = parent->getParent();
        }
    }
    assert(parent);

    // ToDo: memoize using nextWordNode.  That is, either change this to start with the offending node
    // and just set/get its nextWordNode (if the offending node is passed in!), 
    // or else replace the word member with a pointer to basically
    // an array of nextWordNodes for each length, so we just set/get/return nextWordNodes[fromBeg],
    // where fromBeg is rationalized to be in [0, wordLength) (if don't have the offending node, and 
    // would have to traverse (as above) to find it!)
#if 1
    while (fromBeg >= 0) {
        const TrieSln *next = parent->getNextBranchFromLetter(keyWord[fromBeg]);
        if (next != NULL) {
    #ifdef _DEBUG
            const TrieSln *nextGetFirstWordNode  = next->getFirstWordNode();
            const TrieSln *theOffendingStemNode = parent->branches[keyWord[fromBeg] - TrieSln::firstLetter];
            assert(theOffendingStemNode);
            const TrieSln *thisGetNextWordNode  = theOffendingStemNode->getNextWordNode();
            if (thisGetNextWordNode != nextGetFirstWordNode) {
                int dorg = 55; // zoid
            }
    #endif
            return next->getFirstWordNode();
        }
        fromBeg--;
        parent = parent->getParent();
    }
#else
    // Here's an attempt at lazy memoization, which violates const, but not conceptual const:
    const TrieSln *node = parent->branches[keyWord[fromBeg] - TrieSln::firstLetter];
    assert(node);   // theOffendingStemNode exists!
    if (node->getNextStemNode() != NULL) {
        return node->getNextWordNode();
    } else {
        while (fromBeg >= 0) {
            for (int iLetter = 1 + keyWord[fromBeg] - TrieSln::firstLetter; iLetter < TrieSln::numLetters; iLetter++) {
                if (node->branches[iLetter] != NULL) {
                    const_cast<TrieSln *>(node)->setNextBranch(node->branches[iLetter]);	// memoizing violates strict (memory) const-ness
                    return node->branches[iLetter]->getFirstWordNode();
                }
            }
            fromBeg--;
            node = node->getParent();
        }
    }
#endif
    //fromBeg = 0;
    return NULL;
}



const TrieSln* TrieSL::getNextWordNodeOld(const TrieSln *keyNode, const char *keyWord, int fromBeg, int fromEnd) const
{
#if 1	// zoid: dumb optimization?
    if (fromEnd <= 1) {		// 1 means we failed for last column/letter of keyWord; 0 means we failed in a later row...
        assert(keyNode->getWord());			// ...either way, we need the very next word node after keyNode (which is a word node),
        return keyNode->getNextWordNode();	// and word nodes always have their next word node set.
    }
#endif

    // Get the parent of the node containing the first 'bad' letter (zoid: it would be nice if it had been passed in!)
    const TrieSln *node = NULL;
    if (fromBeg < fromEnd) {
        // Start at the root and find the starting node by going deeper through children:
        node = mRoot;
        for (int j = 0; j < fromBeg; j++) {
            node = node->branches[keyWord[j] - TrieSln::firstLetter];
        }
    } else {
        // Start at the word and find the starting node by going up through parents:
        node = keyNode;
        for (int j = 0; j < fromEnd; j++) {
            node = node->getParent();
        }
    }
    assert(node);

    // ToDo: memoize using nextWordNode.  That is, either change this to start with the offending node
    // and just set/get its nextWordNode (if the offending node is passed in!), 
    // or else replace the word member with a pointer to basically
    // an array of nextWordNodes for each length, so we just set/get/return nextWordNodes[fromBeg],
    // where fromBeg is rationalized to be in [0, wordLength) (if don't have the offending node, and 
    // would have to traverse (as above) to find it!)
#if 1
    while (fromBeg >= 0) {
        // ToDo: replace trial of each index with link-following...  
        for (int iLetter = 1 + keyWord[fromBeg] - TrieSln::firstLetter; iLetter < TrieSln::numLetters; iLetter++) {
            if (node->branches[iLetter] != NULL) {
                return node->branches[iLetter]->getFirstWordNode();
            }
        }
        fromBeg--;
        node = node->getParent();
    }
#elif 1
    node = node->branches[keyWord[fromBeg] - TrieSln::firstLetter];
    assert(node);
    node = node->getNextStemNode();
    if (node) {
        node = node->getFirstWordNode();
    }
    return node;
#else
    // Here's an attempt at lazy memoization, which violates const, but not conceptual const:
    if (node->getNextBranch() != NULL) {
        return node->getNextBranch()->getFirstWordNode();
    } else {
        while (fromBeg >= 0) {
            for (int iLetter = 1 + keyWord[fromBeg] - TrieSln::firstLetter; iLetter < TrieSln::numLetters; iLetter++) {
                if (node->branches[iLetter] != NULL) {
                    const_cast<TrieSln *>(node)->setNextBranch(node->branches[iLetter]);	// memoizing violates strict (memory) const-ness
                    return node->branches[iLetter]->getFirstWordNode();
                }
            }
            fromBeg--;
            node = node->getParent();
        }
    }
#endif
    return NULL;
}








static void initFromFile(TrieSL tries[], MapSL maps[], const char *fname)
{
    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r" )) {
        printf("Opened %s\n" , fname);
    } else {
        printf( "Error opening %s; exiting.\n", fname );
        return;
    }

    char word[MAXLEN];
    for (int q = 0; fgets(word, MAXLEN, fi); q++)  {
        // get next word as a line in the file, get its length while terminating it on any non-alphabetic char

        int length = strlen(word);
        if (word[length - 1] < TrieSln::firstLetter) {
            word[length - 1] = NULL;
            length = length - 1;
        }
        char *saveWord = _strdup(word);
        tries[length].insertWord(saveWord);
        maps[length].insert(MapSL::value_type(saveWord, length + q));
    }

    //// Iterate over the map and print out all key/value pairs.
    //for(MapSL::const_iterator it = maps[6].begin(); it != maps[6].end(); ++it)
    //{
    //    cout << " key = first): " << it->first;
    //    cout << " (value = second): " << it->second << '\n';
    //}

    return;
}


void test_trie_insert_print()
{
    //TrieSln *root = new TrieSln();
    TrieSL aTrie;
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
    const TrieSln *node = aTrie.subTrie("an");
    node->printWordsAll();

    TrieSL tries[MAXLEN];
    MapSL  maps[MAXLEN];
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



//static int findWordRectRowsOpto(  int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL maps[]
//, char wordRows[MAXLEN][MAXLEN], char wordCols[MAXLEN][MAXLEN])
//{
//    TotalCalls++;
//
//    if (haveTall == wantTall) {
//        printWordRect(wordRows, wantWide, haveTall, 1);
//        return wantWide*wantTall;
//    }
//
//    MapSL&  rowMap = maps[wantWide];
//    TrieSL& rowTri = tries[wantWide];
//    TrieSL& colTri = tries[wantTall];
//
//    TrieSln *node = rowTri.getRoot();
//    for(;;) {
//START:
//        if (node == NULL) {
//            return 0;
//        }
//        const char *word = node->firstWordMemo();
//        for (int k = 0; k < wantWide; k++) {
//            wordCols[k][haveTall] = word[k];
//            wordCols[k][haveTall + 1] = NULL;
//            if ( ! colTri.subTrie(wordCols[k]) ) {
//                node = rowTri.nextNode(word, k);
//                goto START;
//            }
//        }
//        strcpy_s(wordRows[haveTall], MAXLEN, word);
//        int area = findWordRectRowsOpto(wantWide, wantTall, haveTall+1, tries, maps, wordRows, wordCols);
//        if (area > 0) {
//            return area;
//        }
//        node = rowTri.nextNode(word, wantWide);
//    }
//    return 0;
//}
