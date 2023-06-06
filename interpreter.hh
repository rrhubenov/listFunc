#pragma once

#include "values.hh"
#include "expressions.hh"
#include "exceptions.hh"
#include "utils.hh"
#include <unordered_map>

extern NumberValue* f;
extern NumberValue* t;

extern int curTok;

class Environment {
    unordered_map<string, FunctionDeclarationExpr*> functions;
public:
    FunctionDeclarationExpr* findFun(string name);
    void addFun(string name, FunctionDeclarationExpr* func);
};

struct NativeFunction {
    unsigned arity;
    Value* (*function)(vector<Expression*>);

    NativeFunction(unsigned arity, Value* (*function)(vector<Expression*>));
};

extern unordered_map<string, NativeFunction> nativeFunctions;
extern vector<Value*> garbage;
extern Environment global;

Value* interpret(Expression* expr);


