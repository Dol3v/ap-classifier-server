#pragma once

#include <string>
#include <set>
#include <memory>
#include "metric.hpp"

/// data entry
template<class Data, class Class>
class DataEntry {
public:
    Data data;
    Class data_class;

    DataEntry(Data data, Class class_);
};


/// KNN classifier
template<class Data, class Class>
class KnnClassifier {
private:
    int k;
    std::vector<DataEntry<Data, Class> *> data_set;
    std::set<Class> present_classes;
public:
    /// Constructs a classifier with a given k value.
    explicit KnnClassifier(int k);

    /// loads data into the classifier.
    void load_data(const Data &t, const Class &s);

    /// Predicts the class of a given data sample using the KNN algorithm and a given metric.
    Class predict(const Data &t, const Metric<Data>& metric);

     std::set<Class>* get_classes() const { return new std::set<Class>(present_classes); }

    ~KnnClassifier();
};


#include "knn.tpp"