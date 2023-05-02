#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE), top(NULL), size(0) {}

void StackFrame::run(string filename) {
    ifstream FILE(filename);
    string line;
    string cmd[3] = { "NULL" };
    //cmd[0]: store type (int/float)
    //cmd[1]: store command 
    //cmd[2]: store <ind>, <val>
    int cnt = 0;
    while (!FILE.eof()) {
        for (int i = 0; i < 3; i++) cmd[i] = "NULL";
        getline(FILE, line);
        cnt++;
        if (line[0] == 'i' || line[0] == 'f') {
            cmd[0] = line[0];
            cmd[1] = line.substr(1, line.find(' ') - 1);
            if (cmd[1] == "const" || cmd[1] == "load" || cmd[1] == "store") {
                cmd[2] = line.substr(line.find(' ') + 1, line.length() - line.find(' '));
            }
        }
        else if (line.substr(0, 3) == "val") {
            cmd[1] = "val";
            cmd[2] = line.substr(line.find(' ') + 1, line.length() - line.find(' '));
        }
        else {
            cmd[1] = "top";
        }
        //Commands
        if (cmd[1] == "add") {
            this->add(cmd[0], cnt);
        }
        else if (cmd[1] == "sub") {
            this->sub(cmd[0], cnt);
        }
        else if (cmd[1] == "mul") {
            this->mul(cmd[0], cnt);
        }
        else if (cmd[1] == "div") {
            this->div(cmd[0], cnt);
        }
        else if (cmd[1] == "rem") {
            this->rem(cmd[0], cnt);
        }
        else if (cmd[1] == "neg") {
            this->neg(cmd[0], cnt);
        }
        else if (cmd[1] == "and") {
            this->bitwiseAnd(cmd[0], cnt);
        }
        else if (cmd[1] == "or") {
            this->bitwiseOr(cmd[0], cnt);
        }
        else if (cmd[1] == "eq") {
            this->eq(cmd[0], cnt);
        }
        else if (cmd[1] == "neq") {
            this->neq(cmd[0], cnt);
        }
        else if (cmd[1] == "lt") {
            this->lt(cmd[0], cnt);
        }
        else if (cmd[1] == "gt") {
            this->gt(cmd[0], cnt);
        }
        else if (cmd[1] == "bnot") {
            this->bnot(cmd[0], cnt);
        }
        else if (cmd[1] == "const") {
            this->cons(cmd[0], stof(cmd[2]), cnt);
        }
        else if (cmd[1] == "load") {
            this->load(cmd[0], stoi(cmd[2]), cnt);
        }
        else if (cmd[1] == "store") {
            this->store(cmd[0], stoi(cmd[2]), cnt);
        }
        else if (cmd[1] == "2f") {
            this->i2f(cnt);
        }
        else if (cmd[1] == "2i") {
            this->f2i(cnt);
        }
        else if (cmd[1] == "top") {
            this->getTop(cnt);
        }
        else this->val(stoi(cmd[2]), cnt);
    }
    FILE.close();
}

void StackFrame::pop() {
    node* tmp = this->top;
    this->top = this->top->next;
    this->size--;
    delete tmp;
}

void StackFrame::push(node* newNode) {
    newNode->next = this->top;
    this->top = newNode;
    this->size++;
}
//
void StackFrame::add(string valType, int line) {
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //iadd
            if (top->type == 0 && top->next->type == 0) {   
                int pNew_value = (int)top->value + (int)top->next->value;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));  
        }
        else {      //fadd
            float pNew_value = top->value + top->next->value;
            node* pNew = new node(pNew_value, 1, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::sub(string valType, int line) {
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //isub
            if (top->type == 0 && top->next->type == 0) {   
                int pNew_value = (int)top->next->value - (int)top->value;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));   //invalid
        }
        else {      //fsub
            float pNew_value = top->next->value - top->value;
            node* pNew = new node(pNew_value, 1, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::mul(string valType, int line) {
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //mul
            if (top->type == 0 && top->next->type == 0) {   
                int pNew_value = (int)top->value * (int)top->next->value;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));  
        }
        else {      //fmul
            float pNew_value = top->value * top->next->value;
            node* pNew = new node(pNew_value, 1, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::div(string valType, int line) {    //SE -> TMM -> DBZ
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //idiv
            if (top->type == 0 && top->next->type == 0) {
                if (top->value == 0) throw(DivideByZero(line));
                else {
                    int pNew_value = (int)top->next->value / (int)top->value;
                    node* pNew = new node(pNew_value, 0, NULL);
                    pop();
                    pop();
                    push(pNew);
                }
            }
            else throw(TypeMisMatch(line));
        }
        else {      //fdiv
            if (top->value == 0) throw(DivideByZero(line));
            else {
                float pNew_value = top->next->value / top->value;
                node* pNew = new node(pNew_value, 1, NULL);
                pop();
                pop();
                push(pNew);
            }
        }
    }
}

void StackFrame::rem(string valType, int line) {    //SE -> TMM -> DBZ
    if (this->size < 2) throw(StackEmpty(line));
    else {
        if (top->type == 0 && top->next->type == 0) {
            if (this->top->value == 0) throw(DivideByZero(line));
            else {
                int pNew_value = top->next->value - ((int)(top->next->value / top->value)) * top->value;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::neg(string valType, int line) {    //SE -> TMM
    if (this->size == 0) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {     //ineg
            if (top->type == 1) throw(TypeMisMatch(line));
            else top->value = -(int)(top->value);
        }
        else {  //fneg
            node* pNew = new node(-(top->value), 1, NULL);
            pop();
            push(pNew);
        }
    }
}

void StackFrame::bitwiseAnd(string valType, int line) {   //SE -> TMM
    if (this->size < 2) throw(StackEmpty(line));
    else {
        if (top->type == 0 && top->next->type == 0) {
            int pNew_value = (int)top->next->value & (int)top->value;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::bitwiseOr(string valType, int line) {    //SE -> TMM
    if (this->size < 2) throw(StackEmpty(line));
    else {
        if (top->type == 0 && top->next->type == 0) {
            int pNew_value = (int)top->next->value | (int)top->value;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::eq(string valType, int line) {     //SE -> TMM
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //ieq
            if (top->type == 0 && top->next->type == 0) {
                int pNew_value = (top->next->value == top->value) ? 1 : 0;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));
        }
        else {      //feq
            int pNew_value = (top->next->value == top->value) ? 1 : 0;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}
//from here
void StackFrame::neq(string valType, int line) {    //SE -> TMM
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //ineq
            if (top->type == 0 && top->next->type == 0) {
                int pNew_value = (top->next->value != top->value) ? 1 : 0;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));
        }
        else {      //fneq
            int pNew_value = (top->next->value != top->value) ? 1 : 0;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::lt(string valType, int line) {    //SE -> TMM 
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //ilt
            if (top->type == 0 && top->next->type == 0) {
                int pNew_value = (top->next->value < top->value) ? 1 : 0;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));
        }
        else {      //flt
            int pNew_value = (top->next->value < top->value) ? 1 : 0;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::gt(string valType, int line) {
    if (this->size < 2) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType == 0) {    //igt
            if (top->type == 0 && top->next->type == 0) {
                int pNew_value = (top->next->value > top->value) ? 1 : 0;
                node* pNew = new node(pNew_value, 0, NULL);
                pop();
                pop();
                push(pNew);
            }
            else throw(TypeMisMatch(line));
        }
        else {      //fgt
            int pNew_value = (top->next->value > top->value) ? 1 : 0;
            node* pNew = new node(pNew_value, 0, NULL);
            pop();
            pop();
            push(pNew);
        }
    }
}

void StackFrame::bnot(string valType, int line) {   //SE -> TMM
    if (this->size == 0) throw(StackEmpty(line));
    else {
        if (top->type == 0) {
            node* pNew = new node(-1, 0, NULL);
            pNew->value = (top->value == 0) ? 1 : 0;
            pop();
            push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}
//
void StackFrame::cons(string valType, float value, int line) {
    if (this->size == OPERAND_STACK_MAX_SIZE / 2) throw(StackFull(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        node* pNew = new node(value, getType, NULL);
        if (getType == 0) pNew->value = (int)pNew->value;
        push(pNew);
    }
}


void StackFrame::load(string valType, int index, int line) {
    if (localVarArr[index / 2].type == -1) throw(UndefinedVariable(line));
    else if (index / 2 > LOCAL_VARIABLE_ARRAY_SIZE / 2) throw(ArrayOutOfRange(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (localVarArr[index / 2].type == getType) {
            node* pNew = new node(localVarArr[index / 2].value, localVarArr[index / 2].type, NULL);
            if (this->size == OPERAND_STACK_MAX_SIZE / 2) throw(StackFull(line));
            else push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::store(string valType, int index, int line) {
    if (this->size == 0) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (top->type != getType) throw(TypeMisMatch(line));
        else {
            if (index / 2 > LOCAL_VARIABLE_ARRAY_SIZE / 2) throw(ArrayOutOfRange(line));
            else {
                localVarArr[index / 2].value = top->value;
                localVarArr[index / 2].type = top->type;
                pop();
            }
        }
    }
}

void StackFrame::f2i(int line) {
    if (this->size == 0) throw(StackEmpty(line));
    else {
        if (top->type == 1) {
            node* pNew = new node((int)top->value, 0, NULL);
            pop();
            push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::i2f(int line) {
    if (this->size == 0) throw(StackEmpty(line));
    else {
        if (top->type == 0) {
            node* pNew = new node(top->value, 1, NULL);
            pop();
            push(pNew);
        }
        else throw(TypeMisMatch(line));
    }
}

void StackFrame::getTop(int line) {
    if (this->size == 0) throw(StackEmpty(line));
    else {
        if (top->type == 1) cout << this->top->value << '\n';
        else cout <<  (int)top->value << '\n';
    }
}

void StackFrame::val(int index, int line) {
    if (index / 2 > LOCAL_VARIABLE_ARRAY_SIZE / 2) throw(ArrayOutOfRange(line));
    else {
        if (localVarArr[index / 2].type == -1) throw(UndefinedVariable(line));
        else cout << localVarArr[index / 2].value << '\n';   
    }
}
