#include "utils.hh"


Value* copyValue(Value* v) {
    if(v->getType() == val_type_num) {
        NumberValue* n = (NumberValue*) v;
        return new NumberValue(n->val);
    } else {
        ListValue* l = (ListValue*) v;
        vector<Value*> copies;
        for_each(l->elements.begin(), l->elements.end(),
        [&](Value* v) {
            copies.push_back(copyValue(v));
        });
        return new ListValue(copies);
    }
}

