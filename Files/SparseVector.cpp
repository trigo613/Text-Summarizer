//
// Created by skche on 01/10/2024.
//

#include <cmath>
#include "SparseVector.h"

SparseVector::SparseVector(int size, double default_value) {
    this->size_ = size;
    this->default_value = default_value;
}


double SparseVector::operator[](int index) const {
    if (data.find(index) == data.end()) {
        return default_value;
    }
    return data.at(index);
}

double SparseVector::norm() const{
    double sum = 0;
    for (const auto& pair : data) {
        sum += pair.second * pair.second;
    }
    return std::sqrt(sum);
}

double dot(const SparseVector &a, const SparseVector &b) {
    double sum = 0;
    for (const auto& pair : a.data) {
        sum += pair.second * b[pair.first];
    }
    return sum;
}


double SparseVector::sum() const {
    double sum = 0;
    for (const auto& pair : data) {
        sum += pair.second;
    }
    return sum;
}

int SparseVector::size() const {
    return size_;
}


void SparseVector::insert(int index, double value) {
    if (value != default_value) {
        data[index] = value;
    }
}

