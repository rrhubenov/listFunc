#pragma once
#include<string>
#include<vector>
#include<iostream>

using namespace std;

enum Token {
	tok_eof = -1,
	tok_identifier = -2,
	tok_number = -3,
    tok_ds = -4,
    tok_newline = -5,
    tok_sqr_brckt_l = -6,
    tok_sqr_brckt_r = -7
};


//int gettok();
int getNextToken();

enum ExpressionType {
    type_expr = -1,
    type_num = -2,
    type_param_arg = -3,
    type_func_call = -4,
    type_func_decl = -5,
    type_list = -6
};

void log(const char* str);

class Expression {
public:
    int virtual getType() = 0;
    string virtual print() = 0;

    virtual ~Expression() {}
};

class NumExpr : public Expression {
public:
    double val;
    NumExpr(double val);

    int getType();

    string print();

    ~NumExpr();
};

class ListExpr: public Expression {
public:
    vector<Expression*> elements;

    ListExpr(vector<Expression*> elements);
    int getType();
    string print();

    ~ListExpr();
};

class ParamArgExpr : public Expression {
public:
    NumExpr* ordinal;
    ParamArgExpr(NumExpr* ordinal);

    int getType();

    string print();

    ~ParamArgExpr();
};

class FunctionDeclarationExpr : public Expression {
public:
    string functionIdentifier;
    Expression* body;
    int argCount;
    FunctionDeclarationExpr(
            string functionIdentifier,
            Expression* body,
            int argCount);

    int getType();

    string print();

    ~FunctionDeclarationExpr();
};

class FunctionCallExpr : public Expression {
public:
    string fname;
    vector<Expression*> args;

    FunctionCallExpr(string fname, vector<Expression*> args);

    int getType();

    string print();

    ~FunctionCallExpr();
};

Expression* logError(const char*);
void substituteParam(int, Expression*, Expression*);
Expression* parseExpression();
Expression* copyExpression(Expression*);


