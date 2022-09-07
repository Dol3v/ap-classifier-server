#include <stdexcept>
#include <cmath>
#include "metric.hpp"

template<class T>
double Metric<T>::operator()(const T &t1, const T &t2) const {
    throw std::logic_error("unimplemented function");
}

//template <class T>
//std::ostream& operator<<(std::ostream& os, const Metric<T>& metric) {
//    os << metric.name;
//    return os;
//}

double EuclideanMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double diff = 0;
    for (int i = 0; i < t.size(); i++) {
        diff += std::pow(t.at(i) - t1.at(i), 2);
    }
    return pow(diff, 1. / (double) t.size());
}

double ManhattanMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double distance = 0;
    for (int i = 0; i < t.size(); i++) {
        distance += std::abs(t.at(i) - t1.at(i));
    }
    return distance;
}

double ChebyshevMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double distance = 0;
    for (int i = 0; i < t.size(); i++) {
        distance = std::max(distance, std::abs(t.at(i) - t1.at(i)));
    }
    return distance;
}

Metric<std::vector<double>> *metric_from_name(const std::string &name) {
    if (name == "EUC")
        return new EuclideanMetric;
    if (name == "CHE")
        return new ChebyshevMetric;
    if (name == "MAN")
        return new ManhattanMetric;
    return nullptr;
}