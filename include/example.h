#pragma once

#include <string>

/**
 * This is a dummy class to demonstrate features of the boiler plate.
 */
class Dummy {
   public:
    /**
     * Default constructor for Dummy (does nothing).
     */
    Dummy();
    /**
     * Here is how to comment a function.
     * @param name you can use @ param to demonstrate what a function takes as a parameter.
     * @return You can use @ return to demonstrate what a function returns. Here, we return nothing
     *  because `void`.
     */
    void sayHelloTo(std::string name);
};
