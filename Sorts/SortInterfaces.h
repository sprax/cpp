// sort.h

#pragma once

template <typename T>
class ArraySort
{
public:
    virtual void sort(T[], size_t size) = 0;
  //virtual bool isSorted(T[], size_t size) = 0;
};

template <typename T>
class ListSort
{
public:
    virtual void sort(T **list) = 0;
    virtual void sort(T **list, size_t size) = 0;
  //virtual bool isSorted(T **list, size_t size) = 0;
};

