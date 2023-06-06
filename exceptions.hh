#pragma once
#include<string>

using namespace std;

class InvalidArgCountException: public exception {
public:
    const char* what() const noexcept;
};

class InvalidArgTypeException: public exception {
public:
    const char* what() const noexcept;
};

