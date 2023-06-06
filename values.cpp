#include "values.hh"


NumberValue::NumberValue(double val) {
    this->val = val;
}

void NumberValue::print() {
    cout << this->val << flush;
}

int NumberValue::getType() {
    return val_type_num;
}

NumberValue::~NumberValue() {

}


ListValue::ListValue(vector<Value*> elements) {
    this->elements = elements;
}

void ListValue::print() {
    cout << "[ " << flush;
    for_each(elements.begin(), elements.end(),
    [](Value* v) {
        v->print();
        cout << " " << flush;
    });

    cout << "]" << flush;
}

int ListValue::getType() {
    return val_type_list;
}

ListValue::~ListValue() {
   // for_each(elements.begin(), elements.end(),
   // [](Value* v) {
   //     delete v;
   // });
}




