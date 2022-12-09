#pragma once

#ifdef __has_include
#    if __has_include(<version>)
#        include <version>
#    endif
#    if __has_include(<variant>)
#        include <variant>
#    endif
#endif

#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/api/intrinsic_fn.hpp"
#include "coek/api/objective.hpp"
#if __cpp_lib_variant >= 201606
#    include "coek/api/constraint_map.hpp"
#    include "coek/api/parameter_array.hpp"
#    include "coek/api/variable_array.hpp"
#endif

#include "coek/model/model.hpp"
#include "coek/model/nlp_model.hpp"

#ifdef COEK_WITH_COMPACT_MODEL
#    include "coek/compact/coek_sets.hpp"
#    include "coek/compact/constraint_sequence.hpp"
#    include "coek/compact/expression_sequence.hpp"
#    include "coek/compact/objective_sequence.hpp"
#    include "coek/compact/parameter_map.hpp"
#    include "coek/compact/sequence_context.hpp"
#    include "coek/compact/variable_map.hpp"
#    include "coek/model/compact_model.hpp"
#endif

#include "coek/solvers/solver.hpp"
#include "coek/util/sequence.hpp"
