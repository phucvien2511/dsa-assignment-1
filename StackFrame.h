#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
#include <stack>
/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
public:
    class node;
    class avlNode;
private:
    node* top;
    avlNode* root;
    int stackSize;
    int treeSize;

public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    //Support functions
    void pop();
    void push(StackFrame::node* newNode);
    int compareKey(std::string a, std::string b);
    int getHeight(StackFrame::avlNode* root);
    int getMax(int a, int b);
    int getBalance(StackFrame::avlNode* root);
    avlNode* leftRot(StackFrame::avlNode* root);
    avlNode* rightRot(StackFrame::avlNode* root);
    avlNode* insertAVL(StackFrame::avlNode* root, std::string key, float value, int type);
    avlNode* searchAVL(StackFrame::avlNode* root, std::string key);
    avlNode* searchPar(StackFrame::avlNode* root, std::string key);
    //Arithmetic Instructions
    void add(std::string valType, int line);
    void sub(std::string valType, int line);
    void mul(std::string valType, int line);
    void div(std::string valType, int line);
    void rem(std::string valType, int line);
    void neg(std::string valType, int line);
    void bitwiseAnd(std::string valType, int line);
    void bitwiseOr(std::string valType, int line);
    void eq(std::string valType, int line);
    void neq(std::string valType, int line);
    void lt(std::string valType, int line);
    void gt(std::string valType, int line);
    void bnot(std::string valType, int line);
    //Load and Store Instructions
    void cons(std::string valType, float value, int line);
    void load(std::string valType, std::string var, int line);
    void store(std::string valType, std::string var, int line);
    //Type conversion Instructions
    void f2i(int line);
    void i2f(int line);
    //Operand Stack Management Instructions
    void getTop(int line);
    //Local Variable Management Instructions
    void val(std::string var, int line);
    void par(std::string var, int line);
    void delTree(StackFrame::avlNode* root) {
        if (!root) return;
        delTree(root->left);
        delTree(root->right);
        delete root;
    }
    //Destructor
    ~StackFrame() {
        while (top != NULL) pop();
        delete this->top;
        delTree(this->root);
    };
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
    //Operand stack
public:
    class node {
    public:
        float value;
        int type;
        node* next;
    public:
        node() : value(0), type(-1), next(NULL) {};
        node(float value, int type, node* next) : value(value), type(type), next(NULL) {};
    };
    class avlNode {
    public:
        std::string key;
        float value;
        int type;
        int height;
        avlNode* left;
        avlNode* right;
    public:
        avlNode() : key(""), value(0), type(-1), height(1), left(NULL), right(NULL) {};
        avlNode(std::string key, float value, int type, int height, avlNode* left, avlNode* right) : key(key), value(value), type(type), height(1), left(left), right(right) {};
        
    };
};
 
#endif // !__STACK_FRAME_H__