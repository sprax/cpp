// BinLink.cpp : binary-link nodes to be used in binary trees or doubly-linked lists.

#include "stdafx.h"

#include <list>
#include <queue>
#include <stack>

#include "BinLink.hpp"

using namespace std;

int BinLink::mCount = 0;    // TODO: eliminate this, make it file-scope, or make it non-static member of BinTree

void BinLink::printList(BinLink *head) {
    while (head != NULL) {
        cout << head->data() << " ";
        head =  head->right();
    }
    cout << endl;
}

void BinLink::printListAsLetters(BinLink *head) {
    while (head != NULL) {
        cout << head->toLetter() << " ";
        head = head->mRight;
    }
    cout << endl;
}

void BinLink::printDepthFirstIterativePreOrder(const BinLink *root)
{
    stack<const BinLink*> *nodeStack = new stack<const BinLink*>();
    nodeStack->push(root);
    while (! nodeStack->empty()){
        const BinLink *currentNode = nodeStack->top();
        cout << currentNode->mData << endl;
        nodeStack->pop();
        BinLink *right = currentNode->right();
        if (right != NULL) {
            nodeStack->push(right);
        }
        BinLink *left = currentNode->left();
        if (left != NULL) {
            nodeStack->push(left);
        }
    }
} 



static BinLink *growRandomBinTreeRecurse(int depth, int n) 
{
    BinLink *binLink = new BinLink(rand() % n, NULL, NULL);
    if (depth-- > 0) {
        binLink->mLeft  = growRandomBinTreeRecurse(depth, n);
        binLink->mRight = growRandomBinTreeRecurse(depth, n);
    }   // otherwise, binLink->mLeft = binLink->mRight = NULL (by construction)
    return binLink;
}    
//
//static BinLink *initAlphabet(int length) 
//{
//    assert(length > 0);
//    length--;
//    BinLink *head = new BinLink('A' + length, NULL, NULL);
//    while(--length >= 0) {
//        BinLink *temp = new BinLink('A' + length, head, NULL);
//        head->left = temp;
//        head = temp;
//    }
//    head->left = NULL;
//    return head;
//}

#if 0    
	
	public static void printList(BinLink head) {
		while (head != NULL) {
			System.out.print(head.mData + " ");
			head = head.right;
		}
		printf();
	}
#endif
    
    static void printTreeDepthFirstRecursivePreOrder(BinLink *head) 
    {
        if (head != NULL) {
            printf("%2d ", head->mData);
            printTreeDepthFirstRecursivePreOrder(head->left());
            printTreeDepthFirstRecursivePreOrder(head->right());
        }
    }
    
#if 0
    public void printDepthFirstRecursivePreOrder() {
        System.out.print(mData + " ");
        if (left != NULL) {
            mLeft->printDepthFirstRecursivePreOrder();
        }
        if (right != NULL) {
            mRight->printDepthFirstRecursivePreOrder();
        }
    }
    
    public void printDepthFirstRecursiveInOrder() {
        if (left != NULL) {
            mLeft->printDepthFirstRecursiveInOrder();
        }
        System.out.print(mData + " ");
        if (right != NULL) {
            mRight->printDepthFirstRecursiveInOrder();
        }
    }
    
    public void printDepthFirstRecursivePostOrder() {
        if (left != NULL) {
            mLeft->printDepthFirstRecursivePostOrder();
        }
        if (right != NULL) {
            mRight->printDepthFirstRecursivePostOrder();
        }
        System.out.print(mData + " ");
    }
#endif


#if 0
    public void printDepthFirstIterativeInOrder() {
        Stack<BinLink> nodeStack = new Stack<BinLink>();
        nodeStack.push(this);
        if (left != NULL) {
            nodeStack.push(left);
        }            
        while (! nodeStack.isEmpty()){
            BinLink currentNode = nodeStack.pop();
            if (currentNode.visited == false) {
                System.out.print(currentNode.mData + " ");
                currentNode.visited = true;
            }
            if (currentNode.right != NULL) {
                nodeStack.push(currentNode.right);
            }
            if (currentNode.left != NULL) {
                nodeStack.push(currentNode.left);
            }
        }
    } 

    public void printDepthFirstIterativePostOrder() {
        Stack<BinLink> nodeStack = new Stack<BinLink>();
        boolean initState = this.visited;
        nodeStack.push(this);
        while (! nodeStack.empty()) {
            BinLink currentNode = nodeStack.peek();
            if ((currentNode.left != NULL) && (currentNode.mLeft->visited == initState)) {
                nodeStack.push(currentNode.left);
            } else {
                if ((currentNode.right != NULL) && (currentNode.mRight->visited == initState)) {
                    nodeStack.push(currentNode.right);
                } else {
                    System.out.print(currentNode.mData + " ");
                    currentNode.visited = ! initState;
                    nodeStack.pop();
                }
            }
        }
    }

    public void printBreadthFirstQueueLevelOrder() {
        LinkedList<BinLink> queue = new LinkedList<BinLink>();
        queue.add(this);
        do {
            BinLink node = queue.remove();
            System.out.print(node.mData + " ");
            if (node->mLeft != NULL) {
                queue.add(node->mLeft);
            }
            if (node->mRight != NULL) {
                queue.add(node->mRight);
            }
        } while ( ! queue.isEmpty());
    }
    
    public void printBreadthFirstQueueLevelSpaced(int depth) {
        LinkedList<BinLink> queue = new LinkedList<BinLink>();
        int counter = 0, nextPowerOf2 = 1;
        
//        char spaces[] = new char[80];
//        Arrays.fill(spaces, ' ');
//        if (depth < 0 || depth > 80) {
//            depth = 0;
//        }
//        String space = new String(spaces);
        
        System.out.format("\n%s", StringOfSpaces.get(depth));
        queue.add(this);
        do {
            BinLink node = queue.remove();
            System.out.print(node.mData + " ");
            if (++counter == nextPowerOf2) {
                counter = 0;
                depth = max(0, depth - nextPowerOf2);
                nextPowerOf2 *= 2;
                System.out.format("\n%s", StringOfSpaces.get(depth));
            }
            if (node->mLeft != NULL) {
                queue.add(node->mLeft);
            }
            if (node->mRight != NULL) {
                queue.add(node->mRight);
            }
        } while ( ! queue.isEmpty());
    }
    
    
	public static void shuffle (int[] array)
	{
	   Random rng = new Random();   // i.e., java.util.Random.
	   int n = array.length;        // The number of items left to shuffle (loop invariant).
	   while (n > 1)
	   {
	      int k = rng.nextInt(n);  // 0 <= k < n.
	      n--;                     // n is now the last pertinent index;
	      int temp = array[n];     // swap array[n] with array[k] (does nothing if k == n).
	      array[n] = array[k];
	      array[k] = temp;
	   }
	}

	
	/** generic?
	public static T[] RandomPermutation<T>(T[] array)
	{
	    T[] retArray = new T[array.Length];
	    array.CopyTo(retArray, 0);

	    Random random = new Random();
	    for (int i = 0; i < array.Length; i += 1)
	    {
	        int swapIndex = random.right(i, array.Length);
	        if (swapIndex != i)
	        {
	            T temp = retArray[i];
	            retArray[i] = retArray[swapIndex];
	            retArray[swapIndex] = temp;
	        }
	    }

        return retArray;
        }
        */

#endif

    BinLink * BinLink::growBreadthFirstQueueLevelOrder(int depth, int count) 
    {
        std::queue<BinLink *> queue;
        BinLink * root = new BinLink(getCount());
        queue.push(root);
        int last = count - 1;
        if (last < 0)
            last = 0;
        while ( ! queue.empty() && getCount() < count) {

            BinLink * node = queue.front();
            queue.pop();    // zoid -- needed after front?
            node->mLeft  = new BinLink(addCount(1), NULL, NULL);
            if (getCount() >= last)
                break;
            node->mRight = new BinLink(addCount(1), NULL, NULL);
            if (getCount() >= last)
                break;
            queue.push(node->mLeft);
            queue.push(node->mRight);
        }
        return root;
    }   



BinLink * BinLink::growGlobalCountingBinTreeRecurse(int depth, int n)
{
    BinLink  *root = new BinLink(getCount(), NULL, NULL);
    if (depth-- > 0) {
        root->mLeft  = growGlobalCountingBinTreeRecurse(depth, addCount(1));
        root->mRight = growGlobalCountingBinTreeRecurse(depth, addCount(1));
    }   // otherwise, binLink->mLeft = binLink->mRight = NULL (by construction)
    return root;
}



int BinLink::unit_test()
{
#ifdef _WIN32
    printf("BinLink::unit_test (file/func  %s : %s)\n", __FILE__, __FUNCTION__);
#else
    printf("BinLink::unit_test (file  %s)\n", __FILE__);
#endif
    std::queue<BinLink *> queue;
    bool em = queue.empty();

    BinLink * root = new BinLink();
    queue.push(root);

    BinLink * frn = queue.front();
    BinLink * nxt = queue.front();
    queue.pop();    // zoid -- needed after front?
    em = queue.empty();
    if ( ! em )
        BinLink * agn = queue.front();



    stack<int> *iStack = new stack<int>();
    iStack->push(1);
    iStack->push(2);
    iStack->push(3);
    printf("top: %d\n", iStack->top());
    iStack->pop();
    printf("top: %d\n", iStack->top());
    iStack->pop();
    printf("top: %d\n", iStack->top());
    iStack->pop();
    em = iStack->empty();
    if ( ! em )
        iStack->pop();


    BinLink *rootA = new BinLink(1, NULL, NULL);
    rootA->mLeft   = new BinLink(2, NULL, NULL);
    rootA->mLeft->mLeft   = new BinLink(3, NULL, NULL);
    rootA->mLeft->mRight  = new BinLink(4, NULL, NULL);
    rootA->mLeft->mRight->mRight  = new BinLink(5, NULL, NULL);
    rootA->mRight  = new BinLink(6, NULL, NULL);
    printDepthFirstIterativePreOrder(rootA);
    
    return 0;
}

/*
printList(initAlphabet(4));
BinLink list = NULL;
int array[] = { 0, 1, 2, 3, 4, 5 };
shuffle(array);
for (int j = 6; --j >= 0; ) {
BinLink temp = new BinLink(array[j] + 'a', list, NULL);
list = temp;
}
BinLink.printList(list);
*/	



