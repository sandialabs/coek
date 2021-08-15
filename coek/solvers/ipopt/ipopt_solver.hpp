#pragma once

#include <memory>
#include "coek/coek_model.hpp"
#include "coek/solvers/solver.hpp"


namespace coek {

class IpoptSolverRepn
{
public:

    virtual int perform_solve() = 0;
    virtual void set_start_from_last_x(bool flag) = 0;
};


class IpoptSolver : public NLPSolverRepn
{
protected:

    std::shared_ptr<IpoptSolverRepn> nlp;

public:

    IpoptSolver() : NLPSolverRepn() {}

    void load(NLPModel& model);

    int resolve();

    int solve(NLPModel& model);
};

}
