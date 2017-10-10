//  wordRectMain.cpp : find largest dense rectangles of words from a dictionary, multi-threaded
//  Sprax Lines, July 2010
//
//  Word Rectangle (ITA Software hiring puzzle)
//  
//  Write a program to find the largest possible rectangle of letters such 
//  that every row forms a word (reading left to right) and every column 
//  forms a word (reading top to bottom). Words should appear in this 
//  dictionary: WORD.LST (1.66MB). Heuristic solutions that may not always 
//  produce a provably optimal rectangle will be accepted: seek a reasonable 
//  trade-off of efficiency and optimality. 

//  TODO: Define a base class for all tries based on TrieNode or its derived classes?
//  TODO: Put previously static info in base class accessors (read-only)
//  TODO: Make an option to find at least one rect in each possible wide-tall pair

#include "wordPlatform.h"
#include "wordRectMain.hpp"
#include "WordRectSearchExec.hpp"

#include <vector>
#include <algorithm>
#include <functional>      // For greater<int>( )
#include <iostream>

// default values:
static const char *defDictFileName = "../WORD.LST.txt"; // "EnuWords.txt";
static const char *defProgramName  = "wordRectMulti";
static const int   defMinTall = 2;
static const int   defMaxTall = MAXLEN; // TODO: remove
static const int   defMinArea = 4;
static const int   defMaxArea = MAXLEN*MAXLEN;
//static time_t      gWordRectMainSearchStartTime = 0;

static void usage(int argc, const char* argv[], const char *reason=NULL)
{
    printf( "\n  Usage: %s [-dhmqs] [dictionary] [minArea minHeight maxHeight maxArea]\n", defProgramName);
    printf( "All arguments are optional, but if present, must be in the order above.\n"
            "  The defaults are the minimum and maximum values the program accepts:\n"
            "    %d %d %d %d.\n", defMinArea, defMinTall, defMaxTall, defMaxArea);
    printf( "This tells the program to search for word rectangles of the dimensions between\n"
            "%d x %d (or area %d) and %d x %d (or area %d),\n"
            , defMinTall, defMinTall, defMinArea, defMaxTall, defMaxTall, defMaxArea);
    printf( "using linked tries to check column words to find the next candidate for each word row.\n\n"
            "The letter options are:\n");
    printf( "-a   Use alternative search algorithm (might be faster).\n");
    printf( "-d   Use the next argument for the dictionary file name (instead of %s).\n", defDictFileName);
    printf( "-h   Show this help message.\n");
    printf( "-i   Order the search for word rectangles in increasing order (slower but fun).\n"
            "     The default is to order the search by decreasing area, so it can end when the"
            "     first (largest) is found.\n");
    printf( "-l   Find word lattices instead of word rectangles\n");
    printf( "-m   Use std::map::upper_bound instead of linked tries to find rows (much slower).\n");
    printf( "-o   Odd dimensions only: search only for rectangles of odd widths and heights.\n");
    printf( "-q   Quiet mode.  Omit most of the messages about starting and ending finder-threads.\n");
    printf( "-s   Single-threaded mode; not using a pool of parallel worker threads\n");
    printf( "-w   Find word waffles instead of word rectangles or lattices.\n");
    printf( "-z   Find ALL MAXIMAL word rectangles in the specified range, instead of only\n"
            "     one for each possibly maximal total area.  If a larger word rect is found,\n"
            "     any ongoing searches for smaller rects are terminated as usual.  Even so,\n"
            "     this option may result in very long run times.\n");

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


#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

char *getCWD(void)
{
	char* buffer;

	// Get the current working directory: 
	if ((buffer = _getcwd(NULL, 0)) == NULL)
	{
		perror("_getcwd error");
		return NULL;
	}
	else
	{
		printf("%s \nLength: %d\n", buffer, strlen(buffer));
		return buffer;
	}
}

#ifdef _MBCS
int main(int argc, char* argv[], char *env)
{   
    return wordRectMain(argc, (const char **)argv);
}
#endif

int wordRectMain(int argc, const char* argv[])
{   
    // set/get default args, check actual args and either show usage or call wordRect
	uint minWordLength = 2, maxWordLength = 36; // TODO: arbitrary numbers
    int  minArea = defMinArea;
    int  minTall = defMinTall;
    int  maxTall = defMaxTall;
    int  maxArea = defMaxTall*defMaxTall;
    int  tempInt = 0;
    int  verbosity = 2;
    unsigned int managerFlags = WordRectSearchExec::eDefaultZero;
    unsigned int finderFlags = 0;

    const char *programName  = argv[0] ? argv[0] : defProgramName;
    const char *dictFileName = defDictFileName;
    char reason[BUFSIZE];
    int j = 1, minCharCount = 500;
    if (j < argc && argv[j][0] == '-') {   // [-letters] options?
        for (const char *pc = argv[j]; *++pc != '\0'; ) {
            switch (*pc) {
            case 'a' :
                finderFlags |= WordRectFinder<CharMap>::eUseAltAlg;
                break;
            case 'c' :
                break;
            case 'd' :
                if (argv[++j] != NULL) {
                    dictFileName = argv[j];
                }
                break;
            case 'h' : 
                usage(argc, argv, "help was requested (-h)");
                break;
            case 'i' :
				managerFlags |= WordRectSearchExec::eIncreasingSize;
                break;
            case 'l' :
                managerFlags |= WordRectSearchExec::eFindLattices;
                break;
            case 'm' :
                finderFlags |= WordRectFinder<CharMap>::eUseMaps;
                break;
            case 'o' :
                managerFlags |= WordRectSearchExec::eOnlyOddDims;
                break;
			case 'q' :
				verbosity = 1;
				break;
			case 'r' :
				verbosity = 0;
				break;
			case 's' :
				managerFlags |= WordRectSearchExec::eSingleThreaded;
				break;
			case 't' :
                //TrixNode::testTrixNode(charFreqMap);
				break;
            case 'w' :
                managerFlags |= WordRectSearchExec::eFindWaffles;
                break;
            case 'z' :
                finderFlags |= WordRectFinder<CharMap>::eFindAll;
                break;
            default:
                sprintf_safe(reason, BUFSIZE, "got unknown option: %c", *pc);
                usage(argc, argv, reason);
                break;
            }
        }
        j++;
    }

#if _DEBUG
	char *cwd = getCWD();
	printf("CWD is %s\n", cwd);
#endif

	// Word lattices must have odd dims
	if (managerFlags &  WordRectSearchExec::eFindLattices)
		managerFlags |= WordRectSearchExec::eOnlyOddDims;

    if (j < argc) {
        tempInt = atoi(argv[j++]);
        if (tempInt < minArea) {
            sprintf_safe(reason, BUFSIZE, "minArea %d < %d", tempInt, minArea);
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

    const char *mapsOrTries = finderFlags & WordRectFinder<CharMap>::eUseMaps ? "maps" : "tries";
    printf("%s: using %s,\n", programName, mapsOrTries);
    printf("   min & max Area: %d %d, min & max Height: %d %d.\n", minArea, maxArea, minTall, maxTall);
    if (minArea > maxArea || minTall > maxTall) {
        usage(argc, argv, "minArea > maxArea or minHeight > maxHeight");
    }


    WordRectSearchExec& searchExec = WordRectSearchExec::getInstance();
    searchExec.setOptions(managerFlags, finderFlags, minWordLength, maxWordLength, verbosity);
    searchExec.startupSearchManager(dictFileName, minCharCount, minArea, minTall, maxTall, maxArea);
	searchExec.destroySearchManager();
	WordRectSearchExec::deleteInstance();
    return 0;
}
