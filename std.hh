#pragma once
#include <vector>
#include "values.hh"
#include "expressions.hh"
#include "exceptions.hh"
#include "interpreter.hh"

Value* std_eq(vector<Expression*> args);
Value* std_head(vector<Expression*> args);
Value* std_tail(vector<Expression*> args);
Value* std_le(vector<Expression*> args);
Value* std_length(vector<Expression*> args);
Value* std_concat(vector<Expression*> args);
Value* std_if(vector<Expression*> args);
Value* std_nand(vector<Expression*> args);
Value* std_write(vector<Expression*> args);
Value* std_mod(vector<Expression*> args);
Value* std_add(vector<Expression*> args);
Value* std_sub(vector<Expression*> args);
Value* std_mul(vector<Expression*> args);
Value* std_div(vector<Expression*> args);
Value* std_sqrt(vector<Expression*> args);
Value* std_list(vector<Expression*> args);
Value* std_int(vector<Expression*> args);
Value* std_read(vector<Expression*> args);


bool isNum(Value* v);
bool isList(Value* v);
NumberValue* toNum(Value* v);
ListValue* toList(Value* v);

