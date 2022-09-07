#include "metric.hpp"
#include "utils.hpp"
#include <tuple>
#include "metric.hpp"
#include <map>
#include <iostream>

template<class Data, class Class>
DataEntry<Data, Class>::DataEntry(Data data, Class class_) : data(data), data_class(class_) {
}

template<class Data, class Class>
KnnClassifier<Data, Class>::KnnClassifier(int k) : k(k) {
    std::cout << "model loaded with param " << k << "\n";
}

template<class Data, class Class>
void KnnClassifier<Data, Class>::load_data(const Data &t, const Class &s) {
    auto *data = new DataEntry<Data, Class>(t, s);
    data_set.push_back(data);
    present_classes.insert(s);
}

template<class Data, class Class>
Class KnnClassifier<Data, Class>::predict(const Data &t, const Metric<Data>& metric) {
    if (this->data_set.size() < k) {
        throw std::logic_error("insufficient data loaded to make a prediction");
    }
    // calculate distances
    std::vector<std::pair<double, Class>> distances;
    for (auto entry: data_set) {
        distances.push_back(std::pair<double, Class>(metric(t, entry->data), entry->data_class));
    }
    // order first k elements
    using pair = std::pair<double, Class>;
    auto comp = [](const pair &c1, const pair &c2) {
        return c1.first < c2.first;
    };
    utils::quick_select(distances.begin(), distances.end(), k + 1, comp);

    // count occurrences per class
    std::map<Class, int> occurrences;
    for (auto it = distances.begin(); it != distances.begin() + k; ++it) {
        if (occurrences.find(it->second) == occurrences.end()) {
            occurrences.insert(std::pair<Class, double>(it->second, 0));
        }
        ++occurrences[it->second];
    }
    // return max element
    return utils::max_element(occurrences.begin(), occurrences.end(),
                              [](const std::pair<Class, int> &p1, const std::pair<Class, int> &p2) {
                                  return p1.second < p2.second;
                              }).first;
}

template<class Data, class Class>
KnnClassifier<Data, Class>::~KnnClassifier() {
    for (auto entry: data_set) {
        delete entry;
    }
}
