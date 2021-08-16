#pragma once

#include <memory>
#include <string>
#include <map>
#include "coek/coek_model.hpp"
#include "coek/solvers/solver.hpp"


namespace coek {

class IpoptSolverRepn
{
public:

    virtual int perform_solve() = 0;
    virtual void set_start_from_last_x(bool flag) = 0;
    virtual void set_options(
                    std::map<std::string,std::string>& string_options,
                    std::map<std::string,int>& integer_options,
                    std::map<std::string,double>& double_options
                    ) = 0;
};


class IpoptSolver : public NLPSolverRepn
{
protected:

    bool available_;
    std::shared_ptr<IpoptSolverRepn> repn;

    void initialize();

public:

    IpoptSolver() 
        : available_(true), NLPSolverRepn()
        { initialize(); }

    void load(NLPModel& model);

    int resolve();

    int solve(NLPModel& model);

    bool available()
        {return available_;}
};

}
