#ifndef linkedLists_h
#define linkedLists_h

struct tSLL { int val; tSLL *nxt; };
struct tDLL { int val; tDLL *nxt;  tDLL *prv; };

tSLL * string2SLL(const char *str, int length);
void printSLL(const tSLL *nod);
void printDLL(const tDLL *head, const tDLL *tail);
void reverseSLL(tSLL **head);
void reverseDLL(tDLL **head, tDLL **tail);
tSLL *reverseP(tSLL *itr);
bool isPalindrome(tSLL *lst, int len);

#endif // linkedLists_h