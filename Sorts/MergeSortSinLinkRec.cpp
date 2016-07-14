// MergeSortSinLink.cpp : Adapted from GeeksForGeeks
#include "stdafx.h"

#include "MergeSortSinLinkRec.h"
#include "MergeSortSinLinkTest.h"

/* Find the middle link and split the list there,
 * returning the middle link's next pointer as the
 * starting link of the list's second half.
 * Uses the fast/slow pointer tactic, which is 
 * less efficient than just counting once and then
 * repeatedly dividing by 2.
*/
SinLink *findMiddleLinkAndSplitList(SinLink* list)
{
    // handle base cases:
    if (list == NULL || list->next == NULL)
    {
        return NULL;
    }
    else
    {
        SinLink *slow = list;
        SinLink *fast = list->next;

        /* Advance 'slow' runner one node for every two nodes by the 'fast' runner: */
        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                fast = fast->next;
                slow = slow->next;
            }
        }

        /* 'slow' is before the midpoint in the list, so split it in two at that point. */
        SinLink *middle = slow->next;
        slow->next = NULL;
        return middle;
    }
}


// MoveNode() function takes the node from the front of the source, and moves it to the front of the dest.    
// It is an error to call this with the source list empty.      
// Before calling MoveNode():    source == {1, 2, 3}  dest == {4, 5, 6}      
// After calling MoveNode():     source == {2, 3}     dest == {1, 4, 5, 6} 
void MoveNodePtrPtr(SinLink** destRef, SinLink** sourceRef)
{
    SinLink* newNode = *sourceRef;
    assert(newNode != NULL);

    // Advance the source pointer
    *sourceRef = newNode->next;

    // Link the dest list to follow off the moving node
    newNode->next = *destRef;

    // Mmake dest point to the moved node
    *destRef = newNode;
}

SinLink* SortedMergePtrPtr(SinLink* a, SinLink* b)
{
    SinLink* result = NULL;       /* point to the last result pointer */
    SinLink** lastPtrRef = &result;
    while (1)   
    {
        if (a == NULL)
        {
            *lastPtrRef = b;
            break;
        }
        else if (b == NULL)
        {
            *lastPtrRef = a;
            break;
        }
        if (a->data <= b->data)
        {
            MoveNodePtrPtr(lastPtrRef, &a);
        }
        else
        {
            MoveNodePtrPtr(lastPtrRef, &b);
        }
        /* tricky: advance to pointer to the next ".next" field */
        lastPtrRef = &((*lastPtrRef)->next);
    }  
    return(result);
}

// @Deprecated.  Recursion uses stack space in proportion
// to the length of the linked list,
// so use this.
// See http://geeksforgeeks.org/?p=3622 for details of this function 
static SinLink* SortedMergeRecursive(SinLink* a, SinLink* b)
{
    SinLink* result = NULL;

    /* Base cases */
    if (a == NULL)
        return(b);
    else if (b == NULL)
        return(a);

    /* Pick either a or b, and recur */
    if (a->data <= b->data)
    {
        result = a;
        result->next = SortedMergeRecursive(a->next, b);
    }
    else
    {
        result = b;
        result->next = SortedMergeRecursive(a, b->next);
    }
    return(result);
}

// @Deprecated  
// Sorts a singly-linked list by changing next pointers (not data),
// and without computing its length, for better or worse.
void mergeSortSinLinkRecursiveNoCount(SinLink** pHead)
{
    SinLink* head = *pHead;

    // base case -- length 0 or 1
    if ((head == NULL) || (head->next == NULL))
    {
        return;
    }

    // Split list into first and second halves:
    SinLink* half1 = head;
    SinLink* half2 = findMiddleLinkAndSplitList(head);

    // Recursively sort the sublists:
    mergeSortSinLinkRecursiveNoCount(&half1);
    mergeSortSinLinkRecursiveNoCount(&half2);

    // Merge the two sorted lists together
    *pHead = SortedMergeRecursive(half1, half2);
}



int test_mergeSortSinLinkRec(int level)
{
	puts("test_mergeSortSinLink:");
    test_mergeSortSinLinkToy();
    test_mergeSortSinLinkRand(100);
    return 0;
}



void MergeSortSinLinkRec::mergeSort(SinLink **list)
{
    mergeSortSinLinkRecursiveNoCount(list);
}
void MergeSortSinLinkRec::mergeSort(SinLink **list, size_t size)
{
    mergeSortSinLinkRecursiveNoCount(list);
}

