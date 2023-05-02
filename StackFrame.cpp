#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE), top(NULL), root(NULL), stackSize(0), treeSize(0) {}

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
        else if (line.substr(0, 3) == "val" || line.substr(0, 3) == "par") {
            cmd[1] = line.substr(0, 3);
            cmd[2] = line.substr(line.find(' ') + 1, line.length() - line.find(' '));
        }
        else {
            cmd[1] = "top";
        }
        //Commands
        for (int i = 0; i < 3; i++) {
            int len = cmd[i].length();
            if (cmd[i][len - 1] == '\r') cmd[i] = cmd[i].substr(0, len - 1);
        }
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
            this->load(cmd[0], cmd[2], cnt);
        }
        else if (cmd[1] == "store") {
            this->store(cmd[0], cmd[2], cnt);
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
        else if (cmd[1] == "par") {
            this->par(cmd[2], cnt);
            
        }
        else this->val(cmd[2], cnt);
        
    }
    FILE.close();
}
//Support functions
void StackFrame::pop() {
    node* tmp = this->top;
    this->top = this->top->next;
    stackSize--;
    delete tmp;
}

void StackFrame::push(node* newNode) {
    newNode->next = this->top;
    this->top = newNode;
    stackSize++;
}
//0: a < b ; 1: a > b ; 2: a = b
int StackFrame::compareKey(string a, string b) {
    int shorterLen = (a.length() <= b.length()) ? a.length() : b.length();
    for (int i = 0; i < shorterLen; i++) {
        if (a[i] != b[i]) {
            if ((int)(a[i]) > (int)(b[i])) return 1;   //a > b
            else return 0;  //a < b
        }
    }
    //Don't have different character
    if (a.length() > b.length()) return 1;  //a > b
    else if (a.length() < b.length()) return 0; //a < b
    else return 2;  //a = b
}

int StackFrame::getHeight(StackFrame::avlNode* root) {
    if (!root) return 0;
    return root->height;
}
int StackFrame::getMax(int a, int b) {
    int res = (a < b) ? b : a;
    return res;
}
int StackFrame::getBalance(StackFrame::avlNode* root) {
    if (!root) return 0;
    return getHeight(root->left) - getHeight(root->right);
}

StackFrame::avlNode* StackFrame::leftRot(StackFrame::avlNode* root) {
    avlNode* rNode = root->right;
    avlNode* lrNode = rNode->left;
    rNode->left = root;
    root->right = lrNode;
    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
    rNode->height = 1 + getMax(getHeight(rNode->left), getHeight(rNode->right));
    return rNode;
    
}

StackFrame::avlNode* StackFrame::rightRot(StackFrame::avlNode* root) {
    avlNode* lNode = root->left;
    avlNode* rlNode = lNode->right;
    lNode->right = root;
    root->left = rlNode;
    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
    lNode->height = 1 + getMax(getHeight(lNode->left), getHeight(lNode->right));
    return lNode;

}
//TODO: store a same key will replace an old infos with new one.
StackFrame::avlNode* StackFrame::insertAVL(avlNode* root, string key, float value, int type) {
    if (!root) {
        avlNode* pNew = new avlNode(key, value, type, 1, NULL, NULL);
        treeSize++;
        return(pNew);
    }
    if (compareKey(key, root->key) == 0) {  //key < root->key
        root->left = insertAVL(root->left, key, value, type);
    }
    else if (compareKey(key, root->key) == 1) { //key > root->key
        root->right = insertAVL(root->right, key, value, type);
    }
    else { // key = root->key
        root->key = key;
        root->value = value;
        root->type = type;
    }
    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
    if (getBalance(root) > 1 && compareKey(key, root->left->key) == 0) {     //LL
        return rightRot(root);
    }
    if (getBalance(root) < -1 && compareKey(key, root->right->key) == 1) {   //RR
        return leftRot(root);
    }
    if (getBalance(root) > 1 && compareKey(key, root->left->key) == 1) {    //LR
        root->left = leftRot(root->left);
        return rightRot(root);
    }
    if (getBalance(root) < -1 && compareKey(key, root->right->key) == 0) {   //RL
        root->right = rightRot(root->right);
        return leftRot(root);
    }
    treeSize++;
    return root;
    
}
StackFrame::avlNode* StackFrame::searchAVL(StackFrame::avlNode* root, string key) {
    if (!root || compareKey(key, root->key) == 2) return root;
    if (compareKey(key, root->key) == 0) return searchAVL(root->left, key);
    return searchAVL(root->right, key);
}

StackFrame::avlNode* StackFrame::searchPar(StackFrame::avlNode* root, string key) {
    if (!root || (!root->left && !root->right)) return NULL;
    if (root->left && compareKey(root->left->key, key) == 2) return root;
    if (root->right && compareKey(root->right->key, key) == 2) return root;
    if (compareKey(key, root->key) == 0) return searchPar(root->left, key);
    return searchPar(root->right, key);
}
//
void StackFrame::add(string valType, int line) {
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize == 0) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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

void StackFrame::neq(string valType, int line) {    //SE -> TMM
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize < 2) throw(StackEmpty(line));
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
    if (this->stackSize == 0) throw(StackEmpty(line));
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
    if (this->stackSize == OPERAND_STACK_MAX_SIZE / 2) throw(StackFull(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        node* pNew = new node(value, getType, NULL);
        if (getType == 0) pNew->value = (int)pNew->value;
        push(pNew);
    }
}

void StackFrame::load(string valType, string key, int line) {
    avlNode* tmp = searchAVL(root, key);
    if (!tmp) throw(UndefinedVariable(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (tmp->type == getType) {
            if (stackSize == OPERAND_STACK_MAX_SIZE) throw(StackFull(line));
            else {
                node* pNew = new node(tmp->value, getType, NULL);
                push(pNew);
            }
        }
        else throw(TypeMisMatch(line));
    }
}
void StackFrame::store(string valType, string key, int line) {
    if (stackSize == 0) throw(StackEmpty(line));
    else {
        int getType = (valType == "i") ? 0 : 1;
        if (getType != top->type) throw(TypeMisMatch(line));
        else {
            if (treeSize == LOCAL_VARIABLE_SPACE_SIZE / 2) throw(LocalSpaceFull(line));
            else {
                float getVal = top->value;
                pop();
                this->root = insertAVL(root, key, getVal, getType);
            }
        }       
    }
}

void StackFrame::f2i(int line) {
    if (stackSize == 0) throw(StackEmpty(line));
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
    if (stackSize == 0) throw(StackEmpty(line));
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
    if (stackSize == 0) throw(StackEmpty(line));
    else {
        if (top->type == 1) cout << this->top->value << '\n';
        else cout << (int)top->value << '\n';
    }
}

void StackFrame::val(string key, int line) {
    avlNode* tmp = searchAVL(root, key);
    if (!tmp) throw(UndefinedVariable(line));
    else cout << tmp->value << '\n';
}

void StackFrame::par(string key, int line) {
    if (treeSize == 0) throw(UndefinedVariable(line));
    else {
        if (compareKey(key, root->key) == 2) cout << "null\n";
        else {
            avlNode* tmp = searchPar(root, key);
            if (!tmp) throw(UndefinedVariable(line));
            else cout << tmp->key << '\n';
        }
    }
}
