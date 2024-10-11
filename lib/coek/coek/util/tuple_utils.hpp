#pragma once

namespace coek {

template <typename TupleT, typename VecT, std::size_t... Ts>
inline void copy_tuple_to_vector_Impl(const TupleT& tp, std::vector<VecT>& vec,
                                      std::index_sequence<Ts...>)
{
    ((vec[Ts] = std::get<Ts>(tp)), ...);
}

template <typename TupleT, typename VecT, std::size_t TupSize = std::tuple_size_v<TupleT>>
inline void copy_tuple_to_vector(const TupleT& tp, std::vector<VecT>& vec)
{
    assert(vec.size() == TupSize);
    copy_tuple_to_vector_Impl(tp, vec, std::make_index_sequence<TupSize>{});
}

}  // namespace coek
