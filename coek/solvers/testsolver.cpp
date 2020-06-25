#include <iostream>
#include "coek/coek_model.hpp"
#include "testsolver.hpp"


namespace coek {

int TestSolver::solve(Model& model)
{
assert(initial_solve());
//std::cout << "COLLECTING REPNS/VARS" << std::endl << std::flush;

// Collect repns
coek::MutableNLPExpr orepn;
std::vector<coek::MutableNLPExpr> crepn(model.repn->constraints.size());

orepn.collect_terms(model.repn->objectives[0]);
for (size_t i=0; i<model.repn->constraints.size(); i++)
    crepn[i].collect_terms(model.repn->constraints[i]);

return 0;
}


int TestSolver::solve(CompactModel& _model)
{
Model model = _model.expand();
return solve(model);
}


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

}
