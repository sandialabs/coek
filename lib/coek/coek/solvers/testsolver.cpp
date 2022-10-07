#include "testsolver.hpp"

#include <cassert>
#include <iostream>

#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/compact_model.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

int TestSolver::solve(Model& model)
{
    assert(initial_solve());
    // std::cout << "COLLECTING REPNS/VARS" << std::endl << std::flush;

    // Collect repns
    coek::MutableNLPExpr orepn;
    std::vector<coek::MutableNLPExpr> crepn(model.repn->constraints.size());

    orepn.collect_terms(model.repn->objectives[0]);
    for (size_t i = 0; i < model.repn->constraints.size(); i++)
        crepn[i].collect_terms(model.repn->constraints[i]);

    return 0;
}

#ifdef COEK_WITH_COMPACT_MODEL
int TestSolver::solve(CompactModel& _model)
{
    Model model = _model.expand();
    return solve(model);
}
#endif

int TestSolver::resolve()
{
    if (initial_solve()) {
        // Setup the model the first time here
    }
    else {
        // Update the model here
    }

    // Perform optimization here

    // NOTE: nothing is done to 'solve' a problem in this test
    // solver.

    return 0;
}

}  // namespace coek
