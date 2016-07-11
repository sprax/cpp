
#include <iostream>
#include "WordTrie.hpp"
#include "CharFreqMap.hpp"

using namespace std;

struct TreeNode
{
    enum Kind {RED, BLUE};

    TreeNode(Kind kind_, TreeNode* left_ = NULL, TreeNode* right_ = NULL)
        : kind(kind_), left(left_), right(right_)
    {}

    Kind kind;
    TreeNode *left, *right;
};

template <typename NodeActor>
class Visitor
{
public:

    void visit_preorder(TreeNode* node);

    void visit_inorder(TreeNode* node);

    void visit_postorder(TreeNode* node);

private:

    NodeActor      mNodeActor;

    void dispatch_node(TreeNode* node)
    {
        switch (node->kind) {
            case TreeNode::RED:
                mNodeActor.handle_RED(node);
                break;
            case TreeNode::BLUE:
                mNodeActor.handle_BLUE(node);
                break;
            default:
                assert(0);
        }
    }
};

struct GenericNodeActor
{
    void handle_RED(TreeNode* node)
    {
        cerr << node->kind << " Generic handle RED\n";
    }

    void handle_BLUE(TreeNode* node)
    {
        cerr << node->kind << "Generic handle BLUE\n";
    }
};

struct SpecialNodeActor : public GenericNodeActor
{
    void handle_RED(TreeNode* node)
    {
        cerr << node->kind << "RED is special\n";
    }
};