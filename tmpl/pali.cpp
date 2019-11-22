
// from MB 2019.10.04

class Solution {
public:


    bool isPalindrome(string& s) {
      if (!s.size())
        return true;

      /* Using two indeces */
      int i = 0;
      int j = s.size() -1;

      char I;
      char J;

      while (i < j)
      {
        /* could rather make a function encapsulating below if/else ifs

          pair<char,char> getNext(int& i, int& j, string& s);

          or even return a bool

          or char getNext(int& i, bool incrementing, string& s);

        */
        if (((s[i] >= 'a') && (s[i] <= 'z')) || ((s[i] >= '0') && (s[i] <= '9')))
        {
          I = s[i];
        } else if (((s[i] >= 'A') && (s[i] <= 'Z')))
        {
          I = s[i] - ('A' - 'a');
        } else {
          i++;
          continue;
        }
        if (((s[j] >= 'a') && (s[j] <= 'z')) || ((s[j] >= '0') && (s[j] <= '9')))
        {
          J = s[j];
        } else if (((s[j] >= 'A') && (s[j] <= 'Z')))
        {
          J = s[j] - ('A' - 'a');
        } else {
          j--;
          continue;
        }
        if (I != J)
          return false;
        i++; j--;
      }



     /* Using a deque

      deque<char> d1;

      for (int i = 0; i < s.size(); i++)
      {
        if (((s[i] >= 'a') && (s[i] <= 'z')) || ((s[i] >= '0') && (s[i] <= '9')))
        {
          d1.push_back(s[i]);
        } else if (((s[i] >= 'A') && (s[i] <= 'Z')))
        {
          d1.push_back(s[i] - ('A' - 'a'));

        }
      }
      while (d1.size() > 1)
      {
        if (d1.front() != d1.back())
          return false;
        d1.pop_front();
        d1.pop_back();
      }

      */


      /*  Building 2 stacks

      stack<char> s1;
      stack<char> s2;

      bool odd = false;

      for (int i = 0; i < s.size(); i++)
      {
        if (((s[i] >= 'a') && (s[i] <= 'z')) || ((s[i] >= '0') && (s[i] <= '9')))
        {
          s1.push(s[i]);
        } else if (((s[i] >= 'A') && (s[i] <= 'Z')))
        {
          s1.push(s[i] - ('A' - 'a'));

        }
      }
      odd = s1.size() % 2 ? true : false;

      while (s1.size() > s2.size() + (odd?1:0))
      {
        s2.push(s1.top());
        s1.pop();
      }
      if (odd)
        s1.pop();


      while (!s1.empty())
      {
        if (s1.top() != s2.top())
          return false;
        s1.pop();
        s2.pop();
      }
      */

      return true;

    }
};
