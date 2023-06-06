#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Value {
public:
    void virtual print() = 0;
    int virtual getType() = 0;

    virtual ~Value() {}
};

enum ValueTypes {
    val_type_num = -1,
    val_type_list = -2
};

class NumberValue : public Value {
public:
    double val;
    NumberValue(double val);
    void print();
    int getType();
    ~NumberValue();
};

class ListValue : public Value {
public:
    vector<Value*> elements;
    ListValue(vector<Value*> elements);
    void print();
    int getType();
    ~ListValue();
};

