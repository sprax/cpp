
// MaxHeap.hpp - Interface of a max heap class

#ifndef __J2K__MAXHEAP_HPP__
#define __J2K__MAXHEAP_HPP__   // To avoid multiple declaration of a class

#if 0

class MaxHeap {                // Max-heap class

public:
  MaxHeap( Elem*, int, int );  // Constructor

  int  heapsize() const;       // Return current size of the heap
  bool isLeaf(int) const;      // TRUE if pos is a leaf position
  int  leftchild(int) const;   // Return position for left child
  int  rightchild(int) const;  // Return position for right child
  int  parent(int) const;      // Return position for parent of pos
  void insert(const Elem);     // Insert value into the maxheap
  Elem removemax();            // Remove maximum value
  Elem remove(int);            // Remove value at specified position
  void buildMaxHeap();         // Heapify contents of Heap

private:
  Elem* Heap;                  // Pointer to the heap array
  int   size;                  // Maximum size of the heap
  int   n;                     // Number of elements now in the heap
  void  siftdown(int);         // Put an element in its correct place
};
#endif
#endif
