// MergeSortSinLink.cpp : Adapted from GeeksForGeeks
#include "stdafx.h"

#include "MergeSortSinLink.h"
#include "MergeSortSinLinkTest.h"

SinLink *splitList(SinLink *list, unt size)
{
    assert(size > 0);
    while (--size > 0)
    {
        list = list->next;
    }
    SinLink *tail = list->next;
    list->next = NULL;
    return tail;
}

SinLink *MoveNode(SinLink** destRef, SinLink** sourceRef)
{
    SinLink* srcLink = *sourceRef;
    assert(srcLink != NULL);

    // Advance the source pointer
    *sourceRef = srcLink->next;

    // Link the dest list to follow off the moving node
    srcLink->next = *destRef;

    // Make dest point to the moved node
    *destRef = srcLink;

    return NULL;
}

// Merge sorted lists NIECE-ly, 
// where NIECE = No Input Error Checking or Emptiness allowed.
static SinLink* SortedMergeSrcIntoDstListsNIECE(SinLink* dst, SinLink* src)
{
    assert(dst != NULL && src != NULL && dst->data <= src->data);

    SinLink *result = dst;
#if 1
    do {
        SinLink *nxt = dst->next;
        if (nxt == NULL)
        {
            dst->next = src;
            break;
        }
        //printf("Comparing:  %2d  %2d\n", nxt->data, src->data);
        if (nxt->data <= src->data)
        {
            dst = nxt;
        }
        else
        {
            SinLink *tmp = src;
            src = src->next;
            dst->next = tmp;
            tmp->next = nxt;
            dst = tmp;
            if (src == NULL)
                break;
        }
    } while (dst != NULL);
#elif 0
    for (;;)
    {
        if (dst->data > src->data)
        {
            int val = dst->data;
            dst->data = src->data;
            src->data = val;

        }
        if (dst->next == NULL)
        {
            dst->next = src;
            break;
        }
        dst = dst->next;
    }
#else
    SinLink *linkA = dst, *linkB = src;
    linkA = linkA->next;
    for (SinLink *linkZ = dst; ; linkZ = linkZ->next) 
    {
        if (linkA->data <= linkB->data)
        {
            linkZ->next = linkA;
            if (linkA->next == NULL)
            {
                linkA->next = linkB;
                break;
            }
            else
            {
                linkA = linkA->next;
            }
        }
        else 
        {
            linkZ->next = linkB;
            if (linkB->next == NULL)
            {
                linkB->next = linkA;
                break;
            }
            else {
                linkB = linkB->next;
            }
        }
    }
#endif
    return(result);
}


// Sorts a singly-linked list by changing next pointers (not data).
// Uses pre-computed length, which is why it is an internal (static) method.
void mergeSortSinLink(SinLink** pHead, unt size)
{
    // base case 1
    if (size < 2)
        return;

    // base case 2
    SinLink* head = *pHead;
    if (size == 2)
    {
        SinLink *tail = head->next;
        if (head->data > tail->data)
        {
            head->next = NULL;
            tail->next = head;
            *pHead = tail;
        }
        return;
    }

    // Now we know size > 2.  Split list into first and second halves:
    unt headSize = size >> 1;                   // headSize > 0
    unt tailSize = size - headSize;             // tailSize > 1
    SinLink* tail = splitList(head, headSize);

    // Recursively sort the half-lists:
    mergeSortSinLink(&head, headSize);
    mergeSortSinLink(&tail, tailSize);

    // Merge the two sorted lists together.
    // We know that neither list is empty (size < 1).
    // Preserve stability by always merging tail into head,
    // so that two links have the same sort value, the one
    // that started left remains left.
    if (head->data > tail->data)
    {
        // pop the first link from tail
        SinLink *temp = tail;
        tail = tail->next;
        // push it in front of head
        temp->next = head;

        // If the tail had size 1, it now has size 0 and we're done;
        // otherwise merge the remainder of tail into head.
        if (tailSize == 1)
        {
            *pHead = temp;
            return;
        }
        else
        {
            head = temp;
        }
    }

    // We already know head and tail are non-empty, 
    // and that head->data <= tail->data
    *pHead = SortedMergeSrcIntoDstListsNIECE(head, tail);
}

// Exported mergeSort method: computes the list length itself,
// instead of trusting an external supplied length as argument.
static void mergeSortSinLink(SinLink** pHead)
{
    unt size = listSize(*pHead);
    mergeSortSinLink(pHead, size);
}


unt listSize(SinLink *list)
{
    unt size = 0;
    while (list != NULL)
    {
        list = list->next;
        size++;
    }
    return size;
}

void MergeSortSinLink::mergeSort(SinLink **list)
{
    mergeSortSinLink(list);
}
void MergeSortSinLink::mergeSort(SinLink **list, size_t size)
{
    mergeSortSinLink(list, size);
}


int test_mergeSortSinLink(int level)
{
	puts("test_mergeSortSinLink:");
    test_mergeSortSinLinkToy();
    test_mergeSortSinLinkRand(100);
    return 0;
}

