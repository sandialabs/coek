#pragma once

#include "model.hpp"
#include "solver.hpp"


namespace coek {

class GurobiSolver : public Solver
{
public:

    ADModel* model;

    GurobiSolver() : Solver() {}

    void set_model(ADModel* _model)
        {model = _model;}

    int solve();
};

}
