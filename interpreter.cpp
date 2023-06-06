
#include "interpreter.hh"

NumberValue* f = new NumberValue(0);
NumberValue* t = new NumberValue(1);

unordered_map<string, NativeFunction> nativeFunctions;
vector<Value*> garbage;
Environment global;


NativeFunction::NativeFunction(unsigned arity, Value* (*function)(vector<Expression*>)) {
    this->arity = arity;
    this->function = function;
}

FunctionDeclarationExpr* Environment::findFun(string name) {
    return this->functions.find(name)->second;
}
void Environment::addFun(string name, FunctionDeclarationExpr* func) {
    this->functions.insert(make_pair(name, func));
}

NumberValue* interpretNumExpr(NumExpr* expr) {
    return new NumberValue(expr->val);
}

NumberValue* interpretFuncDecl(FunctionDeclarationExpr* expr) {
    global.addFun(expr->functionIdentifier, expr);
    return new NumberValue(0);
}

Value* callNativeFunction(FunctionCallExpr* expr) {
    NativeFunction f = nativeFunctions.find(expr->fname)->second;
    if(f.arity != expr->args.size()) {
        throw InvalidArgCountException();
    } else {
        Value* v = f.function(expr->args);
        return v;
    }
}

Value* callUserFunction(FunctionCallExpr* expr) {
    FunctionDeclarationExpr* func = global.findFun(expr->fname);
    if(expr->args.size() != func->argCount) {
        throw InvalidArgCountException();
    }

    Expression* bodyCopy = copyExpression(func->body);

    for(size_t i = 0; i < func->argCount; i++) {
        substituteParam(i, bodyCopy, expr->args[i]);
    }

    return interpret(bodyCopy);
}

Value* interpretFuncCall(FunctionCallExpr* expr) {
    if(nativeFunctions.find(expr->fname) != nativeFunctions.end()) {
        return callNativeFunction(expr);
    } else {
        return callUserFunction(expr);
    }
}

ListValue* interpretListExpr(ListExpr* expr) {
    vector<Value*> elements;
    for_each(expr->elements.begin(), expr->elements.end(),
    [&](Expression* elem) {
        elements.push_back(interpret(elem));
    });

    return new ListValue(elements);
}

Value* interpret(Expression* expr) {
    Value* v;
    switch(expr->getType()) {
        case type_num:
            v = interpretNumExpr((NumExpr*) expr);
            break;
        case type_list:
            v = interpretListExpr((ListExpr*) expr);
            break;
        case type_func_decl:
            v = interpretFuncDecl((FunctionDeclarationExpr*) expr);
            break;
        case type_func_call:
            v = interpretFuncCall((FunctionCallExpr*) expr);
            break;
        default:
            v = new NumberValue(-1);
            break;
    }
    garbage.push_back(v);
    return copyValue(v);
}


