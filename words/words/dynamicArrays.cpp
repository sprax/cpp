/**********************************************

On Jul 14, 2016, at 12:08 AM, Sprax Lines <sprax@comcast.net> wrote :

Create a list, , of  empty sequences, where each sequence is indexed from  to.The elements within each of the  sequences also use - indexing.
Create an integer, , and initialize it to .
The  types of queries that can be performed on your list of sequences() are described below :
Query :
    Find the sequence, , at index  in .
    Append integer  to sequence .
Query :
      Find the sequence, , at index  in .
      Find the value of element  in(where  is the size of) and assign it to .
      Print the new value of  on a new line
      Task
      Given, , and  queries, execute each query.

  Note : is the bitwise XOR operation, which corresponds to the ^ operator in most languages.Learn more about it on Wikipedia.

         Input Format

         The first line contains two space - separated integers, (the number of sequences) and(the number of queries), respectively.
         Each of the  subsequent lines contains a query in the format defined above.
*******************************************/
#include "stdafx.h"

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int test_dynamicArrays() {
    int numSeq, numQry, qtype, x, y, index, last = 0;
    cin >> numSeq >> numQry;
    vector<vector<int>> seqList(numSeq);
    for (int q = 0; q < numQry; q++) {
        cin >> qtype >> x >> y;
        index = (x ^ last) % numSeq;
        if (qtype == 1) {
            seqList[index].push_back(y);
        }
        else {
            vector<int> seq = seqList[index];
            last = seq.at(y % seq.size());
            cout << last << endl;
        }
    }


    return 0;
}
