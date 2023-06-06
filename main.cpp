#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "interpreter.hh"
#include "std.hh"
#include "expressions.hh"
#include "err.h"

FILE* input_file;

int main(int argc, char** argv) {
    nativeFunctions.insert(make_pair("eq", NativeFunction(2, std_eq)));
    nativeFunctions.insert(make_pair("le", NativeFunction(2, std_le)));
    nativeFunctions.insert(make_pair("nand", NativeFunction(2, std_nand)));
    nativeFunctions.insert(make_pair("length", NativeFunction(1, std_length)));
    nativeFunctions.insert(make_pair("head", NativeFunction(1, std_head)));
    nativeFunctions.insert(make_pair("tail", NativeFunction(1, std_tail)));
    nativeFunctions.insert(make_pair("list", NativeFunction(3, std_list)));
    nativeFunctions.insert(make_pair("concat", NativeFunction(2, std_concat)));
    nativeFunctions.insert(make_pair("if", NativeFunction(3, std_if)));
    nativeFunctions.insert(make_pair("read", NativeFunction(0, std_read)));
    nativeFunctions.insert(make_pair("write", NativeFunction(1, std_write)));
    nativeFunctions.insert(make_pair("int", NativeFunction(1, std_int)));

    // Arithemtic operations
    nativeFunctions.insert(make_pair("add", NativeFunction(2, std_add)));
    nativeFunctions.insert(make_pair("sub", NativeFunction(2, std_sub)));
    nativeFunctions.insert(make_pair("mul", NativeFunction(2, std_mul)));
    nativeFunctions.insert(make_pair("div", NativeFunction(2, std_div)));
    nativeFunctions.insert(make_pair("mod", NativeFunction(2, std_mod)));
    nativeFunctions.insert(make_pair("sqrt", NativeFunction(1, std_sqrt)));

    input_file = stdin;

    if(argc > 1) {
        if(argc != 2) 
            errx(1, "0 or 1 arguments expected.");

        char* file_name = argv[1];
        input_file = fopen(file_name, "r");
    }

    getNextToken();
    Expression* expr;
    Value* val;
    while(1) {
        switch(curTok) {
        case tok_eof:
            return 0;
        case tok_newline:
            getNextToken();
            break;
        default:
            expr = parseExpression();
            if(expr == nullptr) {
               continue; 
            }
            val = interpret(expr);
            if(input_file == stdin) {
                cout << "> ";
                val->print();
                cout << endl;
            }
            delete val;
            if(expr->getType() != type_func_decl) {
                delete expr;
            }
            for_each(garbage.begin(), garbage.end(),
            [](Value* v) {
                delete v;
            });
            garbage.clear();
            break;
        }
    }

    delete f;
    delete t;
    
	return 0;
}

