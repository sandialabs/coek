#include <coek/coek.hpp>
#include <map>
#include <vector>

#ifdef COEK_WITH_COMPACT_MODEL
void nqueens_compact(coek::CompactModel& model, size_t N)
{
    auto Rows = coek::RangeSet(0, N - 1);
    auto Cols = coek::RangeSet(0, N - 1);
    auto i = coek::set_element("i");
    auto j = coek::set_element("j");

    auto x = model.add(coek::variable("x", Rows * Cols))
                 .bounds(0, 1)
                 .value(0)
                 .within(coek::VariableTypes::Binary);

    model.add(
        coek::objective("obj", coek::Sum(x(i, j), coek::Forall(i).In(Rows).Forall(j).In(Cols))));

    model.add(coek::constraint("one_per_row", coek::Sum(x(i, j), coek::Forall(j).In(Cols)) == 1,
                               coek::Forall(i).In(Rows)));

    model.add(coek::constraint("one_per_column", coek::Sum(x(i, j), coek::Forall(i).In(Rows)) == 1,
                               coek::Forall(j).In(Cols)));

    model.add(coek::constraint(
        "\\diagonals_col",
        x(0, i) + coek::Sum(x(j, i + j), coek::Forall(j).In(coek::RangeSet(1, N - i - 1))) <= 1,
        coek::Forall(i).In(coek::RangeSet(0, N - 2))));

    model.add(coek::constraint(
        "\\diagonals_row",
        x(i, 0) + coek::Sum(x(i + j, j), coek::Forall(j).In(coek::RangeSet(1, N - i - 1))) <= 1,
        coek::Forall(i).In(coek::RangeSet(1, N - 2))));

    model.add(coek::constraint(
        "/diagonals_col",
        x(0, i) + coek::Sum(x(j, i - j), coek::Forall(j).In(coek::RangeSet(1, i))) <= 1,
        coek::Forall(i).In(coek::RangeSet(1, N - 1))));

    model.add(coek::constraint(
        "/diagonals_row",
        x(i, N - 1)
                + coek::Sum(x(i + j, N - 1 - j), coek::Forall(j).In(coek::RangeSet(1, N - i - 1)))
            <= 1,
        coek::Forall(i).In(coek::RangeSet(1, N - 2))));
}
#endif
