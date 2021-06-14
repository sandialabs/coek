#ifndef __coek_hpp
#define __coek_hpp

#pragma once

#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/api/intrinsic_fn.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"

#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/coek_indexed.hpp"
#include "coek/compact/sequence_context.hpp"
#include "coek/compact/expression_sequence.hpp"
#include "coek/compact/objective_sequence.hpp"
#include "coek/compact/constraint_sequence.hpp"
#endif

#include "coek/coek_model.hpp"

#endif
