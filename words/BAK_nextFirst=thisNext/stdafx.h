// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <signal.h>


#include <string>
#include <iostream>
#include <map>

#include "linkedLists.h"

#include "trie.h"
#include "triesl.hpp"

void sigint_handler ( int sig );

void reverseWords(char *str);

void strrevptr(char *beg, char *end);
int  sttrevGood(char *str);            
void sttrevDumb(char *str);             
void strrevlenGood(char *str, int len); 
void strrevlenDumb(char *str, int len);

unsigned int lowerCaseChar(unsigned int c);
unsigned int upperCaseChar(unsigned int c);
char changeCaseChar(char c);

void upperCaseEvenIndexChars(char *pc);
void upperCaseEveryNthChars(char *pc, int nth);
void lowerCaseEveryMthButChangeCaseEveryNthChars(char *pc, int mth, int nth);
void test_trie_insert_print();

void test_charString();

// TODO: reference additional headers your program requires here
