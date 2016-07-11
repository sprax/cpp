// TrixNode.cpp : trie nodes for words
// Sprax Lines,  July 2010

#include "TrixNode.hpp"
//#include "CharFreqMap.hpp"

//bool TrixNode::isSubKeyStem(const char *subKey, int subKeyLen, const TrixNode* lastStemNode) const	// deprecated: flexible but tricky to call
//{
//    //  subKey may be the address of a letter in the interior of a key; if so, 'this' must correspond to the letter at that address.
//    //  Follow the mBranches for each letter in subKey, starting from this, 
//    //      return true if key is a valid word stem, or false if it is not, 
//    //      and set lastStemNode to be the node of the last letter that is part of a valid stem.
//
//    assert(subKey && *subKey == mStem[mDepth]);		// The letter starting subKey must match letter at this node.
//    const TrixNode * node = lastStemNode = this;
//    while (subKeyLen >= 0) {
//        node = node->mBranches[*subKey++];
//        if ( ! node ) {
//            return false;
//        }
//        lastStemNode = node;
//        --subKeyLen;
//    }
//    return true;
//}

//const TrixNode* TrixNode::subTrix(const char *key, int idx) const		// deprecated, doesn't say where the key failed
//{
//    //  Follow the mBranches for each letter in the key up the specified index, and:
//    //      return the last node, if key is a valid word stem, or 
//    //      return NULL, if key is not a stem for any word in the trie.
//    const TrixNode *node = this;
//    do {
//        node = node->mBranches[(uchr)*key++];
//    } while (node != NULL && --idx >= 0);
//    return node;
//}

//const TrixNode* TrixNode::subTrie(const char *nullTerminatedKey) const	// deprecated: depends on null-termination, loses info.
//{
//    //  Follow the mBranches for each letter in key, and:
//    //      return the last node, if key is a valid word stem, or 
//    //      return NULL, if key is not a stem for any word in the trie.
//    //  The key must be null-terminated, and we throw away the information of where the key fails.
//    const TrixNode *node = this;
//    for (const char *pc = nullTerminatedKey; *pc != NULL; pc++) {
//        uchr ucx = *pc;
//        node = node->mBranches[ucx];
//        if (node == NULL) {
//            return  NULL;			// Not found - reached end of branch
//        }
//    }
//    return node;
//}
//
//void TrixNode::printWordsAll() const
//{
//    if (mWord != NULL) {
//        printf("%s\n", mWord);
//    }
//    for (const TrixNode *node = firstBranch; node != NULL; node = node->nextBranch) {
//        node->printWordsAll();
//    }
//}

/*****************************************
class Out {
    static const int maxSize = 8;
    static       int minSize;
    int              theSize;

    class In {
        friend class Out;                   // this is the key
        static const int inMaxSize = 8;
        static       int inMinSize;
        int              inTheSize;

        In(Out out) {
            printf("Out sizes: %d %d %d   In sizes: %d %d %d\n"
                , maxSize, minSize, out.theSize
                , inMaxSize, inMinSize, inTheSize);
        }
    };

    int printSizes(In in) {
        return printf("Out sizes: %d %d %d   In sizes: %d %d %d\n"
                , maxSize, minSize, theSize
                , in.inMaxSize, in.inMinSize, in.inTheSize);
    }

};
***********************************/
/************************
//#include "WordTrie.hpp"
#include "CharMap.hpp"

template <typename T>
int TrixNode::testTrixNode(const WordTrie<T> &trie, const CharMap &charMap)
{
	TrixNode *trixNode = new TrixNode(charMap, NULL, 0);
	delete    trixNode;
	return 0;
}
 ************************/
