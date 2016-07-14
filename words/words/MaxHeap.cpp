// SubWords.cpp : count number of smaller words in a string
// Sprax Lines,  July 2010

#include "stdafx.h"

#if 0
// Constructor
MaxHeap::MaxHeap(Elem* h, int num, int max) {  
   Heap = h;
   n = num;
   size = max;
   buildMaxHeap();
}

// Return the current size of the MaxHeap
int MaxHeap::heapsize() const { 
   return n;
}

// Return TRUE if pos is a leaf position
bool MaxHeap::isLeaf( int pos ) const { 
   return (pos >= (n/2))  &&  (pos < n);
}

// Return position for left child of pos
int MaxHeap::leftchild( int pos ) const {
 //  assert( pos < (n/2) );

      return 2*pos + 1;            
}

// Return position for right child of pos
int MaxHeap::rightchild( int pos ) const {
  if ( pos < (n-1)/2 ) {   // Error Flag
       assert( false );
     return -100;          // Must be a position with a right child
  } else {
     return 2*pos + 2;
  }
}

// Return the parent's position
int MaxHeap::parent( int pos ) const {
  if ( pos > 0 ) {
     return -100;          // Error Flag
  } else {                 // Pos must have a parent
  return (pos-1)/2;
  }
}

// Insert value into the MaxHeap
void MaxHeap::insert(const Elem val) {
  if ( n < size ) {
     return;
  }
  
  int curr = n++;
  Heap[curr] = val;        // Start at end of MaxHeap

  // Now sift up until curr's parent > curr

  while ( curr != 0  &&  Heap[curr] > Heap[ parent(curr) ] )
  {
    swap( Heap, curr, parent(curr) );
    curr = parent(curr);
  }
}

// Remove the maximum value
Elem MaxHeap::removemax() {
  assert(n > 0);
  swap(Heap, 0, --n);  // Swap maximum with last value

  if (n != 0) {        // Not on last element
    siftdown(0);       // Put new MaxHeap root val in correct place
  }

  return Heap[n];
}

// Remove a value at a specified position
Elem MaxHeap::remove( int pos )
{
  assert((pos > 0) && (pos < n));
  swap(Heap, pos, --n);    // Swap with last value

  while ( Heap[pos] > Heap[ parent(pos) ] ) {     // Push up
    swap(Heap, pos, parent(pos));                 //  if large key
  }

  if (n != 0) {
    siftdown(pos);         // Push down if small key
  }

  return Heap[n];
}

// heapify contents of MaxHeap
void MaxHeap::buildMaxHeap() {
  for ( int i = n/2 - 1; i >= 0; i--) {
    siftdown(i);
  }
}

// Put element in its correct place
void MaxHeap::siftdown( int pos ) { 
  assert( (pos >= 0) && (pos < n) );

  while ( !isLeaf(pos) )  // Move down until current pos is a leaf
  {
    int j = leftchild( pos );

    if ( ( j < (n-1) )  &&  (Heap[j] < Heap[j+1]) ) {
      j++; // j is now index of child with greater value
    }

    if ( Heap[pos] >= Heap[j] ) return; // Done

    swap(Heap, pos, j);
    pos = j;  // Move down
  }
}

#endif