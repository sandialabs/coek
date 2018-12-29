#pragma once

#include "model.hpp"
#include "solver.hpp"


class IpoptSolver : public Solver
{
public:

    ADModel* model;

    IpoptSolver() : Solver() {}

    void set_model(ADModel* _model)
        {model = _model;}

    int solve();
};

