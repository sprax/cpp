// words.cpp : Defines the entry point for the console application.
#include "stdafx.h"

static unsigned int TotalCalls = 0;
static const char **NextRows = NULL;
static time_t timeNow = 0, timeWas = 0, timeLastSigint = 0;



void printWordRect(const char *wordRect[], bool save) 
{
    static const int offset = 'A' - 'a';
    int wordTall = 0;
    while(wordRect && wordRect[wordTall] != NULL && wordTall < MAXLEN) {
        wordTall++;
    }
    int wordWide = wordTall ? strlen(wordRect[0]) : 0;
    printf("word rectangle %2d * %2d = %d (%s):\n", wordWide, wordTall, wordWide*wordTall
        , save ? "COMPLETE" : "partial");
    for (int k = 0; k < wordTall; k++) {
        for (const char *pc = wordRect[k]; *pc != NULL; pc++) {
            printf(" %c", *pc + offset);
        }
        printf("\n");
    }
}


static int findWordRectRowsMap(int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL maps[]
, const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    TotalCalls++;

    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    MapSL&  rowMap = maps[wantWide]; 
    TrieSL& colTri = tries[wantTall];
    char temp[MAXLEN];

    int j = 1, k = 0;
    for(MapSL::const_iterator it = rowMap.begin(); it != rowMap.end(); ++it) {
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


//static int findWordRectRowsMapul(int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL maps[]
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
//    TrieSL& colTri = tries[wantTall];
//    char temp[MAXLEN];
//
//    int k = 0;
//    MapSL::const_iterator itr = rowMap.begin();
//    MapSL::const_iterator end = rowMap.end();
//    const char *oldy = NULL;
//    while(itr != end) {
//        const char *word = itr->first;
//        for (k = 0; k < wantWide; k++) {
//            wordCols[k][haveTall] = temp[k] = word[k];
//            if ( ! colTri.subTrix(wordCols[k], haveTall) ) {
//                break;
//            }
//        }
//        if (k == wantWide) {
//            strcpy_s(wordRows[haveTall], MAXLEN, word);
//            int area = findWordRectRowsMapul(wantWide, wantTall, haveTall+1, tries, maps, wordRows, wordCols);
//            if (area > 0) {
//                return area;
//            }
//            ++itr;
//        } else {
//            temp[k+1] = '{';
//            temp[k+2] = '\0';
//            itr = rowMap.upper_bound(temp);
//        }
//        oldy = word;
//    }
//    return 0;
//}

static int findWordRectRowsMapul_const_cast(int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL maps[]
, const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    TotalCalls++;

    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    MapSL&  rowMap = maps[wantWide]; 
    TrieSL& colTri = tries[wantTall];
    char temp1, temp2;

    for(MapSL::const_iterator itr = rowMap.begin(), end = rowMap.end(); itr != end; ) {
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

static int findWordRectRowsMapUpper(int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL& rowMap
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    NextRows = wordRows;
    TotalCalls++;
    if ( (TotalCalls % 10000) == 0 ) {
        timeNow = time(&timeNow);
        if (difftime(timeNow, timeWas) > 20*60) {
            printWordRect(NextRows, 0);
            timeWas = timeNow;
        }
    }

    //MapSL&  rowMap = maps[wantWide]; // zoid
    TrieSL& colTri = tries[wantTall];
    char temp[MAXLEN];

    for(MapSL::const_iterator itr = rowMap.begin(), end = rowMap.end(); itr != end; ) {
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


static int findWordRectRowsTrie(  int wantWide, int wantTall, int haveTall, TrieSL tries[], MapSL& colMap
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    TrieSL& rowTri = tries[wantWide];			// zoid: don't need to re-offset every call; want's are invariant
    TrieSL& colTri = tries[wantTall];
    const TrieSln *node = rowTri.getRoot();    // Could do slightly better if haveTall > 0; not worth it
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
            MapSL::const_iterator itr = colMap.upper_bound(wordCols[k]);
            const char *temp = itr->first;
            if (temp differs from col[k] in the first haveTall letters) {
                break;
            }
#endif
        }
        // At least one wantTall-length word exists to complete each column, so try adding another row.

        if (k == wantWide) {	// zoid: would not have to test this, if the break above were replace with...  a GOTO?
            wordRows[haveTall] = word;
            int area = findWordRectRowsTrie(wantWide, wantTall, haveTall+1, tries, colMap, wordRows, wordCols);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// zoid: limit to the correct range
        }
        // As wordRows[haveTall], this word fails at column k: no wantTall-letter 
        // word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word[0 ... k].
        node = rowTri.getNextWordNodeOld(node, word, k, wantWide - k);

        // zoid: now = rowTri.getNextWordNodeMemo(lastRowWordStem)
    }
    return 0;
}

// zoid: todo: make this (and other find functions) methods of TrieSL

static int findWordRectRowsTrix(  int wantWide, int wantTall, int haveTall, TrieSL& rowTri, TrieSL& colTri
    , const char *wordRows[MAXLEN], char wordCols[MAXLEN][MAXLEN])
{
    TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    // We could initialize a little smarter when haveTall > 0, but don't bother optimizing this.

    int k = 0;	 // init before outer loop
    const TrieSln *rowRoot = rowTri.getRoot();
    const TrieSln *lastColStemNode = NULL; 
    for(const TrieSln *rowWordNode = rowRoot->getFirstWordNode(); rowWordNode != NULL; ) {

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
        rowWordNode = rowTri.getNextWordNodeRef(rowWordNode, word, k, wantWide - k);

        // zoid: now = rowTri.getNextWordNodeMemo(lastRowWordStem)
    }
    return 0;
}

static int findWordRectRowsTriZ(  int wantWide, int wantTall, int haveTall, TrieSL& rowTri, TrieSL& colTri
    , const char *wordRows[MAXLEN], const TrieSln ** colNodes[MAXLEN] )
{
    TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    //	const TrieSln *lastColStemNode = NULL; 

    // We could initialize a little smarter when haveTall > 0, but don't bother optimizing this.
    int k = 0, prevTall = haveTall - 1;	// An index of -1 is ok here.

    if (haveTall > 0) {
        const TrieSln *colNode = colNodes[0][prevTall]->getFirstBranch();
        const char *stem = colNode->getFirstWordNode()->getWord();
        int letter = colNode->getLetterAt(0);
        assert(letter == stem[0]);
        const TrieSln *firstWN = rowTri.getFirstWordNodeFromLetter(stem[0]);
        const TrieSln *otherWN = rowTri.getRoot()->branches[stem[0] - TrieSln::firstLetter]->getFirstWordNode();
        if (firstWN != otherWN) {
            int dbg = 99;
        }
    }

    const TrieSln *rowWordNode = (haveTall > 0) ?
        rowTri.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(0)) : 
        rowTri.getFirstWordNode();
    while(rowWordNode != NULL) {
        const char *word = rowWordNode->getWord();
        for ( ; k < wantWide; k++) {
            // For each resulting stem (column), is there a word of length wantTall that could complete it?
            const TrieSln *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);
            if (colBranch == NULL) {
                break;                              // ...if not, break (and try the next word)
            }
            colNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
        }

        // At least one wantTall-length word exists to complete each column, so try adding another row.
        if (k == wantWide) {	// zoid: would not have to test this, if the break above were replace with...  a GOTO?
            wordRows[haveTall] = word;
            int area = findWordRectRowsTriZ(wantWide, wantTall, haveTall+1, rowTri, colTri, wordRows, colNodes);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// zoid: limit to the correct range
        }
        // As a candidate for wordRows[haveTall], this word failed at column k: 
        // no wantTall-letter word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word-up-to-length-k.
        rowWordNode = rowTri.getNextWordNodeRef(rowWordNode, word, k, wantWide - k);
    }
    return 0;
}




static int findWordRectRowsTriN(  int wantWide, int wantTall, int haveTall, TrieSL& rowTri, TrieSL& colTri
    , const char *wordRows[MAXLEN], const TrieSln ** colNodes[MAXLEN] )
{
    TotalCalls++;
    if (haveTall == wantTall) {
        printWordRect(wordRows, 1);
        return wantWide*wantTall;
    }

    //	const TrieSln *lastColStemNode = NULL; 

    // We could initialize a little smarter when haveTall > 0, but don't bother optimizing this.
    int k = 0, prevTall = haveTall - 1;	// An index of -1 is ok here.

    if (haveTall > 0) {
        const TrieSln *colNode = colNodes[0][prevTall]->getFirstBranch();
        const char *stem = colNode->getFirstWordNode()->getWord();
        int letter = colNode->getLetterAt(0);
        assert(letter == stem[0]);
        const TrieSln *firstWN = rowTri.getFirstWordNodeFromLetter(stem[0]);
        const TrieSln *otherWN = rowTri.getRoot()->branches[stem[0] - TrieSln::firstLetter]->getFirstWordNode();
        if (firstWN != otherWN) {
            int dbg = 99;
        }
    }

    const TrieSln *rowWordNode = (haveTall > 990) ? // zoid
        rowTri.getFirstWordNodeFromLetter(colNodes[0][prevTall]->getLetterAt(0)) : 
        rowTri.getFirstWordNode();
    while(rowWordNode != NULL) {
        const char *word = rowWordNode->getWord();
        for ( ; k < wantWide; k++) {
            // For each resulting stem (column), is there a word of length wantTall that could complete it?
            const TrieSln *colBranch = colNodes[k][prevTall]->getBranchAtLetter(word[k]);
            if (colBranch == NULL) {
                break;                              // ...if not, break (and try the next word)
            }
            colNodes[k][haveTall] = colBranch;      // ...if so, add the branch node and continue.
        }

        // At least one wantTall-length word exists to complete each column, so try adding another row.
        if (k == wantWide) {	// zoid: would not have to test this, if the break above were replace with...  a GOTO?
            wordRows[haveTall] = word;
            int area = findWordRectRowsTriZ(wantWide, wantTall, haveTall+1, rowTri, colTri, wordRows, colNodes);
            if (area > 0) {
                return area;
            }
            k = wantWide - 1;	// zoid: limit to the correct range
        }
        // As a candidate for wordRows[haveTall], this word failed at column k: 
        // no wantTall-letter word begins with the haveTall letters in in the resulting column.  
        // So try the next word of length wantWide with stem > word-up-to-length-k.
        rowWordNode = rowTri.getNextWordNodeRef(rowWordNode, word, k, wantWide - k);
    }
    return 0;
}


//volatile static int g_sigint = 0;
void sigint_handler ( int sig )
{
    //g_sigint = 1;
    static int signal_count = 0;

    // On SIGINT (Ctrl-C), print out whatever we've got so far, or exit on double Ctrl-C
    if (signal_count++ < 1) {   // give notice the first time
        printf("\nCaught %d signal...\n", sig);
    }
    timeNow = time(&timeNow);                   // time now, in seconds since 1969
    timeLastSigint = timeNow - timeLastSigint;        // seconds since last call
    if (sig && timeLastSigint < 1) {
        printf("signal_handler signal(%d) AND time difference (%d < 2 seconds) -- bye bye!\n", sig, timeLastSigint);
        //printf("bye bye!\n");
        exit(0);
    }
    timeLastSigint = timeNow;          // remember the time of this call
    printWordRect(NextRows, 0);     // show latest (partial) word rect
    if (sig) {
        printf("\nTo exit, hit Ctrl-C twice.\n\n");
    }
}


static void initFromSortedFile(TrieSL tries[], MapSL maps[], const char *fname)
{   // This function allocates memory for holding all the words.
    // If this were to be called more than once, the caller must release all previously 
    // loaded words to prevent a huge memory leak.
    FILE *fi;
    if ( ! fopen_s(&fi, fname, "r") ) {
        printf("Opened %s\n" , fname);
    } else {
        printf("Error opening %s; exiting.\n", fname);
        return;
    }

    TrieSln *prevNode[MAXLEN] = { NULL, };
    char word[MAXLEN];
    for (int q = 0; fgets(word, MAXLEN, fi); q++)  {
        // got next word as a line in the file; now get its length, and remove any trailing CR/LF
        int length = strlen(word);
        if (word[length - 1] < TrieSln::firstLetter) {
            word[length - 1] = NULL;
            length = length - 1;
        }
        char *saveWord = _strdup(word);		// Allocate word's memory here, for life of program.
        prevNode[length] = tries[length].insertWord(saveWord, prevNode[length]);
        if (maps) {
            maps[length].insert(MapSL::value_type(saveWord, q));   // strictly increasing value preserves the order // but zoid, consider mapping word -> wordNode ??
        }
    }
    fclose(fi);
    return;
}


static void wordRect(int minArea, int minTall, bool useTrie=true)
{
    TrieSL tries[MAXLEN];
    MapSL  maps[MAXLEN];
    char ctimes[MAXLEN];
    time_t newTime, oldTime, begTime;
    double diffTime = 0.0, totalTime = 0.0;

    begTime = time( &oldTime );
    ctime_s(ctimes, MAXLEN, &begTime );
    printf("wordRect using findWordRectRowsTrie: starting at %s\n", ctimes);

    initFromSortedFile(tries, maps, "WORD.LST.txt");	// if this were called a 2nd time, maps should be used first to destroy all existing nodes.

    newTime = time( &oldTime );

    int area = 0, maxArea = minArea, wantArea = 0;
    const char * wordRows[MAXLEN] = { NULL }, **rows = wordRows;
    char prevCols[MAXLEN][MAXLEN];
    const TrieSln *  colNodesMem[MAXLEN][MAXLEN];
    const TrieSln ** colNodes[MAXLEN];
    for (int j = 0; j < MAXLEN; j++) {
        colNodes[j] = &colNodesMem[j][1];
    }
    //memset(colNodes, 0, TrieSln::numLetters*TrieSln::numLetters*sizeof(TrieSln*));

    // zoid: TODO: 28 = max found word len, returned from initFromSortedFile
    for (int wantTall = minTall; wantTall <= 28; wantTall++) {
        for (int wantWide = wantTall; wantWide <= 28; wantWide++) {
            wantArea = wantWide*wantTall;
            if (wantArea <= maxArea) {
                printf("Skip %2d * %2d because %3d <= %3d\n", wantWide, wantTall, wantArea, maxArea);
            } else {
                time( &newTime );
                diffTime  = difftime( newTime, oldTime ) / 60.0;    // ToDo: track in milliseconds but report in minutes?
                totalTime = difftime( newTime, begTime ) / 60.0;
                oldTime = newTime;
                printf("TRY  %2d * %2d because %3d >  %3d (time: %7.2f  total: %7.2f)\n", wantWide, wantTall, wantArea, maxArea, diffTime, totalTime);
                MapSL& rowMap = maps[wantWide];
                //MapSL& colMap = maps[wantTall];
                TrieSL& rowTri = tries[wantWide];			// zoid: don't need to re-offset every call; want's are invariant
                TrieSL& colTri = tries[wantTall];
                if (useTrie) {
                    for (int j = 0; j < wantWide; j++) {
                        colNodes[j][-1] = colTri.getRoot();
                    }
                    // area = findWordRectRowsTriN(    wantWide, wantTall, 0, rowTri, colTri, rows, colNodes);
                    area = findWordRectRowsTriZ(    wantWide, wantTall, 0, rowTri, colTri, rows, colNodes);
                    //  area = findWordRectRowsTrix(    wantWide, wantTall, 0, rowTri, colTri, rows, prevCols);
                    //  area = findWordRectRowsTrie(    wantWide, wantTall, 0, tries, colMap, rows, prevCols);
                } else {
                    area = findWordRectRowsMapUpper(wantWide, wantTall, 0, tries, rowMap, rows, prevCols);
                }
                if (maxArea < area) {
                    maxArea = area;
                }
            }
        }
    }
    return;  // breakpoint here...
}


int _tmain(int argc, _TCHAR* argv[])
{
#if 0
    test_octal();
    test_charString();
    return 0;
    //test_fibmem();
    test_fibmemo();
    //return 0;
    test_arrayContainsTwoEntriesThatSumToN();
    test_multimap();
    //return 0;
#endif

    bool useTrie = true;
    int minArea = 0;
    int minTall = 0;
    if (argc > 0) {
        minArea = _ttoi(argv[1]);
        if (argc > 1)
            minTall = _ttoi(argv[2]);
        _tprintf(L"%ws:  area(%d)\n", argv[0], minArea, minTall);
    }
    signal( SIGINT, sigint_handler );
    wordRect(minArea, minTall, useTrie);
    return 0;
}
