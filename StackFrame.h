#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
public:
    class node;
    class arr;
    arr* localVarArr = new arr[localVarArrSize/2];
private:
    node* top;
    int size;
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    //Support functions
    void pop();
    void push(StackFrame::node* newNode);
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
    void load(std::string valType, int index, int line);
    void store(std::string valType, int index, int line);
    //Type conversion Instructions
    void f2i(int line);
    void i2f(int line);
    //Operand Stack Management Instructions
    void getTop(int line);
    //Local Variable Management Instructions
    void val(int index, int line);
    //Destructor
    ~StackFrame() {
        while (top != NULL) pop();
        delete[] localVarArr;
        delete this->top;
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
public:
    class arr {
    public:
        float value;
        int type;
    public:
        arr() : value(0), type(-1) {};
    };
};


#endif // !__STACK_FRAME_H__