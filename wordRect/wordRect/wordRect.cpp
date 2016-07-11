// wordRect.cpp : find largest dense rectangles of words from a dictionary

#include "TrieTree.hpp"

// static globals: These are used by the interrupt handler (which prints partial results on Ctrl-C).
// This code is only safe for single-threaded execution.  One trivial parallelization is to run
// multiple copies on multiple processors/cores, with different input parameters, which partition
// the search space (e.g. wordRect 4 2 63 &; wordRect 63 2 &);
// Better would be to either: 
// 1) put WordRows, HaveTall, etc., on the stack and lose the signal handler, or: 
// 2) give each thread its own memory on the heap, and give the signal handler access so it can 
// display the partial results from all the parallel threads.  (Once the tries or maps are initialized,
// they are only read through pointers to const, so they could be shared.)
static int TotalCalls = 0;
static volatile int HaveTall = 0;
static const char * WordRows[BUFSIZE] = { NULL, };      // WordRows[MAXLEN] always remains NULL (because MAXLEN < BUFSIZE)
static time_t timeBeg, timeTry, timeLastSigint;

static void printWordRect(const char * const wordRect[MAXLEN], bool complete, int haveTall) 
{
    static const int offset = 'A' - 'a';
    if (haveTall > MAXLEN) {
        haveTall = MAXLEN;
    }
    time_t timeNow; 
    time( &timeNow );
    int wordWide = (haveTall > 0) ? strlen(wordRect[0]) : 0;
    printf("word rectangle %2d * %2d = %3d     %s after %d seconds:\n", wordWide, haveTall, wordWide*haveTall
        , (complete ? "COMPLETE" : "progress"), timeNow - timeTry);
    for (int k = 0; k < haveTall; k++) {
        for (const char *pc = wordRect[k]; *pc != NULL; pc++) {
            printf(" %c", *pc + offset);
        }
        printf("\n");
    }
}

void sigint_handler ( int sig )
{
    // On SIGINT (Ctrl-C), print the word rectangle in progress, or, on double-Ctrl-C, exit.
    static int signal_count = 0;
    if (sig != SIGINT) {
        return;                 // only handl Ctrl-C
    }
    signal(sig,  SIG_IGN);      // ignore all Ctrl-C's until the handler is reset

    //if (signal_count == 0) {  // give notice the first time?
    //    printf("\nCaught signal %d (Ctrl-C)\n", sig);
    //}

    signal_count++;
    time_t timeNow;
    time( &timeNow );                                 // time now, in seconds since 1969
    timeLastSigint = timeNow - timeLastSigint;        // seconds since last call
    if (timeLastSigint < 2) {
        printf("signal_handler signal(%d) AND time difference (%d < 2 seconds) -- bye bye!\n", sig, timeLastSigint);
        exit(0);
    }
    timeLastSigint = timeNow;                   // remember the time of this call
    printWordRect(WordRows, false, HaveTall);   // show latest (partial) word rect
    if (signal_count < 5) {                     // advise the user, the first few times...
        printf("\nTo exit, hit Ctrl-C twice, quickly.\n\n");
    }
    signal(SIGINT, sigint_handler);		        // reinstall this handler
}

static int initFromSortedFile(TrieTree tries[], WordMap maps[], const char *fname)
{   // This function allocates memory for holding all the words.
    // If this were to be called more than once, the caller must release all previously 
    // loaded words to prevent a huge memory leak.
    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r") ) {
        printf("Opened %s\n" , fname);
    } else {
        printf("Error opening %s; exiting.\n", fname);
        return 0;
    }

    TrieNode *prevNode[BUFSIZE] = { NULL, };
    char word[BUFSIZE];
    int numWords = 0;
    while (fgets(word, BUFSIZE, fi))  {
        // got next word as a line in the file; now get its length, and remove any trailing newline.
        int length = strlen(word);
        if (word[length - 1] < TrieNode::firstLetter) {
            word[length - 1] = NULL;
            length = length - 1;
        }
        char *saveWord = _strdup(word);		// Allocate word's memory here, for the life of the program.
        numWords++;
        prevNode[length] = tries[length].insertWord(saveWord, prevNode[length]);
        if (maps) {
            maps[length].insert(WordMap::value_type(saveWord, numWords));   // strictly increasing value preserves the order
        }
    }
    fclose(fi);
    return numWords;
}


static int findWordRectRowsMap(int wantWide, int wantTall, int haveTall, TrieTree tries[], WordMap maps[]
, const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    //  TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }

    WordMap&  rowMap = maps[wantWide]; 
    TrieTree& colTri = tries[wantTall];
    char temp[BUFSIZE];

    int j = 1, k = 0;
    for(WordMap::const_iterator it = rowMap.begin(); it != rowMap.end(); ++it) {
        const char *word = it->first;
        if (0 < k && k < wantWide) {
            for (j = k; j > 0; j--) {
                if (word[j] != temp[k]) {
                    break;
                }
            }
            if (j == 0) {
                continue;   // first k letters of new word are the same as those of previously disqualified word
            }
        }
        for (k = 0; k < wantWide; k++) {
            wordCols[k][haveTall] = temp[k] = word[k];
            wordCols[k][haveTall + 1] = NULL;
            if ( ! colTri.subTrie(wordCols[k]) ) {
                break;
            }
        }
        if (k == wantWide) {
            wordRows[haveTall] = word;
            int area = findWordRectRowsMap(wantWide, wantTall, haveTall+1, tries, maps, wordRows, wordCols);
            if (area > 0) {
                return area;
            }
        }
    }
    return 0;
}


static int findWordRectRowsMapul_const_cast(int wantWide, int wantTall, int haveTall, TrieTree tries[], WordMap maps[]
, const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    //TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    WordMap&  rowMap = maps[wantWide]; 
    TrieTree& colTri = tries[wantTall];
    char temp1, temp2;

    for(WordMap::const_iterator itr = rowMap.begin(), end = rowMap.end(); itr != end; ) {
        const char *word = itr->first;
        for (int k = 0; k < wantWide; k++) {
            wordCols[k][haveTall] = word[k];
            if ( ! colTri.subTrix(wordCols[k], haveTall) ) {
                if (k < wantWide - 1) {
                    temp1 = word[k+1];
                    temp2 = word[k+2];
                    (char)word[k + 1] = '{';
                    (char)word[k + 2] = '\0';
                    itr = rowMap.upper_bound(word);
                    (char)word[k + 1] = temp1;
                    (char)word[k + 2] = temp2;
                } else {
                    ++itr;
                }
                goto END_LOOP;   
            }
        }
        wordRows[haveTall] = word;
        int area = findWordRectRowsMapul_const_cast(wantWide, wantTall, haveTall+1, tries, maps, wordRows, wordCols);
        if (area > 0) {
            return area;
        }
        ++itr;
END_LOOP:;
    }
    return 0;
}

static int findWordRectRowsMapUpper(int wantWide, int wantTall, int haveTall, TrieTree tries[], WordMap& rowMap
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    //TotalCalls++;
    //if ( (TotalCalls % 10000) == 0 ) {
    //    timeNow = time(&timeNow);
    //    if (difftime(timeNow, timeWas) > 20*60) {
    //        printWordRect(NextRows, 0, haveTall);
    //        timeWas = timeNow;
    //    }
    //}

    //WordMap&  rowMap = maps[wantWide]; // zoid
    TrieTree& colTri = tries[wantTall];
    char temp[BUFSIZE];

    for(WordMap::const_iterator itr = rowMap.begin(), end = rowMap.end(); itr != end; ) {
        const char *word = itr->first;
        for (int k = 0; k < wantWide; k++) {
            wordCols[k][haveTall] = temp[k] = word[k];
            if ( ! colTri.subTrix(wordCols[k], haveTall) ) {
                temp[k+1] = '{';				// ASCII decimal 123, the char after 'z'
                temp[k+2] = '\0';				// NULL-terminate the C-string
                itr = rowMap.upper_bound(temp);	// Get first word-node alphabetically > temp
                goto END_LOOP;          		// break out of both inner and outer loop
            }
        }
        wordRows[haveTall] = word;
        int area = findWordRectRowsMapUpper(wantWide, wantTall, haveTall+1, tries, rowMap, wordRows, wordCols);
        if (area > 0) {
            return area;
        }
        ++itr;
END_LOOP:;
    }
    return 0;
}


static int findWordRectRowsTrie(  int wantWide, int wantTall, int haveTall, TrieTree tries[]
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    //TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    TrieTree& rowTri = tries[wantWide];			// zoid: don't need to re-offset every call; want's are invariant
    TrieTree& colTri = tries[wantTall];
    const TrieNode *node = rowTri.getRoot();    // Could do slightly better if haveTall > 0; not worth it
    while(node) {
        node = node->getFirstWordNode();
        if ( ! node ) {
            break;                              // failure
        }
        const char *word = node->getWord();
        // For each resulting stem (column), is there a word of length wantTall that could complete it?

        int k = 0;	 // zoid: why start over at k = 0?  
        for ( ; k < wantWide; k++) {
            wordCols[k][haveTall] = word[k];
#if 1		// Use the trie...
            if ( ! colTri.subTrix(wordCols[k], haveTall) ) {
                break;		 // zoid: don't break; instead, get a new word and reset k back however far it needs to go right here; return 0 only if no new word can be found, which really means previous row word must change.
            }
#elif 0	// Use some STL hash_set
#elif 2
            // Or stupidly use the maps:
            wordCols[k][haveTall+1] = NULL;
            WordMap::const_iterator itr = colMap.upper_bound(wordCols[k]);
            const char *temp = itr->first;
            if (temp differs from col[k] in the first haveTall letters) {
                break;
            }
#endif
        }
        // At least one wantTall-length word exists to complete each column, so try adding another row.

        if (k == wantWide) {	// zoid: would not have to test this, if the break above were replace with...  a GOTO?
            wordRows[haveTall] = word;
            int area = findWordRectRowsTrie(wantWide, wantTall, haveTall+1, tries, wordRows, wordCols);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// zoid: limit to the correct range
        }
        // As wordRows[haveTall], this word fails at column k: no wantTall-letter 
        // word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word[0 ... k].
        node = rowTri.getNextWordNodeAndIndex(node, word, k, wantWide - k);
    }
    return 0;
}

// zoid: todo: make this (and other find functions) methods of TrieTree

static int findWordRectRowsTrix(  int wantWide, int wantTall, int haveTall, TrieTree& rowTri, TrieTree& colTri
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    //TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    // We could initialize a little smarter when haveTall > 0, but don't bother optimizing this.

    int k = 0;	 // init before outer loop
    const TrieNode *rowRoot = rowTri.getRoot();
    const TrieNode *lastColStemNode = NULL; 
    for(const TrieNode *rowWordNode = rowRoot->getFirstWordNode(); rowWordNode != NULL; ) {

        const char *word = rowWordNode->getWord();
        // For each resulting stem (column), is there a word of length wantTall that could complete it?

        for ( ; k < wantWide; k++) {
            wordCols[k][haveTall] = word[k];
#if 1
            if ( ! colTri.subTrix(wordCols[k], haveTall) ) {
                break;		 // zoid: don't break; instead, get a new word and reset k back however far it needs to go right here; return 0 only if no new word can be found, which really means previous row word must change.
            }
#else
            if ( ! rowRoot->isSubKeyStem(wordCols[k], haveTall, lastColStemNode) ) {
                break;
            }
#endif
        }
        // At least one wantTall-length word exists to complete each column, so try adding another row.

        if (k == wantWide) {	// zoid: would not have to test this, if the break above were replace with...  a GOTO?
            wordRows[haveTall] = word;
            int area = findWordRectRowsTrix(wantWide, wantTall, haveTall+1, rowTri, colTri, wordRows, wordCols);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// zoid: limit to the correct range
        }
        // As wordRows[haveTall], this word fails at column k: no wantTall-letter 
        // word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word[0 ... k].
        rowWordNode = rowTri.getNextWordNodeAndIndex(rowWordNode, word, k, wantWide - k);

        // zoid: now = rowTri.getNextWordNodeMemo(lastRowWordStem)
    }
    return 0;
}

static int findWordRectRowsUsingGetNextWordNodeAndIndex(  int wantWide, int wantTall, int haveTall, TrieTree& rowTri, TrieTree& colTri
    , const char *wordRows[MAXLEN], const TrieNode ** colNodes[BUFSIZE] )
{
    //TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    int k = 0, prevTall = haveTall - 1;	            // prevTall == -1 is ok.
    const TrieNode *rowWordNode = (haveTall > 0) ?
        rowTri.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(haveTall)) : 
    rowTri.getFirstWordNode();
    while(rowWordNode != NULL) {
        const char *word = rowWordNode->getWord();
        for ( ; k < wantWide; k++) {
            // For each resulting stem (column), is there a word of length wantTall that could complete it?
            const TrieNode *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);
            if (colBranch == NULL) {
                break;                              // ...if not, break (and try the next word)
            }
            colNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
        }

        // At least one wantTall-length word exists to complete each column, so try adding another row.
        if (k == wantWide) {
            wordRows[haveTall] = word;
            int area = findWordRectRowsUsingGetNextWordNodeAndIndex(wantWide, wantTall, haveTall+1, rowTri, colTri, wordRows, colNodes);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// limit to the correct range
        }
        // As a candidate for wordRows[haveTall], this word failed at column k: 
        // no wantTall-letter word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word-up-to-length-k.
        rowWordNode = rowTri.getNextWordNodeAndIndex(rowWordNode, word, k, wantWide - k);
    }
    return 0;
}

static int findWordRectRowsTriS(  int wantWide, int wantTall, int haveTall, TrieTree& rowTri, TrieTree& colTri
    , const char *wordRows[MAXLEN], const TrieNode ** colNodes[BUFSIZE] )
{
    //TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1, haveTall);
        return wantWide*wantTall;
    }
    HaveTall = haveTall;

    int prevTall = haveTall - 1, wantWideM1 = wantWide - 1;             // prevTall == -1 is ok.
    const TrieNode *rowNodesMem[BUFSIZE], **rowNodes = &rowNodesMem[1];  // put row nodes on the local stack: 1 row per stack level.
    rowNodes[-1] = rowTri.getRoot();
    const TrieNode *rowWordNode = (haveTall > 0) ?
        rowTri.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(haveTall)) : 
    rowTri.getFirstWordNode();
    for (int k = 0; rowWordNode != NULL; ) {
        const char *word = rowWordNode->getWord();
        for (;;) {
            // For each stem (partial column) that would result from adding this word as the next row,
            // is there a word of length wantTall that could complete it?
            rowNodes[k] = rowNodes[k-1]->getBranchAtLetter(word[k]);                        // Need this later, so always set it.
            const TrieNode *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);   // Need this later only if non-NULL.
            if (colBranch == NULL) {
                break;                              // ...if not, break to get the next word.
            }
            colNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
            if (k == wantWideM1) {
                // At least one wantTall-length word exists to complete each column, including this row,
                wordRows[haveTall] = word;                                 // so try adding another row.
                int area = findWordRectRowsTriS(wantWide, wantTall, haveTall+1, rowTri, colTri, wordRows, colNodes);
                if (area > 0) {
                    return area;                    // Found a wordRect, so return up through the stack
                }
                break;                              // This word failed, so break to get the next word.
            }
            k++;
        }
        // As a candidate for wordRows[haveTall], this word failed at column k,
        // either locally (no completion for column k < wantWide-1) or deeper down
        // in the wordRect (all wantWide columns were ok, some later word row could not be found).
        // Either way, we try the next possible word, which is the first word whose 
        // k-letter stem is greater than this word's.  If such a word exists, we get 
        // it just by following 2 links:  (failed node)->nextStemNode->firstWordNode.
        // (If we weren't using the rowNodes, we'd need to find the parent of the first
        // disqualified node, as does TrieTree::getNextWordNodeAndIndex, but over all, that is slower.)
        const TrieNode * nextStem = rowNodes[k]->getNextStemNode();
        if (nextStem == NULL) {
            return 0;  // We've tried all possible word stems for the partial columns we got, so return. 
        }
        k = nextStem->getDepth() - 1;
        rowWordNode = nextStem->getFirstWordNode();
    }
    return 0;
}

static void wordRect(int minArea, int minTall, int maxTall, int maxArea, int useMaps)
{
    TrieTree triesMem[MAXLEN], *tries = &triesMem[-1];  // index by word length (not length-1)
    WordMap   mapsMem[MAXLEN], *maps  = &mapsMem[-1];   // index by word length (not length-1)
    char     ctimeBuf[BUFSIZE];
    
    int numWords = initFromSortedFile(tries, maps, "WORD.LST.txt");	// call this once; word/trie/map memory is retained for life of the program.
    if (numWords == 0) {
        printf("Found 0 words; exiting.\n");
        exit(1);
    }
    
    int area = 0, haveArea = 0, wantArea = 0;
    const char **wordRows = WordRows;
    char prevCols[MAXLEN][MAXLEN];
    const TrieNode  *colNodesMem[BUFSIZE][BUFSIZE],   **colNodes[BUFSIZE];
    for (int j = 0; j < BUFSIZE; j++) {
        colNodes[j] = &colNodesMem[j][1];   // For each column j, colNodes[j][-1] will point to the colTri root.
    }

    time_t timeBeg;
    time( &timeBeg );
    ctime_s(ctimeBuf, BUFSIZE, &timeBeg );
    printf("Starting the search at %s\n", ctimeBuf);
    signal( SIGINT, sigint_handler );
    printf("Hit Ctrl-C to see the word rectangle in progress...\n\n");

    for (int wantTall = minTall; wantTall <= maxTall; wantTall++) {
        TrieTree& colTri = tries[wantTall];

        for (int wantWide = wantTall; wantWide <= MAXLEN; wantWide++) {
            TrieTree& rowTri = tries[wantWide];
            for (int j = 0; j < wantWide; j++) {
                colNodes[j][-1] = colTri.getRoot();
            }
            WordMap& rowMap = maps[wantWide];
            wantArea = wantWide*wantTall;
            if (wantArea < minArea) {
                printf("Skip %2d * %2d because %3d <  %3d (minArea)\n",  wantWide, wantTall, wantArea, minArea);
            } else if (wantArea <= haveArea) {
                printf("Skip %2d * %2d because %3d <= %3d (area already found)\n", wantWide, wantTall, wantArea, haveArea);
            } else if (wantArea > maxArea) {
                printf("Skip %2d * %2d because %3d >  %3d (maxArea)\n",  wantWide, wantTall, wantArea, maxArea);
            } else {
                time( &timeTry );
                printf("TRY  %2d * %2d because %3d >= %3d  seconds total: %d\n", wantWide, wantTall, wantArea, haveArea, timeTry - timeBeg);
                if (useMaps) {                                                                                                  // time to find 11 x 4:
                    area = findWordRectRowsMapUpper(wantWide, wantTall, 0, tries, rowMap, wordRows, prevCols);                              // 80 seconds
                } else {
                    area = findWordRectRowsTriS(    wantWide, wantTall, 0, rowTri, colTri, wordRows, colNodes);                         // 15 seconds
                    //  area = findWordRectRowsUsingGetNextWordNodeAndIndex(    wantWide, wantTall, 0, rowTri, colTri, wordRows, colNodes); // 33 seconds
                    //  area = findWordRectRowsTrix(    wantWide, wantTall, 0, rowTri, colTri, wordRows, prevCols);                         // 46 seconds
                    //  area = findWordRectRowsTrie(    wantWide, wantTall, 0, tries, wordRows, prevCols);                                  // 40 seconds
                }
                if (haveArea < area) {
                    haveArea = area;
                }
            }
        }
    }
    return;
}

static void usage(int argc, char* argv[], int minArea, int minTall, int maxTall, int maxArea, int useMaps)
{
    printf("Usage: wordRect [minArea minHeight maxHeight maxArea useMaps]\n");    
    printf("All arguments are optional.  The default values are: %d %d %d %d %d,\n"
        , minArea, minTall, maxTall, maxArea, useMaps);
    printf("This tells the program to search for word rectangles of increasing area\n");
    printf("from %d x %d (or area %d) up to %d x %d (or area %d),\n"
        , minTall, minTall, minArea, maxTall, maxTall, maxArea);
    printf("using %s to find the next candidate for each word row.\n"
        , (useMaps != 0) ? "std::maps upper_bound" : "tries with forward links");
    printf("\nIf you specify minArea (%s) as < 4, you get this message.\n", argv[1]);
    printf("Exiting from this command:\n    ");
    for (int j = 0; j < argc; j++) {
        printf("%s ", argv[j]);
    }
    printf("\n");
    exit(0);
}

int main(int argc, char* argv[])
{
    int  minArea = 4;
    int  minTall = 2;
    int  maxTall = MAXLEN;
    int  maxArea = MAXLEN*MAXLEN;
    int  tempInt = 0;
    int  useMaps = 0;    // if non-zero, use std::maps (slower by a factor of 3 or more)
    if (argc > 1) {
        tempInt = atoi(argv[1]);
        if (tempInt < minArea) {
            usage(argc, argv, minArea, minTall, maxTall, maxArea, useMaps);
        }
        minArea = tempInt;
        if (argc > 2) {
            tempInt = atoi(argv[2]);
            minTall = tempInt > minTall ? tempInt : minTall;
            if (argc > 3) {
                tempInt = atoi(argv[3]);
                maxTall = tempInt < maxTall ? tempInt : maxTall;
                if (argc > 4) {
                    tempInt = atoi(argv[4]);
                    maxArea = tempInt < maxArea ? tempInt : maxArea;
                    if (argc > 5) {
                        useMaps = atoi(argv[5]);
                    }
                }
            }
        }
    }
    printf("%s: using %s,\n", argv[0], (useMaps ? "maps" : "tries"));
    printf("   min & max Area: %d %d, min & max Height: %d %d.\n", minArea, maxArea, minTall, maxTall);
    if (minArea > maxArea || minTall > maxTall) {
        printf("  Exit because min area > max area or min height > max height.\n");
        exit(0);
    }
    wordRect(minArea, minTall, maxTall, maxArea, useMaps);
    return 0;
}
