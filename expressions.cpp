#include "expressions.hh"

int curTok;
static string identifierStr;
static double numVal;
extern FILE* input_file;

int gettok() {
    static int lastChar = ' ';

    // Skip whitespaces
	while(lastChar == ' ' || lastChar == '\t') {
		lastChar = getc(input_file);
	}

    if(lastChar == '\n') {
        lastChar = ' ';
        return tok_newline;
    }
    
    // Function name
    if(isalpha(lastChar)) {
        identifierStr = lastChar;
        while(isalnum(lastChar = getc(input_file))) {
            identifierStr += lastChar;
        }
        // TODO: Find function
        return tok_identifier;
    }

    if(lastChar == '#') {
        lastChar = getc(input_file);
        return tok_ds;
    }
    
    // Real number
    if(isdigit(lastChar) || lastChar == '.') {
        string numStr;
        do {
            numStr += lastChar;
            lastChar = getc(input_file);
        } while(isdigit(lastChar) || lastChar == '.');

        numVal = strtod(numStr.c_str(), 0);
        return tok_number;
    }

    if(lastChar == '[') {
        lastChar = getc(input_file);
        return tok_sqr_brckt_l;
    }

    if(lastChar == ']') {
        lastChar = getc(input_file);
        return tok_sqr_brckt_r;
    }

    if(lastChar == EOF)
        return tok_eof;

    int thisChar = lastChar;
    lastChar = getc(input_file);
    return thisChar;

}

int getNextToken() {
    return curTok = gettok();
}


void log(const char* str) {
    cout << str << endl << flush;
}

Expression* parseExpression();

NumExpr::NumExpr(double val) {
    this->val = val;
}

int NumExpr::getType() {
    return type_num;
}

string NumExpr::print() {
    return to_string(val);
}

NumExpr::~NumExpr() {

}

ListExpr::ListExpr(vector<Expression*> elements) {
    this->elements = elements;
}

int ListExpr::getType() {
    return type_list;
}

string ListExpr::print() {
    string result = "[";
    for_each(elements.begin(), elements.end(),
    [&](Expression* p) {
        result += " ";
        result += p->print(); 
    });
    result += " ]";
    return result;
}

ListExpr::~ListExpr() {
    for_each(elements.begin(), elements.end(),
    [](Expression* p) {
        delete p;
    });
}

ParamArgExpr::ParamArgExpr(NumExpr* ordinal) {
    this->ordinal = ordinal;
}

int ParamArgExpr::getType() {
    return type_param_arg;
}

string ParamArgExpr::print() {
    return "#" + to_string(ordinal->val);
}

ParamArgExpr::~ParamArgExpr() {
    delete ordinal;
}

FunctionDeclarationExpr::FunctionDeclarationExpr(
        string functionIdentifier,
        Expression* body,
        int argCount) {
    this->functionIdentifier = functionIdentifier;
    this->body = body;
    this->argCount = argCount;
}

int FunctionDeclarationExpr::getType() {
    return type_func_decl;
}

string FunctionDeclarationExpr::print() {
    string result = functionIdentifier + " -> ";
    return result + body->print();
}


FunctionDeclarationExpr::~FunctionDeclarationExpr() {
    delete body;
}

FunctionCallExpr::FunctionCallExpr(string fname, vector<Expression*> args) {
    this->fname = fname;
    this->args = args;
}

int FunctionCallExpr::getType() {
    return type_func_call;
}

string FunctionCallExpr::print() {
    string result = fname + "(";

    for_each(args.begin(), args.end(),
    [&](Expression* p) {
        result += p->print(); 
        result += ",";
    });
    result.pop_back();
    return result + ")";
}

FunctionCallExpr::~FunctionCallExpr() {
    for_each(args.begin(), args.end(),
    [](Expression* p) {
        delete p;
    });
}


Expression* logError(const char* str) {
    cerr << "Error: " << str << endl;
    return nullptr;
}

int numberOfParamArgs(Expression* expr, int max) {
    if(expr->getType() == type_num) {
        return max;
    } else if(expr->getType() == type_param_arg) {
        ParamArgExpr* parg = (ParamArgExpr*) expr;
        int arg = parg->ordinal->val;
        if(arg + 1 > max) {
            return arg+1;
        } else {
            return max;
        }
    } else if(expr->getType() == type_func_call) {
        FunctionCallExpr* fcall = (FunctionCallExpr*) expr;
        for_each(fcall->args.begin(), fcall->args.end(),
        [&](Expression* arg) {
            max = numberOfParamArgs(arg, max);
        });
        return max;
    } else if(expr->getType() == type_list) {
        ListExpr* l = (ListExpr*) expr;
        for_each(l->elements.begin(), l->elements.end(),
        [&](Expression* elem) {
            max = numberOfParamArgs(elem, max);
        });
        return max;
    } else {
        logError("Unexpected counting of param args");
        return -1;
    }
}


int getOrdinalVal(Expression* expr) {
    return ((ParamArgExpr*) expr)->ordinal->val;
}

// Replace all occurrences of a a param arg with the given expression
void substituteParam(int ordinal, Expression* reduct, Expression* expr) {
    if(reduct->getType() == type_func_call) {
        FunctionCallExpr* func = (FunctionCallExpr*) reduct;        
        for(size_t i = 0; i < func->args.size(); i++) {
            if(func->args[i]->getType() == type_param_arg) {
                if(getOrdinalVal(func->args[i]) == ordinal) {
                    // Free up the param arg
                    delete func->args[i];
                    func->args[i] = expr;
                }
            } else {
                substituteParam(ordinal, func->args[i], expr);
            }
        }
    } else if(reduct->getType() == type_list) {
        ListExpr* l = (ListExpr*) reduct;
        for(size_t i = 0; i < l->elements.size(); i++) {
            if(l->elements[i]->getType() == type_param_arg) {
                if(getOrdinalVal(l->elements[i]) == ordinal) {
                    delete l->elements[i];
                    l->elements[i] = expr; 
                }
            } else {
                substituteParam(ordinal, l->elements[i], expr);
            }
        }
    }
}

NumExpr* parseNumberExpression() {
    NumExpr* result = new NumExpr(numVal);
    getNextToken();
    return result;
}

ParamArgExpr* parseParamArg() {
    getNextToken(); // eat '#'
    auto res = parseNumberExpression();
    return new ParamArgExpr(res);
}

//Parse identifier
// identifier =:= functionName | functionCall | functionDefinition
Expression* parseFunction() {
    string idName = identifierStr;

    getNextToken(); // Eat identifier

    // TODO: Switch '>' to '->'
    // Function declaration
    if(curTok == '>') { 
        getNextToken(); // Eat '>'
        Expression* bodyExpr = parseExpression(); // Parse body expression

        // if(bodyExpr == nullptr) {
        //     return nullptr
        // }

        int argCount = numberOfParamArgs(bodyExpr, 0);
        return new FunctionDeclarationExpr(idName, bodyExpr, argCount);
    } else if(curTok != '(') {
        return logError("Expected a '>' or '(' after function identifier");
    } else {
        getNextToken(); // Eat '('
        vector<Expression*> args;
        if(curTok != ')') {
            while(1) {
                if(Expression* arg = parseExpression()) {
                    args.push_back(arg);
                    for_each(args.begin(), args.end(), 
                    [](Expression* expr){
                        delete expr;
                    });
                } else {
                    return nullptr;
                }

                if(curTok == ')')
                    break;

                if(curTok != ',')
                    return logError("Expected ')' or ',', in arg list");

                getNextToken();
            }
        }

        getNextToken(); // eat ')'

        return new FunctionCallExpr(idName, args);
    }

}

ListExpr* parseList() {
    getNextToken(); // Eat '['
    vector<Expression*> elements;
    while(curTok != tok_sqr_brckt_r) {
        Expression* expr = parseExpression();

        if(expr == nullptr) {
            for_each(elements.begin(), elements.end(),
            [](Expression* expr) {
                    delete expr;
            });
            return nullptr;
        }

        elements.push_back(expr); 
    }
    getNextToken(); // Eat ']'
    return new ListExpr(elements);
}

Expression* parseExpression() {
    switch(curTok) {
    default:
        return logError("unexpected token when parsing expression");
    case tok_identifier:
        return parseFunction();
    case tok_number:
        return parseNumberExpression();
    case tok_ds:
        return parseParamArg();
    case tok_sqr_brckt_l:
        return parseList();
    }
}

Expression* copyExpression(Expression* expr) {
    if(expr->getType() == type_param_arg) {
        int ordinal = getOrdinalVal(expr);
        return new ParamArgExpr(new NumExpr(ordinal));
    } else if(expr->getType() == type_num) {
        return new NumExpr(((NumExpr*) expr)->val);
    } else if(expr->getType() == type_func_call) {
        FunctionCallExpr* func = (FunctionCallExpr*) expr;
        vector<Expression*> copiedExpressions;
        for_each(func->args.begin(), func->args.end(),
        [&](Expression* p) {
            copiedExpressions.push_back(copyExpression(p)); 
        });

        return new FunctionCallExpr(func->fname, copiedExpressions);
    } else if(expr->getType() == type_list) {
        ListExpr* l = (ListExpr*) expr;
        vector<Expression*> copiedExpressions;
        for_each(l->elements.begin(), l->elements.end(),
        [&](Expression* p) {
            copiedExpressions.push_back(copyExpression(p)); 
        });
        return new ListExpr(copiedExpressions); 
    } else {
        // TODO: Cleanup
        logError("Trying to copy something that doesn't need to be copied");
        throw exception();
    }
}

