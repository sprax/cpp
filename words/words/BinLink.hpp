#ifndef BinLink_hpp
#define BinLink_hpp
#include <list>
#include <queue>


class BinLink {
	
public:
	int           mData;
    BinLink     * mLeft;
	BinLink     * mRight;
	
    int      data()  const { return mData;  } 
    BinLink *left()  const { return mLeft;  } 
    BinLink *right() const { return mRight; }
	
    // default constructor with null std string // TODO: eliminate this?
	BinLink() : mData(-1), mLeft(NULL), mRight(NULL)
    {}
	
    // constructor that copies string data into new std string
    BinLink(int data, BinLink *lt=NULL, BinLink *rt=NULL) : mData(data), mLeft(lt), mRight(rt)
    {}

    char toLetter() const { return 'A' + mData % 26; }
    
	
    int getNumNodes() {
        return 1 + getNumDescendents();
    }

    int getNumDescendents() {
        return (mLeft  == NULL ? 0 : 1 +  mLeft->getNumDescendents())
             + (mRight == NULL ? 0 : 1 + mRight->getNumDescendents());
    }

    static int getNumNodesStatic(BinLink * b) {
        if (b == NULL) 
            return 0;
        return 1 + getNumNodesStatic(b->mLeft) + getNumNodesStatic(b->mRight);
    }
    
    static int max(int a, int b) {
        return a > b ? a : b;
    }
    
    int getDepth() {
        return max( (mLeft  == NULL ? 0 : 1 +  mLeft->getDepth())
                  , (mRight == NULL ? 0 : 1 + mRight->getDepth()));
    }

    static int getDepthStatic(BinLink * binLink) {
        if (binLink == NULL || (binLink->mLeft == NULL && binLink->mRight == NULL)) {
            return 0;
        }
        if (binLink->mLeft == NULL) {
            return 1 + getDepthStatic(binLink->mRight);
        } else if (binLink->mRight == NULL) {
            return 1 + getDepthStatic(binLink->mLeft);
        } else {
            return 1 + max( getDepthStatic(binLink->mLeft), getDepthStatic(binLink->mRight) );
        }    
    }
    
    static int mCount;
    static int setCount(int num) { return mCount = num; }
    static int getCount()        { return mCount; }
    static int addCount(int num) { return mCount += num; }    
    	
	static void printList(BinLink *head);
	static void printListAsLetters(BinLink *head);
    static void printDepthFirstIterativePreOrder(const BinLink *root);
    	
    static BinLink * growBreadthFirstQueueLevelOrder(int depth, int count);  
    BinLink * BinLink::growGlobalCountingBinTreeRecurse(int depth, int n);


#if 0	
    public static BinLink growGlobalCountingBinTreeRecurse(int depth, int n) {
        BinLink root = new BinLink(getCount(), NULL, NULL);
        if (depth-- > 0) {
            root.left  = growGlobalCountingBinTreeRecurse(depth, addCount(1));
            root.right = growGlobalCountingBinTreeRecurse(depth, addCount(1));
        }   // otherwise, binLink->mLeft = binLink->mRight = NULL (by construction)
        return root;
    }
    
    public static BinLink growRandomBinTreeRecurse(int depth, Random rng, int n) {
        BinLink b = new BinLink(rng.nextInt(n), NULL, NULL);
        if (depth-- > 0) {
            binLink->mLeft  = growRandomBinTreeRecurse(depth, rng, n);
            binLink->mRight = growRandomBinTreeRecurse(depth, rng, n);
        }   // otherwise, binLink->mLeft = binLink->mRight = NULL (by construction)
        return b;
    }    
    
    
	public static BinLink initAlphabet(int length) {
		assert(length > 0);
		length--;
		BinLink head = new BinLink('A' + length, NULL, NULL);
		while(--length >= 0) {
		     BinLink temp = new BinLink('A' + length, head, NULL);
		     head.left = temp;
		     head = temp;
        }
		head.left = NULL;
		return head;
	}

    
    public static void printTreeDepthFirstRecursivePreOrder(BinLink head) {
        if (head != NULL) {
            System.out.print(head.mData + " ");
            printTreeDepthFirstRecursivePreOrder(head.left);
            printTreeDepthFirstRecursivePreOrder(head.right);
        }
    }
    
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

    public void printDepthFirstIterativePreOrder() {
        Stack<BinLink> nodeStack = new Stack<BinLink>();
        nodeStack.push(this);
        while (! nodeStack.isEmpty()){
            BinLink currentNode = nodeStack.pop();
            System.out.print(currentNode.mData + " ");
            BinLink right = currentNode.right();
            if (right != NULL) {
                nodeStack.push(right);
            }
            BinLink left = currentNode.left();
            if (left != NULL) {
                nodeStack.push(left);
            }
        }
    } 

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
    public:

        static int unit_test();

};




#endif // BinLink_hpp