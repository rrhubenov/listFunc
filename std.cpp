#include "values.hh"
#include "expressions.hh"
#include "std.hh"
#include <iostream>

NumberValue* eq(Value* v1, Value* v2) {
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        if(n1->val == n2->val) {
            return new NumberValue(1);
        } else {
            return new NumberValue(0);
        }
    } else if(v1->getType() == val_type_list && v2->getType() == val_type_list) {
        ListValue* l1 = (ListValue*) v1;
        ListValue* l2 = (ListValue*) v2;
        if(l1->elements.size() != l2->elements.size()) {
            return new NumberValue(0);
        } else {
            for(size_t i = 0; i < l1->elements.size(); i++) {
                if(eq(l1->elements[i], l2->elements[i])->val == 0) {
                    return new NumberValue(0);
                }
            }
            return new NumberValue(1);
        }
    } else if(v1->getType() == val_type_list && v2->getType() == val_type_num) {
        ListValue* l = (ListValue*) v1;
        NumberValue* n = (NumberValue*) v2;

        if(l->elements.size() == 1) {
            return eq(l->elements[0], n);
        } else {
            return new NumberValue(0);
        }
    } else if(v1->getType() == val_type_num && v2->getType() == val_type_list) {
        NumberValue* n = (NumberValue*) v1;
        ListValue* l = (ListValue*) v2;

        if(l->elements.size() == 1) {
            return eq(l->elements[0], n);
        } else {
            return new NumberValue(0);
        }
    } else {
        logError("Cannot compare the given types");
        return new NumberValue(-1);
    }
}

Value* std_eq(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    return eq(v1, v2);
}

Value* head(ListValue* l) {
    return copyValue(l->elements[0]);
}
Value* std_head(vector<Expression*> args) {
    Value* v =interpret(args[0]);
    if(v->getType() == val_type_list) {
        return head((ListValue*) v);
    } else {
        throw InvalidArgTypeException();
    }
}

ListValue* tail(ListValue* l) {
    vector<Value*> elems;
    for(int i = 1; i < l->elements.size(); i++) {
        elems.push_back(l->elements[i]); 
    }
    return new ListValue(elems);
}

Value* std_tail(vector<Expression*> args) {
    Value* v = interpret(args[0]);
    if(v->getType() == val_type_list) {
        return tail((ListValue*) v);
    } else {
        throw InvalidArgTypeException();
    }
}

NumberValue* le(Value* v1, Value* v2) {
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        if(n1->val < n2->val) {
            return new NumberValue(1);
        } else {
            return new NumberValue(0);
        }
    } else if(v1->getType() == val_type_list && v2->getType() == val_type_list) {
        ListValue* l1 = (ListValue*) v1;
        ListValue* l2 = (ListValue*) v2;
        if(l1->elements.size() != l2->elements.size()) {
            return new NumberValue(0);
        } else {
            for(size_t i = 0; i < l1->elements.size(); i++) {
                if(le(l1->elements[i], l2->elements[i])->val == 0) {
                    return new NumberValue(0);
                }
            }
            return new NumberValue(1);
        }
    } else if(v1->getType() == val_type_list && v2->getType() == val_type_num) {
        ListValue* l = (ListValue*) v1;
        NumberValue* n = (NumberValue*) v2;

        if(l->elements.size() == 1) {
            return le(l->elements[0], n);
        } else {
            return new NumberValue(0);
        }
    } else if(v1->getType() == val_type_num && v2->getType() == val_type_list) {
        NumberValue* n = (NumberValue*) v1;
        ListValue* l = (ListValue*) v2;

        if(l->elements.size() == 1) {
            return le(l->elements[0], n);
        } else {
            return new NumberValue(0);
        }
    } else {
        logError("Cannot compare the given types");
        return new NumberValue(-1);
    }
}

Value* std_le(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    return le(v1, v2);
}

Value* std_length(vector<Expression*> args) {
    Value* v = interpret(args[0]);
    if(v->getType() == val_type_list) {
        return new NumberValue(((ListValue*) v)->elements.size());
    } else {
        return new NumberValue(-1); 
    }
}

Value* std_concat(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_list && v2->getType() == val_type_list) {
        ListValue* l1 = (ListValue*) v1;
        ListValue* l2 = (ListValue*) v2;
        vector<Value*> resultVector;
        for_each(l1->elements.begin(), l1->elements.end(),
        [&](Value* v) {
            resultVector.push_back(v);
        });
        for_each(l2->elements.begin(), l2->elements.end(),
        [&](Value* v) {
            resultVector.push_back(v);
        });
        return new ListValue(resultVector);
    } else {
        throw new InvalidArgTypeException();
    }
}


Value* std_if(vector<Expression*> args) {
    Value* condition = interpret(args[0]);
    Expression* iftrue = args[1];
    Expression* iffalse = args[2];
    if(eq(condition, f)->val != 1) {
       return interpret(iftrue);
    } else {
        return interpret(iffalse);
    }
}

Value* std_nand(vector<Expression*> args) {
    Expression* v1 = args[0];
    Expression* v2 = args[1];

    if(eq(interpret(v1), f)->val == 1 
            || eq(interpret(v2), f)->val == 1) {
        return new NumberValue(1);
    } else {
        return new NumberValue(0);
    }
}

Value* std_write(vector<Expression*> args) {
    Value* v = interpret(args[0]);
    v->print();
    std::cout << std::endl;
    return new NumberValue(0);
}

Value* std_mod(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        return new NumberValue((int)n1->val % (int)n2->val);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_add(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        return new NumberValue(n1->val + n2->val);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_sub(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        return new NumberValue(n1->val - n2->val);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_mul(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        return new NumberValue(n1->val * n2->val);
    } else {
        throw InvalidArgTypeException();
    }
}


Value* std_div(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    if(v1->getType() == val_type_num && v2->getType() == val_type_num) {
        NumberValue* n1 = (NumberValue*) v1;
        NumberValue* n2 = (NumberValue*) v2;
        return new NumberValue(n1->val / n2->val);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_sqrt(vector<Expression*> args) {
    Value* v = interpret(args[0]);
    if(v->getType() == val_type_num) {
        NumberValue* n = (NumberValue*) v;
        return new NumberValue(sqrt(n->val));
    } else {
        throw InvalidArgTypeException();
    }
}

bool isNum(Value* v) {
    return v->getType() == val_type_num;
}

bool isList(Value* v) {
    return v->getType() == val_type_list;
}

NumberValue* toNum(Value* v) {
    return (NumberValue*) v;
}

ListValue* toList(Value* v) {
    return (ListValue*) v;
}

Value* std_list(vector<Expression*> args) {
    Value* v1 = interpret(args[0]);
    Value* v2 = interpret(args[1]);
    Value* v3 = interpret(args[2]);

    if(isNum(v1) && isNum(v2) && isNum(v3)) {
        NumberValue* n1 = toNum(v1);
        NumberValue* n2 = toNum(v2);
        NumberValue* n3 = toNum(v3);
        vector<Value*> result;
        int curr = n1->val;
        while(curr <= n3->val) {
            result.push_back(new NumberValue(curr));
            curr = curr + n2->val;
        }
        return new ListValue(result);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_int(vector<Expression*> args) {
    Value* v = interpret(args[0]);
    if(isNum(v)) {
        NumberValue* n = (NumberValue*)v;
        return new NumberValue((int) n->val);
    } else {
        throw InvalidArgTypeException();
    }
}

Value* std_read(vector<Expression*> args) {
    double input;
    std::cin >> input;
    return new NumberValue(input);
}



