#pragma once


namespace coek {

class BaseExpressionTerm;
class VariableTerm;
class ConstantTerm;
class ParameterTerm;
class VariableTerm;
class MonomialTerm;
class InequalityTerm;
class EqualityTerm;
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


class Visitor
{
public:

    virtual void visit(ConstantTerm& arg) = 0;
    virtual void visit(ParameterTerm& arg) = 0;
    virtual void visit(VariableTerm& arg) = 0;
    virtual void visit(MonomialTerm& arg) = 0;
    virtual void visit(InequalityTerm& arg) = 0;
    virtual void visit(EqualityTerm& arg) = 0;
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
};

enum term_id {
    ConstantTerm_id =   1,
    ParameterTerm_id =  2,
    VariableTerm_id =   3,
    MonomialTerm_id =   4,
    InequalityTerm_id = 5,
    EqualityTerm_id =   6,
    NegateTerm_id =     7,
    PlusTerm_id =       8,
    TimesTerm_id =      9,
    DivideTerm_id =     10,
    AbsTerm_id =        11,
    CeilTerm_id =       12,
    FloorTerm_id =      13,
    ExpTerm_id =        14,
    LogTerm_id =        15,
    Log10Term_id =      16,
    SqrtTerm_id =       17,
    SinTerm_id =        18,
    CosTerm_id =        19,
    TanTerm_id =        20,
    SinhTerm_id =       21,
    CoshTerm_id =       22,
    TanhTerm_id =       23,
    ASinTerm_id =       24,
    ACosTerm_id =       25,
    ATanTerm_id =       26,
    ASinhTerm_id =      27,
    ACoshTerm_id =      28,
    ATanhTerm_id =      29,
    PowTerm_id =        30
    };

}
