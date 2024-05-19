#include <cmath>

#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"
#include "coek/model/nlp_model.hpp"
#include "coek/solvers/solver_repn.hpp"
/*
#include "coek/solvers/ipopt/ipopt_solver.hpp"
#include "testsolver.hpp"
#ifdef WITH_GUROBI
#    include "coek/solvers/gurobi/coek_gurobi.hpp"
#endif
#ifdef WITH_GUROBI
#    include "coek/solvers/gurobi/coek_highs.hpp"
#endif
*/

namespace coek {

void SolverCache::find_updated_values()
{
    vupdates.clear();
    pupdates.clear();

    for (auto& [var, oldval] : vcache) {
        auto value = var->value->eval();
        if (fabs(oldval - value) > tolerance) {
            vupdates.insert(var);
            oldval = value;
        }
    }
    for (auto& [param, oldval] : pcache) {
        auto value = param->eval();
        if (fabs(oldval - value) > tolerance) {
            pupdates.insert(param);
            oldval = value;
        }
    }

#ifdef DEBUG
    std::cout << "Updated Fixed Variables: " << vupdates.size() << " Cache Size: " << vcache.size()
              << std::endl;
    std::cout << "Updated Parameters:      " << pupdates.size() << " Cache Size: " << pcache.size()
              << std::endl;
#endif
}

void SolverCache::reset_cache()
{
    initial = true;
    vcache.clear();
    pcache.clear();
}

std::shared_ptr<SolverResults> NLPSolverRepn::resolve(bool reset_nlpmodel)
{
    if (reset_nlpmodel)
        model->reset();
    return this->resolve_exec();
}

template <typename K, typename V>
V& GetWithDef(std::unordered_map<K, V>& m, const K& key)
{
    typename std::unordered_map<K, V>::iterator it = m.find(key);
    if (it == m.end()) {
        V tmp;
        m[key] = tmp;
        return m[key];
    }
    else {
        return it->second;
    }
}

void SolverRepn::load(Model& _model)
{
    model = _model;
    reset();

    vconstvals.clear();
    pconstvals.clear();
    vlinvals.clear();
    plinvals.clear();
    vquadvals.clear();
    pquadvals.clear();
    vnonlvals.clear();
    pnonlvals.clear();

    repn.resize(model.repn->objectives.size() + model.repn->constraints.size());

    // Collect mutable repn data
    {
        size_t j = 0;
        for (size_t i = 0; i < model.repn->objectives.size(); i++, j++)
            repn[j].collect_terms(model.repn->objectives[i]);
        for (size_t i = 0; i < model.repn->constraints.size(); i++, j++)
            repn[j].collect_terms(model.repn->constraints[i]);
    }

    // Populate the maps that identify mutable model values
    int nmutable = 0;
    for (size_t j = 0; j < repn.size(); j++) {
        MutableNLPExpr& _repn = repn[j];
        if (!_repn.is_mutable())
            continue;

        nmutable++;
        std::unordered_set<std::shared_ptr<VariableTerm>> fixed_vars;
        std::unordered_set<std::shared_ptr<ParameterTerm>> params;
        std::unordered_set<std::shared_ptr<SubExpressionTerm>> visited_subexpressions;

        mutable_values(_repn.constval, fixed_vars, params, visited_subexpressions);
        for (auto& it : fixed_vars)
            GetWithDef(vconstvals, it.get()).insert(j);
        for (auto& it : params)
            GetWithDef(pconstvals, it.get()).insert(j);

        for (size_t i = 0; i < _repn.linear_coefs.size(); i++) {
            fixed_vars.clear();
            params.clear();
            mutable_values(_repn.linear_coefs[i], fixed_vars, params, visited_subexpressions);
            for (auto& it : fixed_vars)
                GetWithDef(vlinvals, it.get()).insert(std::pair<size_t, size_t>(j, i));
            for (auto& it : params)
                GetWithDef(plinvals, it.get()).insert(std::pair<size_t, size_t>(j, i));
        }

        for (size_t i = 0; i < _repn.quadratic_coefs.size(); i++) {
            fixed_vars.clear();
            params.clear();
            mutable_values(_repn.quadratic_coefs[i], fixed_vars, params, visited_subexpressions);
            for (auto& it : fixed_vars)
                GetWithDef(vquadvals, it.get()).insert(std::pair<size_t, size_t>(j, i));
            for (auto& it : params)
                GetWithDef(pquadvals, it.get()).insert(std::pair<size_t, size_t>(j, i));
        }

        fixed_vars.clear();
        params.clear();
        mutable_values(_repn.nonlinear, fixed_vars, params, visited_subexpressions);
        for (auto& it : fixed_vars)
            GetWithDef(vnonlvals, it.get()).insert(j);
        for (auto& it : params)
            GetWithDef(pnonlvals, it.get()).insert(j);
    }

#ifdef DEBUG
    std::cout << "# Model Expressions:   " << repn.size() << std::endl;
    std::cout << "# Mutable Expressions: " << nmutable << std::endl;
#endif
}

void SolverRepn::reset() { reset_cache(); }

#ifdef COEK_WITH_COMPACT_MODEL
void SolverRepn::load(CompactModel& _model)
{
    model = _model.expand();
    load(model);
}

std::shared_ptr<SolverResults> SolverRepn::solve(CompactModel& _model)
{
    model = _model.expand();
    return solve(model);
}
#endif

bool SolverRepn::initial_solve()
{
    if (initial) {
        for (auto& [vconst, _] : vconstvals)
            vcache[vconst] = vconst->eval();
        for (auto& [pconst, _] : pconstvals)
            pcache[pconst] = pconst->eval();

        for (auto& [vlin, _] : vlinvals)
            vcache[vlin] = vlin->eval();
        for (auto& [plin, _] : plinvals)
            pcache[plin] = plin->eval();

        for (auto& [vquad, _] : vquadvals)
            vcache[vquad] = vquad->eval();
        for (auto& [pquad, _] : pquadvals)
            pcache[pquad] = pquad->eval();

        for (auto& [vnonl, _] : vnonlvals)
            vcache[vnonl] = vnonl->eval();
        for (auto& [pnonl, _] : pnonlvals)
            pcache[pnonl] = pnonl->eval();

        vupdates.clear();
        pupdates.clear();

        initial = false;
        return true;
    }
    else {
        find_updated_values();
        find_updated_coefs();
        return false;
    }
}

void SolverRepn::find_updated_coefs()
{
    updated_coefs.clear();

    for (auto it = vupdates.begin(); it != vupdates.end(); ++it) {
        try {
            // std::set<size_t>& expr = vconstvals[*it];
            for (size_t j : vconstvals[*it])
                updated_coefs.insert({j, 0, 0});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<std::tuple<size_t, size_t>>& expr = vlinvals[*it];
            for (auto& [i, j] : vlinvals[*it])
                updated_coefs.insert({i, 1, j});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<std::tuple<size_t, size_t>>& expr = vquadvals[*it];
            for (auto& [i, j] : vquadvals[*it])
                updated_coefs.insert({i, 2, j});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<size_t>& expr = vnonlvals[*it];
            for (size_t j : vnonlvals[*it])
                updated_coefs.insert({j, 3, 0});
        }
        catch (...) {
            // TODO
        }
    }

    for (auto it = pupdates.begin(); it != pupdates.end(); ++it) {
        try {
            // std::set<size_t>& expr = pconstvals[*it];
            for (size_t j : pconstvals[*it])
                updated_coefs.insert({j, 0, 0});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<std::tuple<size_t, size_t>>& expr = plinvals[*it];
            for (auto& [i, j] : plinvals[*it])
                updated_coefs.insert({i, 1, j});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<std::tuple<size_t, size_t>>& expr = pquadvals[*it];
            for (auto& [i, j] : pquadvals[*it])
                updated_coefs.insert({i, 2, j});
        }
        catch (...) {
            // TODO
        }
        try {
            // std::set<size_t>& expr = pnonlvals[*it];
            for (size_t j : pnonlvals[*it])
                updated_coefs.insert({j, 3, 0});
        }
        catch (...) {
            // TODO
        }
    }

#ifdef DEBUG
    std::cout << "Updated Coefficients:    " << updated_coefs.size() << std::endl;
#endif
}

}  // namespace coek
