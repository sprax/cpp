//  wordRectMain.cpp : find largest dense rectangles of words from a dictionary, multi-threaded
//  Sprax Lines, July 2010

//  TODO: Optimization: index TrieNode::mBranches from 'a' instead of 0, eliminating the need
//  to subtract 'a' from a character to get its offset into the array.
//
//  TODO: Minimal-space version.  If we only load the trie from a sorted word list,  we could 
//  forego the use of character values as "random access" offsets into a pre-allocated array of all 
//  possible branches.  Instead, we could represent the branches using only a linked list or even a 
//  packed array.  To verify that "quon" is a valid prefix, we'd then have to do 4 list traversals 
//  or 4 small array searches instead of going directly to the array entry by its using 'q', 'u', 'o', 
//  and 'n' directly as indices.  For big searches, that seems a terrible trade-off.  
//  For spell-checking words only as quickly as they are entered from a cell-phone keyboard,
//  it may be fine.

#include "stdafx.h"



/******************************************
#include "WordRectSearchMgr.hpp"

// default values:
static const char *defDictionaryFile = "WORD.LST.txt";
static const char *defProgramName  = "wordRectMulti";
static const int   defMinTall = 2;
static const int   defMaxTall = MAXLEN;
static const int   defMinArea = 4;
static const int   defMaxArea = MAXLEN*MAXLEN;


static void usage(int argc, char* argv[], const char *reason=NULL)
{
    printf( "Usage: %s [-dhmqs] [dictionary] [minArea minHeight maxHeight maxArea]\n", defProgramName);
    printf( "All arguments are optional, but if present, must be in the order above.\n"
            "The defaults are the minimum and maximum values the program accepts:\n"
            "    %d %d %d %d.\n", defMinArea, defMinTall, defMaxTall, defMaxArea);
    printf( "This tells the program to search for word rectangles of the dimensions between\n"
            "%d x %d (or area %d) and %d x %d (or area %d),\n"
            , defMinTall, defMinTall, defMinArea, defMaxTall, defMaxTall, defMaxArea);
    printf( "using tries check column words to find the next candidate for each word row.\n\n"
            "The letter options are:\n"
            "d  Use the next argument for the dictionary file name (instead of %s\n)", defDictionaryFile);
    printf( "h  Show this help message\n"
            "m  Use std::map::upper_bound instead of linked tries to find rows (much slower)\n"
            "q  Quite mode.  Suppress most of the messages about starting and ending finders (threads)\n");
    printf( "s  Order the search for word rectangles from smaller to larger (slower but fun)\n"
            "The default is to order the search by decreasing area, so it can end when the"
            "first (largest) is found.\n");

    printf( "Exiting from this command:\n    ");
    for (int j = 0; j < argc; j++) {
        printf("%s ", argv[j]);
    }
    printf("\n");
    if (reason) {
        printf("Reason: %s\n", reason);
    }
    exit(0);
}


bool nextWideTall_A(int &wide, int &tall, const int &minWide, const int &minTall, const int &maxWide, const int &maxTall, bool asc);
int  nextWideTall_B(int &wide, int &tall, const int &minTall, const int &maxTall, const int &minArea, const int &maxArea, const int& maxWide, bool ascending);

int main(int argc, char* argv[])
{   
    // set/get default args, check actual args and either show usage or call wordRect
    int  minArea = 4;
    int  minTall = 2;
    int  maxTall = MAXLEN;
    int  maxArea = MAXLEN*MAXLEN;
    int  tempInt = 0;
    bool findSmallerFirst = false;
    bool verbosity = true;
    bool useMaps   = false;    // if true, use std::maps (slower by a factor of 3 or more)

    const char *programName  = argv[0] ? argv[0] : defProgramName;
    const char *dictFileName = defDictionaryFile; 
    char reason[BUFSIZE];
    int j = 1;
    if (j < argc && argv[j][0] == '-') {   // [-letters] options?
        for (const char *pc = argv[j]; *++pc != NULL; ) {
            switch (*pc) {
            case 'd' :
                if (argv[++j] != NULL) {
                    dictFileName = argv[j];
                }
                break;
            case 'h' : 
                usage(argc, argv, "help was requested (-h)");
                break;
            case 'm' :
                useMaps = true;
                break;
            case 'q' :
                verbosity = false;
                break;
            case 's' :
                findSmallerFirst = true;
                break;
            default:
                sprintf_s(reason, BUFSIZE, "got unknown option: %c", *pc);
                usage(argc, argv, reason);
                break;
            }
        }
        j++;
    }
    if (j < argc) {
        tempInt = atoi(argv[j++]);
        if (tempInt < minArea) {
            sprintf_s(reason, BUFSIZE, "minArea %d < %d", tempInt, minArea);
            usage(argc, argv, reason);
        }
        minArea = tempInt;
    }
    if (j < argc) {
        tempInt = atoi(argv[j++]);
        minTall = tempInt > minTall ? tempInt : minTall;
    }
    if (j < argc) {
        tempInt = atoi(argv[j++]);
        maxTall = tempInt < maxTall ? tempInt : maxTall;
    }
    if (j < argc) {
        tempInt = atoi(argv[j++]);
        maxArea = tempInt < maxArea ? tempInt : maxArea;
    }
    printf("%s: using %s,\n", programName, (useMaps ? "maps" : "tries"));
    printf("   min & max Area: %d %d, min & max Height: %d %d.\n", minArea, maxArea, minTall, maxTall);
    if (minArea > maxArea || minTall > maxTall) {
        usage(argc, argv, "minArea > maxArea or minHeight > maxHeight");
    }

    WordRectSearchMgr& wordRectSearcher = WordRectSearchMgr::getInstance();
    wordRectSearcher.setOptions(findSmallerFirst, verbosity, useMaps);
    wordRectSearcher.searchForLargestWordRect(dictFileName, minArea, minTall, maxTall, maxArea);
    return 0;
}

******************************************************/


