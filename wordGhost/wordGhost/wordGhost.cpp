//  wordGhost.cpp
//  Sprax Lines,  July 2010

/** TODO:
    Pick random nodes from Trie.  
    Find nearest ancestor(s).
    Play randomized ghost.
    Boggle.
    Crossword puzzle.
    Ringwords -- circles of letters that form many overlapping words.  Find largest 5-word circle.
    Hangman.
    etc.

*/
#include "stdafx.h"
#include "spraxUtil.hpp"
#include "SubWords.hpp"

/**
     wordGhost.cpp : Outcomes for the word game ghost.
     Sprax Lines, July 2010
     This program shows the optimal outcomes from all possible first 2 moves 
     of the word game called ghost.  Optimal means here that both players 
     (human first, computer second) play to win as quickly as possible and 
     to lose as slowly as possible.

  Optimal Ghost

  In the game of Ghost, two players take turns building up an English word
  from left to right. Each player adds one letter per turn. The goal is to 
  not complete the spelling of a word: if you add a letter that completes 
  a word (of 4+ letters), or if you add a letter that produces a string 
  that cannot be extended into a word, you lose. (Bluffing plays and 
  "challenges" may be ignored for the purpose of this puzzle.)

  Write a program that allows a user to play Ghost against the computer.

  The computer should play optimally given the following dictionary: WORD.LST. 
  Allow the human to play first. If the computer thinks it will win, it should 
  play randomly among all its winning moves; if the computer thinks it will 
  lose, it should play so as to extend the game as long as possible (choosing 
  randomly among choices that force the maximal game length).

  Your program should be written as a Java web application that provides a basic
  GUI for a human to play against the optimal computer player from inside the 
  Firefox browser. The web page should access a server using JavaScript's 
  XMLHttpRequest object, and display the results using DOM manipulation.

  Please submit your source code, configuration instructions, and any comments 
  on your approach. Please also include a WAR file that we can test against 
  Tomcat 5.5.x on Sun's J2SE 6.0. Finally, in your submission email, answer 
  this question: if the human starts the game with 'n', and the computer plays 
  according to the strategy above, what unique word will complete the human's 
  victory? 

    Winning: Given current stem, add letter s.t. only odd-length (len >= 4) words complete it.
        If there is more than one, play one with least max word length (win as quickly as possible.)
            If there is more than one of those, choose among them randomly using random picker.)

    Losing: Given current stem, add letter s.t. only the longest of even-length words complete it.
        A)  Absolute criterion: if there is no E-letter completion, he'll have to win on at least E+2 letters.
        B)  relative criterion (turn-taking game): All even length turns are mine, so I can prevent the worst
            case for me.  I can either: 
            1) evaluate his best move for each of my possible moves, and choose the least worst for me, or
            2) (pre)evaluate the entire series of remaining moves, assuming he plays optimally.

    Data: A trie that holds only words of length >= 4, and only one word per 4-letter stem.
    For example, it keeps "base" but not "ba" or "bas" or "baseball", "baseboard", "based", etc,
    because all words extending "base" are unreachable in the game.  The player who spess "base"
    has alraedy lost.
*/
//  The precomputed tree solution:
//  You lose by being force to pick a word.  So a word node is a loser.
//  Clearly, a node is a winner if all of its mBranches are words.  But that is not the only case.
//  A node is a winner iff all of its mBranches are losers, that is, every branch: 
//          1.  is either a word, or
//          2.  has a winner for at least one *its* mBranches.
//  This "recursive" definition can be implemented by starting at word nodes traversing up
//  the trie through parents, marking them alternately as winners or losers.  The rules
//  of deciding whether a parent is a winner or loser are simple enough...
//
//  Some heuristics are easy to see, but not necessarily helpful:
//      e.g. a stem of even/odd length that stems *only* even/odd-length words is a loser.  
//           but many stems stem both even and odd length words, so this heuristic isn't decisive.
// 

#include "stdafx.h"
#include "EvOdNode.hpp"
#include "Trie.hpp"

static const char *defDictionaryFile = "../WORD.LST.txt";
static const char *defProgramName  = "wordGhost";

static bool isLoserNode(const EvOdNode *node)   // For unit testing only!
{
    ADD_COUNT(1);

    if (node->isWord()) {
        return true;
    }
    // If it isn't a word node, it must have at least one branch,
    // and it is a loser if any of its mBranches is a winner.
    const EvOdNode *branch = node->getFirstChild();
    do {
        if ( ! isLoserNode(branch) ) {
            return true;
        }
    } while ((branch = branch->getNextSibling()) != NULL);
    // This node is not a word and all of its mBranches are losers, so it is a winner: 
    return false;
}

class GhostTrie : public EvOdNodeTrie
{
public:
    int unitTest()
    {
        // debugging diagnostics:
        int dbgWinLose = 0, dbgNumTurnsNeStrlen = 0, dbgNumTurnsNeEndDepth = 0;
        int dbgRobotWinsEven = 0, dbgHumanWinsOdd = 0;
        int dbgRobotNotBranch = 0, dbgBranchNotRobot = 0;
        int numCombos = 0;

        for (char c = 'a'; c <= 'z'; c++) {
            printf("\n%c                                                  1 3 5 7 9 1 3 5 7 9 1\n", toupper(c));
            const EvOdNode *ghostNode = getRoot()->getBranchAtLetter(c);
            int j = 0, totalCount = 0, humanWins = 0, robotWins = 0;
            WordLenT numTurns = 8888, tmpDepth = 0;
            const char *aLastWord = NULL, *maxWord = NULL;
            for (const EvOdNode *branch = ghostNode->getFirstChild(); branch != NULL; branch = branch->getNextSibling()) {
                numCombos++;
                SET_COUNT(0);
                bool isRobotWinner = branch->evalIsWinnerGetEnds(numTurns, &aLastWord);
                bool isRobotWinWin = branch->isWinnerMemo();
                bool isRobotLoser  = isLoserNode(branch);
                if (isRobotWinner != isRobotWinWin || isRobotWinner == isRobotLoser) {
                    dbgWinLose++;
                }
                if ( isRobotWinner ) {
                    robotWins++;
                } else {
                    humanWins++;
                }
                if (tmpDepth < numTurns) {
                    tmpDepth = numTurns;
                    maxWord  = aLastWord;
                }
                totalCount += fsCounter;
                //if (isRobotWinner)
                printf("%2d %c %s:  %2d == %2d turns: %c -> %-16s %-16s  %5d\n"
                    , ++j
                    , (branch->getIsWinner() ? 'r' : 'H')
                    , (isRobotWinner ? "roBOT" : "HUMan")
                    , branch->getEndDepth()
                    , numTurns
                    , branch->getLetter()
                    , (aLastWord ? aLastWord : "----")
                    , branch->getEndWord()
                    , fsCounter 
                    );
                if (  isRobotWinner && (numTurns % 2) == 0) {
                    dbgRobotWinsEven++;
                }
                if (! isRobotWinner && (numTurns % 2) == 1) {
                    dbgHumanWinsOdd++;
                }
                int lastLen =  aLastWord ? strlen(aLastWord) : 0;
                if (numTurns != lastLen) {
                    dbgNumTurnsNeStrlen++;
                }
                if (isRobotWinner &&  ! branch->getIsWinner()) {
                    dbgRobotNotBranch++;
                }
                if ( ! isRobotWinner && branch->getIsWinner()) {
                    dbgBranchNotRobot++;
                }
                if (branch->getEndDepth() != numTurns) {
                    dbgNumTurnsNeEndDepth++;
                }
            }
            printf("%s takes %c, max turns %d:    %s\n", (robotWins > 0 ? "ROBOT (Player 2)" : "HUMAN (Player 1)")
                , toupper(c), tmpDepth, maxWord); 
        }
        if (dbgRobotNotBranch || dbgBranchNotRobot) {
            printf("Bugs: RobotNotBranch: %d  BranchNotRobot: %d\n", dbgRobotNotBranch, dbgBranchNotRobot);
        }
        if (dbgNumTurnsNeEndDepth) {
            printf("Bugs: Of %d trials, TurnsNEmSteps: %d\n", numCombos, dbgNumTurnsNeEndDepth);
        }
        return 0;
    }
     
    int playFive() {
        for (int n = 1; n <= 5; n++) {
            printf("\nStarting game %d of 5.  Please enter a letter, Human . . .\n", n);
            char gameWord[BUFSIZE] = { 0, }, entry[BUFSIZE], *pcEntry;
            int  numWinningBranches = 0;
            const EvOdNode *endBranch = NULL;
            for(int j = 0; j <= 28; j += 2) {
                char ch = 0; 
                for (;;) {
                    pcEntry = gets_s(entry, BUFSIZE);
                    ch = (char)tolower(entry[0]);
                    if ('a' <= ch && ch <= 'z') {
                        break;
                    }
                    //printf ("You entered ch(%d) == %c, %s\n", ch, ch, ('a' <= ch && ch <= 'z') ? "ok." : "try again!");
                    printf ("    You entered %c (%d), which is not a letter; please try again . . .\n", ch, ch);
                }
                gameWord[j] = ch;
                printf ("    You entered %c, so the stem is now: %s\n", ch, gameWord);
                const EvOdNode *gameNode = dynamic_cast<const EvOdNode *>(subTrix(gameWord, j));
                if (gameNode == NULL || gameNode->isWord()) {
                    printf("    GAME OVER at turn %d: The Robot wins because \"%s\" %s word.\n"
                        , j, gameWord, (gameNode ? "is a" : "begins no") );
                    break;
                }
                char letter = '_';
                for (const EvOdNode *branch = gameNode->getFirstChild(); branch != NULL; branch = branch->getNextSibling()) {
                    WordLenT gameTurns = 444;
                    const char *gameEndString = NULL;
                    bool isWinner = branch->evalIsWinnerGetEnds(gameTurns, &gameEndString);
                    letter        = branch->getLetter();
                    printf("%s %s + %c ==> %-20s %20s\n"
                        , (isWinner ? "WINNER:" : "LOSER: ")
                        , gameWord, letter, branch->getEndWord(), gameEndString );
                    // save the last winner or, if there are none, save the last loser...
                    if (isWinner) {
                        numWinningBranches++;
                        endBranch = branch;
                    } else if (numWinningBranches == 0) {
                        endBranch = branch;
                    }
                }
                letter = endBranch->getLetter();
                gameWord[j+1] = letter;
                printf("    The Robot adds the letter %c, as in \"%s\", which would make the Robot a %s.\n"
                    , letter, endBranch->getEndWord(), (numWinningBranches > 0 ? "winner" : "loser") );
                if (endBranch->isWord()) {
                    printf("    GAME OVER at turn %d: %c makes %s a word, so YOU WIN, Human!\n"
                        , j, endBranch->getLetter(), endBranch->getWord() );
                    break;
                } else {
                    printf("    Enter a letter to follow \"%s\" . . .\n", gameWord);
                }
            }
        }
        return 0;
    }
};

int main(int argc, char* argv[])    // NB: This is more a unit test than an app; it does not play ghost!
{
    const char *dictionaryFilePath = (argc > 0 && argv[1]) ? argv[1] : defDictionaryFile;
    bool just_test = false;
    if ( just_test ) {
        //test_spraxUtil();
        test_SubWords(dictionaryFilePath);
#ifdef _DEBUG
        printf("Hit <RETURN> to quit . . .\n");
        getchar();
#endif
        exit(0);    // zoid

    } else {
        const int     minWordLen = 4;
        WordMap     * pWordMap = NULL;
        GhostTrie     ghostTrie;
        int maxWordLen = EvOdNodeTrie::initTrieFromSortedDictionaryFile(&ghostTrie, pWordMap, minWordLen, dictionaryFilePath);
        if (maxWordLen <= 0) {
            exit(1);
        }

        ghostTrie.unitTest();
        ghostTrie.playFive();
        Sleep(9999);
    }
    return 0;
}

