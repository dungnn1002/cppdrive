#include "example.h"

#include <iostream>
#include <string>

Dummy::Dummy() {}

bool Dummy::sayHelloTo(std::string name) {
    std::cout << "Hello, " << name << "!" << std::endl;
    return true;
}
