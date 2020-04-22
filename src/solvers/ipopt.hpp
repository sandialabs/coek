#pragma once

#include "coek_model.hpp"
#include "solver.hpp"


namespace coek {

class IpoptSolverRepn;


class IpoptSolver : public NLPSolverRepn
{
protected:

    std::shared_ptr<IpoptSolverRepn> nlp;

    int perform_solve();

public:

    IpoptSolver() : NLPSolverRepn() {}

    void load(NLPModel& model);

    int resolve(bool reset_initial_point);

    int solve(NLPModel& model);
};

}
