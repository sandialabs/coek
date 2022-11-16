#pragma once

#ifdef __has_include
#    if __has_include(<version>)
#        include <version>
#    endif
#    if __has_include(<variant>)
#        include <variant>
#    endif
#endif
#include <stdexcept>

namespace coek {

class BaseExpressionTerm;
class VariableTerm;
#if __cpp_lib_variant
class ParameterRefTerm;
class VariableRefTerm;
#endif
class ConstantTerm;
class ParameterTerm;
class IndexParameterTerm;
class MonomialTerm;
class InequalityTerm;
class EqualityTerm;
class EmptyConstraintTerm;
class NegateTerm;
class PlusTerm;
class TimesTerm;
class DivideTerm;
class AbsTerm;
class CeilTerm;
class FloorTerm;
class ExpTerm;
class LogTerm;
class Log10Term;
class SqrtTerm;
class SinTerm;
class CosTerm;
class TanTerm;
class SinhTerm;
class CoshTerm;
class TanhTerm;
class ASinTerm;
class ACosTerm;
class ATanTerm;
class ASinhTerm;
class ACoshTerm;
class ATanhTerm;
class PowTerm;
class SumExpressionTerm;
class ObjectiveTerm;

class Visitor {
   public:
    virtual ~Visitor() {}

    virtual void visit(ConstantTerm& arg) = 0;
    virtual void visit(ParameterTerm& arg) = 0;
    virtual void visit(IndexParameterTerm& arg) = 0;
    virtual void visit(VariableTerm& arg) = 0;
#if __cpp_lib_variant
    virtual void visit(ParameterRefTerm& arg) = 0;
    virtual void visit(VariableRefTerm& arg) = 0;
#endif
    virtual void visit(MonomialTerm& arg) = 0;
    virtual void visit(InequalityTerm& arg) = 0;
    virtual void visit(EqualityTerm& arg) = 0;
    virtual void visit(EmptyConstraintTerm&) {}
    virtual void visit(NegateTerm& arg) = 0;
    virtual void visit(PlusTerm& arg) = 0;
    virtual void visit(TimesTerm& arg) = 0;
    virtual void visit(DivideTerm& arg) = 0;
    virtual void visit(AbsTerm& arg) = 0;
    virtual void visit(CeilTerm& arg) = 0;
    virtual void visit(FloorTerm& arg) = 0;
    virtual void visit(ExpTerm& arg) = 0;
    virtual void visit(LogTerm& arg) = 0;
    virtual void visit(Log10Term& arg) = 0;
    virtual void visit(SqrtTerm& arg) = 0;
    virtual void visit(SinTerm& arg) = 0;
    virtual void visit(CosTerm& arg) = 0;
    virtual void visit(TanTerm& arg) = 0;
    virtual void visit(SinhTerm& arg) = 0;
    virtual void visit(CoshTerm& arg) = 0;
    virtual void visit(TanhTerm& arg) = 0;
    virtual void visit(ASinTerm& arg) = 0;
    virtual void visit(ACosTerm& arg) = 0;
    virtual void visit(ATanTerm& arg) = 0;
    virtual void visit(ASinhTerm& arg) = 0;
    virtual void visit(ACoshTerm& arg) = 0;
    virtual void visit(ATanhTerm& arg) = 0;
    virtual void visit(PowTerm& arg) = 0;
    virtual void visit(ObjectiveTerm& arg) = 0;
    virtual void visit(SumExpressionTerm&)
    {
        throw std::runtime_error("Visitor cannot handle SumExpressionTerm");
    }
};

enum term_id : unsigned int {
    ConstantTerm_id = 1,
    ParameterTerm_id = 2,
    ParameterRefTerm_id = 106,
    IndexParameterTerm_id = 100,
    VariableTerm_id = 3,
    VariableRefTerm_id = 4,
    // IndexedVariableTerm_id = 101,
    MonomialTerm_id = 5,
    InequalityTerm_id = 6,
    EqualityTerm_id = 7,
    EmptyConstraintTerm_id = 104,
    // EmptyObjectiveTerm_id = 105,
    NegateTerm_id = 8,
    PlusTerm_id = 9,
    TimesTerm_id = 10,
    DivideTerm_id = 11,
    AbsTerm_id = 12,
    CeilTerm_id = 13,
    FloorTerm_id = 14,
    ExpTerm_id = 15,
    LogTerm_id = 16,
    Log10Term_id = 17,
    SqrtTerm_id = 18,
    SinTerm_id = 19,
    CosTerm_id = 20,
    TanTerm_id = 21,
    SinhTerm_id = 22,
    CoshTerm_id = 23,
    TanhTerm_id = 24,
    ASinTerm_id = 25,
    ACosTerm_id = 26,
    ATanTerm_id = 27,
    ASinhTerm_id = 28,
    ACoshTerm_id = 29,
    ATanhTerm_id = 30,
    PowTerm_id = 31,
    SumExpressionTerm_id = 102,
    ObjectiveTerm_id = 103
};

}  // namespace coek
