// TrieNode.cpp : trie nodes for words

#include "stdafx.h"
#include "BoggleNode.hpp"

WordTrie *BoggleNode::sWordTrie;

static int sgDbg = 0;   // TODO: delete this

void BoggleNode::setNeighborsFromRowCol(BoggleNode **boardNodes[], int row, int col, int maxRowIndex, int maxColIndex)
{
    int minRow = (row == 0) ? 0 : row-1;
    int maxRow = (row == maxRowIndex) ? maxRowIndex : row+1;
    int minCol = (col == 0) ? 0 : col-1;
    int maxCol = (col == maxColIndex) ? maxColIndex : col+1;
    BoggleNode *node;
    for (row = minRow; row <= maxRow; row++) {
        for (col = minCol; col <= maxCol; col++) {
            node = boardNodes[row][col];
            if (node != this) {
                mNeighbors[mNumNeighbors++] = node;
            }
        }
    }
}


void BoggleNode::findWords(BoggleWordMap &boardWords)
{
    BoggleNode *path[WordTrie::sMaxWordLen];    // TODO: MIN(sMaxWordLen, BoardSize)
    char str[WordTrie::sMaxWordLen] = { NULL };
    int length = 0;
    findWordsRecursive(boardWords, mWordNodes, path, sWordTrie->getRoot(), str, length);
}

void BoggleNode::findWordsRecursive(BoggleWordMap &boardWords, WordNodeHashSet &wordNodes, BoggleNode *path[], const TrieNode *prevNode, char str[], int length)
{
    static int dbg = 0;

    // Store the location of end of the path, then look for duplicates by retracing the path backwards.
    // If the average path length could get long, a hash_set or map might be more efficient.
    // As it is, the path length cannot be greater than the max word length or the number of BoggleNodes in board.
    BoggleNode **endPath = &path[length];  
    for (BoggleNode **p = endPath; p != path; ) {
        if (*--p == this)	  // this node is already in this path
            return;
    }
    // Retracing forward is likely to be less efficient at finding duplicates
    // for (BoggleNode **p = path; p != endPath; p++) {
    //    if (*p == this)	  // this node is already in this path
    //        return;
    // }      

    str[length] = mLetter;
    length++;
    str[length] = NULL;
    const TrieNode *nextNode = prevNode->getBranchAtLetter(mLetter);
    //const TrieNode *stemNode = sWordTrie->getStemNode(str);
    //if (stemNode != nextNode)
    //    dbg++;
    int freq = 0;
    if (nextNode != NULL) {
        const char *word = nextNode->getWord();
        if (word != NULL) {
            if (sgDbg > 1) {
                printf("findWords: %s \tis a WORD -- inserting it.\n", word);
            }
            wordNodes.insert(nextNode);
            if (boardWords.find(word) != boardWords.end()) {        // word already found at least once
                boardWords[word] = 1 + boardWords.at(word);         // increment the count
            }
            else {
                boardWords.insert(WordMap::value_type(word, 1));    // insert in map for the first time
            }
        } else if (sgDbg > 2) {
            const char *stem = nextNode->getStem();
            printf("findWords: %c", stem[0]);
            for (int j = 1; j < length; j++)
                printf("%c", stem[j]);
            printf("\tis a stem, but not a word.  Try adding neighbors.\n");
        }
        *endPath = this;
        for(int j = 0; j < mNumNeighbors; j++) {
            mNeighbors[j]->findWordsRecursive(boardWords, wordNodes, path, nextNode, str, length);
        }
    } else {
        if (sgDbg > 3) {
            printf("findWords: %s \tis not evem a stem.  Dead end.\n", str);
        }
        return;
    }
}

void BoggleNode::findWordsRecursive_test()
{
    BoggleWordMap foundWords;
    findWords(foundWords);

    printf( "nodesB %2d:\n", mWordNodes.size());
    int num = 0;
    for each( const TrieNode *node in mWordNodes ) {
        printf( "    %2d: %s\n", ++num, node->getWord());
    }
}

