#pragma once

#include <vector>

/// Measures distance between two data points.
template<class T>
class Metric {
private:
    const std::string name;
public:
    explicit Metric(std::string name) : name(std::move(name)) {}
    virtual double operator()(const T &, const T &) const;

    template<class Param>
    friend std::ostream &operator<<(std::ostream &os, const Metric<Param> &metric) {
        os << metric.name;
        return os;
    }
};

// used to prevent linker errors
template
class Metric<std::vector<double>>;

/// The usual Euclidean metric.
class EuclideanMetric : public Metric<std::vector<double>> {
public:
    EuclideanMetric() : Metric<std::vector<double>>("EUC") {}
    virtual double operator()(const std::vector<double> &t, const std::vector<double> &t1) const;
};

/// The Manhattan distance function.
class ManhattanMetric : public Metric<std::vector<double>> {
public:
    ManhattanMetric() : Metric<std::vector<double>>("MAN") {}
    virtual double operator()(const std::vector<double> &t, const std::vector<double> &t1) const;
};

/// The Chebyshev distance function.
class ChebyshevMetric : public Metric<std::vector<double>> {
public:
    ChebyshevMetric() : Metric<std::vector<double>>("CHE") {}
    double operator()(const std::vector<double> &t, const std::vector<double> &t1) const override;
};

/// Constructs a metric from its name, returns null if not existing.
Metric<std::vector<double>> *metric_from_name(const std::string &);
