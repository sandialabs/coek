#pragma once

#include "model.hpp"

namespace coek {

//
// The base solver class that defines the API used by Python
//
class Solver 
{
public:

    Solver(void) {}

    virtual void set_model(ADModel* model) = 0;

    virtual int solve() = 0;

};


Solver* create_solver(const char* name);

}
