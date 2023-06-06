#include "exceptions.hh"

using namespace std;

const char* InvalidArgCountException::what() const noexcept {
    return "Invalid arg count";
}

const char* InvalidArgTypeException::what() const noexcept {
    return "Invalid arg type";
}


