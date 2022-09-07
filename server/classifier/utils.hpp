#pragma once
#include <iterator>
#include <vector>
#include <tuple>

namespace utils {

    /// Runs quick-select on a random-access iterator. The algorithm modifies the iterable in-place.
    ///
    /// Types:
    /// RAIter - random access iterator
    /// Pred - comparison predicate for the iterator elements
    ///
    /// Arguments:
    /// begin, end - begin and end of the iterable
    /// comp - comparison predicate for the iterable's elements
    template <typename RAIter, typename Pred>
    RAIter quick_select(RAIter begin, RAIter end, uint32_t k, Pred comp);

    /// Returns the max element of an iterator.
    template <typename Iter, typename Pred>
    typename std::iterator_traits<Iter>::value_type
    max_element(Iter begin, Iter end, Pred comp);

    template <typename T, typename... Rest>
    std::vector<T> tup2vec4(std::tuple<T, T, T, T, Rest...> values);
}

#include "utils.tpp"