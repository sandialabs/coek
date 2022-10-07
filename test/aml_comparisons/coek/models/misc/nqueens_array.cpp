#include <coek/coek.hpp>
#include <map>
#include <vector>

void nqueens_array(coek::Model& model, size_t N)
{
    auto x = coek::variable("x", {N, N}).bounds(0, 1).value(0).within(coek::VariableTypes::Binary);
    model.add(x);

    // obj
    auto obj = coek::expression();
    for (size_t i : coek::sequence<size_t>(0, N - 1))
        for (size_t j : coek::sequence<size_t>(0, N - 1)) obj += x(i, j);
    model.add_objective(obj);

    // one per row
    for (size_t i : coek::sequence<size_t>(0, N - 1)) {
        auto c = coek::expression();
        for (size_t j : coek::sequence<size_t>(0, N - 1)) c += x(i, j);
        model.add(c == 1);
    }

    // one per column
    for (size_t j : coek::sequence<size_t>(0, N - 1)) {
        auto c = coek::expression();
        for (size_t i : coek::sequence<size_t>(0, N - 1)) c += x(i, j);
        model.add(c == 1);
    }

    // \diagonals_col
    for (size_t i : coek::sequence<size_t>(0, N - 2)) {
        auto c = coek::expression();
        c += x(0, i);
        for (size_t j : coek::sequence<size_t>(1, N - i - 1)) c += x(j, i + j);
        model.add(c <= 1);
    }
    // \diagonals_row
    for (size_t i : coek::sequence<size_t>(1, N - 2)) {
        auto c = coek::expression();
        c += x(i, 0);
        for (size_t j : coek::sequence<size_t>(1, N - i - 1)) c += x(i + j, j);
        model.add(c <= 1);
    }

    // /diagonals_col
    for (size_t i : coek::sequence<size_t>(1, N - 1)) {
        auto c = coek::expression();
        c += x(0, i);
        for (size_t j : coek::sequence<size_t>(1, i)) c += x(j, i - j);
        model.add(c <= 1);
    }
    // /diagonals_row
    for (size_t i : coek::sequence<size_t>(1, N - 2)) {
        auto c = coek::expression();
        c += x(i, N - 1);
        for (size_t j : coek::sequence<size_t>(1, N - i - 1)) c += x(i + j, N - 1 - j);
        model.add(c <= 1);
    }
}
