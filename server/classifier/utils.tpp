#include <random>
#include <utility>

namespace utils {

    /// swaps two variables that implement the deref operator
    template<typename Iter>
    void swap(Iter a, Iter b) {
        auto temp = *a;
        *a = *b;
        *b = temp;
    }

    /// partitions the iterable to contain elements less than the pivot on the left,
    /// and elements bigger than it on its right, using the given predicate.
    template<typename RAIter, typename Pred>
    RAIter partition(RAIter begin, RAIter end, RAIter pivot, Pred comp) {
        typedef typename std::iterator_traits<RAIter>::value_type value_type;
        auto pivot_value = *pivot;
        // moving pivot to the end
        utils::swap(end - 1, pivot);

        RAIter store = begin;
        value_type temp;
        for (auto i = begin; i != end - 1; i++) {
            if (comp(*i, pivot_value)) {
                // move smaller value to the left of the pivot index
                utils::swap(i, store);
                store++;
            }
        }
        // move the pivot to its calculated index
        utils::swap(store, end - 1);
        return store;
    }

    /// runs the quick select algorithm without type-checking. Accepts the random number generator in addition
    /// to the other arguments.
    template<typename RAIter, typename Pred>
    RAIter _quick_select(RAIter begin, RAIter end, uint32_t k, Pred comp, std::mt19937 rng) {
        // creating uniform distribution
        std::uniform_int_distribution<uint32_t> dist(0, end - begin - 1);
        // base case
        if (begin == end) {
            return begin;
        }
        uint32_t pivot_index = dist(rng);
        RAIter pivot = partition(begin, end, begin + pivot_index, comp);
        if (begin == end - 1) {
            // recursion should end
            return pivot;
        }
        if (pivot - begin == k) {
            return pivot;
        } else if (pivot - begin > k) {
            return _quick_select(begin, pivot, k, comp, rng);
        }
        return _quick_select(pivot + 1, end, k - (pivot - begin + 1), comp, rng);
    }

    template<typename RAIter, typename Pred>
    RAIter quick_select(RAIter begin, RAIter end, const uint32_t k, Pred comp) {
        // asserting that we have a random iterator
        static_assert(std::is_base_of<std::random_access_iterator_tag,
                              typename std::iterator_traits<RAIter>::iterator_category>::value,
                      "Iterator must be a random-access iterator");
        // initializing rng
        std::random_device dev;
        std::mt19937 rng(dev());
        return _quick_select(begin, end, k, comp, rng);
    }

    /// Returns the max element of an iterable.
    ///
    /// Iter - iterator type
    /// Pred - comparison predicate
    template<typename Iter, typename Pred>
    typename std::iterator_traits<Iter>::value_type
    max_element(Iter begin, Iter end, Pred comp) {
        if (begin == end) throw std::logic_error("Invalid parameter: end and begin are equal");
        auto max = begin;
        while (++begin != end) {
            if (comp(*max, *begin)) max = begin;
        }
        return *max;
    }

    /// Converts a tuple of type T and into a vector of the same type and size containing  the first
    /// 4 elements of the tuple.
    template <typename T, typename... Rest>
    std::vector<T> tup2vec4(std::tuple<T, T, T, T, Rest...> values) {
        std::vector<T> data;
        data.push_back(std::get<0>(values));
        data.push_back(std::get<1>(values));
        data.push_back(std::get<2>(values));
        data.push_back(std::get<3>(values));
        return data;
    }
};

