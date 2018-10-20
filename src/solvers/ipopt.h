#pragma once

class IpoptSolver : public Solver
{
public:

    Model* model;

    IpoptSolver() : Solver() {}

    void set_model(Model* _model)
        {model = _model;}

    int solve();
};

