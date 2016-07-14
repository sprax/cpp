// MergeSortSinLink.cpp : Adapted from GeeksForGeeks
#include "stdafx.h"

#include "MergeSortSinLinkRec.h"
#include "MergeSortSinLinkTest.h"



/* UTILITY FUNCTIONS */

/* Function to print nodes in a given linked list */
void printSinLinkList(SinLink *SinLink)
{
    while (SinLink != NULL)
    {
        printf("%d ", SinLink->data);
        SinLink = SinLink->next;
    }
    printf("\n");
}

/* Function to insert a SinLink at the beginging of the linked list */
void pushSinLink(SinLink** head_ref, int data)
{
    SinLink *link = new SinLink(data, *head_ref);

    /* move the head to point to the new SinLink */
    *head_ref = link;
}


// duplicate list iteratively
SinLink *dupeList(SinLink *list)
{
    if (list == NULL)
        return  NULL;
    SinLink *copy = new SinLink(list->data, NULL);
    SinLink *iter = copy;
    list = list->next;
    while (list != NULL)
    {
        iter->next = new SinLink(list->data, NULL);
        iter = iter->next;
        list = list->next;
    }
    return copy;
}



static void copyListRec(SinLink **dst, SinLink *src)
{
    if (src == NULL)
        return;
    copyListRec(dst, src->next);
    pushSinLink(dst, src->data);
}

SinLink *dupeListRec(SinLink *src)
{
    if (src == NULL)
        return NULL;

    SinLink *dst = NULL;
    copyListRec(&dst, src->next);
    pushSinLink(&dst, src->data);
    return dst;
}


bool isSorted(SinLink *list)
{
    if (list == NULL)
        return false;
    int oldVal = list->data;
    list = list->next;
    while (list != NULL)
    {
        int newVal = list->data;
        if (oldVal > newVal)
            return false;
        list = list->next;
    }
    return true;
}

static SinLink *makeToyList()
{
    // Create an unsorted linked list to test:
    SinLink *list = NULL;       // empty list
    pushSinLink(&list, 15);
    pushSinLink(&list, 4);
    pushSinLink(&list, 10);
    pushSinLink(&list, 1);
    pushSinLink(&list, 5);
    pushSinLink(&list, 40);
    pushSinLink(&list, 20);
    pushSinLink(&list, 3);
    pushSinLink(&list, 5);
    pushSinLink(&list, 2);
    pushSinLink(&list, 30);
    return list;
}

class Base {
public:
    void setI(int i)  // No need for it to be virtual
    {
        m_i = i;
    }

    virtual ~Base() {}         // Almost always a good idea

    virtual bool isDerived1() = 0 // Is overridden - so make it virtual
    {
        return false;
    }

private:
    int m_i;
};

class Derived1 : public Base {
public:
    virtual ~Derived1() {}

    virtual bool isDerived1()  // Is overridden - so make it virtual
    {
        return true;
    }
};


void test_Base()
{
    Base *base = new Derived1();
}


bool test_mergeSortSinLink(ListSort<SinLink> *listSort, SinLink *list, size_t begSize)
{
    printf("\n The singly-linked list BEFORE sorting (size: %d):\n", begSize);
    printSinLinkList(list);
    listSort->sort(&list, begSize);

    size_t endSize = listSize(list);
    bool sorted = isSorted(list);
    printf("\n The singly-linked list AFTER  sorting (size: %d):  sorted=%d\n", endSize, sorted);
    printSinLinkList(list);
    return sorted;
}


int test_mergeSortSinLinkToy()
{
    SinLink *list = makeToyList();
    size_t size = listSize(list);
    SinLink *copy = dupeListRec(list);
    SinLink *dupe = dupeList(list);

    size_t copySize = listSize(copy);
    MergeSortSinLink::mergeSort(&copy, copySize);
    printSinLinkList(copy);

    MergeSortSinLink *sortA = new MergeSortSinLink();
    test_mergeSortSinLink(sortA, list, size);

    ListSort<SinLink> *sortB = new MergeSortSinLinkRec();
    test_mergeSortSinLink(sortB, dupe, size);

    return 0;
}

int test_mergeSortSinLinkRand(size_t size)
{
    int *A = new int[size];
    for (size_t j = 0; j < size; j++)
    {
        A[j] = rand();
    }

    delete[] A;
    return 0;
}

int test_SinLinkMergeSorts(int level)
{
	puts("test_mergeSortSinLink:");
    test_mergeSortSinLinkToy();
    test_mergeSortSinLinkRand(100);
    return 0;
}



