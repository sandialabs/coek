
#include "model.h"

//
// The base solver class that defines the API used by Python
//
class Solver 
{
public:

    Solver(void) {}

    virtual void set_model(Model* model) = 0;

    virtual int solve() = 0;

};


class IpoptSolver : public Solver
{
public:

    Model* model;

    IpoptSolver() : Solver() {}

    void set_model(Model* _model)
        {model = _model;}

    int solve();
};


Solver* create_solver(const char* name);

