//
// Created by skche on 01/10/2024.
//

#ifndef TEXT_SUMMARIZER_SPARSEVECTOR_H
#define TEXT_SUMMARIZER_SPARSEVECTOR_H

#include <unordered_map>

class SparseVector {
public:
    std::unordered_map<int, double> data;

    int size_;

    double norm_ = 0.0;

    double sum_ = 0.0;

    bool norm_tampered = false;

    bool sum_tampered = false;

    double default_value;

    explicit SparseVector(int size, double default_value = 0);

    double operator[](int index) const;

    double norm();

    void insert(int index, double value);

    double get(int index) const;

    int size() const;

    double sum();

    friend double dot(const SparseVector &a, const SparseVector &b);
};


#endif //TEXT_SUMMARIZER_SPARSEVECTOR_H
