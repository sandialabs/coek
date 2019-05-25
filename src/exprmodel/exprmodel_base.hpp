#pragma once

#include <iostream>

namespace coek {

namespace base {

//
// The base class of models defined with expressions
//
template <class ExprManager>
class ExprModel
{
public:

  typedef typename ExprManager::expr_t expr_t;

  ExprManager manager;
  
  virtual ~ExprModel() {}

  virtual void add_objective(expr_t expr) = 0;

  virtual void add_inequality(expr_t expr) = 0;

  virtual void add_equality(expr_t expr) = 0;

  virtual expr_t get_objective(unsigned int n) = 0;

  virtual expr_t get_constraint(unsigned int n) = 0;

  virtual void print(std::ostream& ostr) = 0;

};

}

}
