#include <iostream>
#include "StackFrame.h"
#include "errors.h"
using namespace std;

/*
Run the testcase written in `filename`
@param filename name of the file
*/
void test(string filename) {
    StackFrame *sf = new StackFrame();
    try {
        sf->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete sf;
}

/*
Main function
*/
int main(int argc, char **argv) {
    //test("test_example.txt");
    test("test000.txt");
    cout << "-----\n";
    test("test001.txt");
    cout << "\n-----\n";
    test("test002.txt");
    cout << "\n-----\n";
    test("test003.txt");
    cout << "\n-----\n";
    test("test004.txt");
    return 0;
}
