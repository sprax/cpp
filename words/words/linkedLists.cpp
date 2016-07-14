// words.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "linkedLists.h"


tSLL * string2SLL(const char *str, int length)
{
    assert(str && length > 0);
    tSLL *next = NULL;
    while (--length >= 0) {
        tSLL *sln = new tSLL();
        sln->val = str[length];
        sln->nxt = next;
        next = sln;
	}
    return next;
}

void printSLL(const tSLL *nod)
{
	while(nod) {
		printf("%c ", nod->val);
		nod = nod->nxt;
	}
	printf("\n");
}

void printDLL(const tDLL *head, const tDLL *tail)
{
	printf("tDLL[");
	while(head) {
		printf("%c ", head->val);
		head = head->nxt;
	}
	printf("\n");
	while(tail) {
		printf("%c ", tail->val);
		tail = tail->prv;
	}
	printf("]\n");
}

void reverseSLL(tSLL **head)
{
	tSLL *revl = NULL, *iter = *head, *temp;
	while (iter) {
        temp = iter;			// save current
		iter = iter->nxt;		// advance iter
		temp->nxt = revl;		// reverse the next ptr of the current node 
		revl = temp;			// make current the rev lists's head
	}
	*head = revl;
}

void reverseDLL(tDLL **head, tDLL **tail)
{
	tDLL *revl = NULL, *iter = *head, *temp;
	while (iter) {
        temp = iter;			// save current
		iter = iter->nxt;		// advance iter
		temp->nxt = revl;		// reverse next ptr of current 
		temp->prv = iter;		// reverse prev ptr of current 
		revl = temp;			// make current the rev lists's head
	}
	*tail = *head;
	*head = revl;
}


tSLL *reverseP(tSLL *itr) 
{ 
   tSLL *rev = NULL; 		// init reverse list
   while (itr) 
   { 
      tSLL *tmp = itr->nxt; 	// get ptr to next node before changing current node
      itr->nxt = rev; 		// reverse current node's next to point "backwards"
      rev = itr; 			// make current node the head of the reverse list
      itr = tmp; 			// advance current
   } 
   return rev; 
}

bool isPalindrome(tSLL *lst, int len)
{
    assert(lst && len > 0);
    tSLL *tmp, *itr = lst, *rev = NULL;
    int halfLen = len/2;
    for (int j = 0; j < halfLen; j++) {
        tmp = itr->nxt;
        itr->nxt = rev;
        rev = itr;
        itr = tmp;
    }
    while (rev != NULL) {
        assert(itr != NULL);
        if (rev->val != itr->val)
            return false;
        rev = rev->nxt;
        itr = itr->nxt;
    }
    return true;
}

bool isPalindrome(tSLL *list)
{
    return true;
}
