#include "example.h"

#include <iostream>
#include <string>

Dummy::Dummy() {}

void Dummy::sayHelloTo(std::string name) { std::cout << "Hello, " << name << "!" << std::endl; }
