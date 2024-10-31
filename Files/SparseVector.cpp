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

double SparseVector::norm(){
    if (!norm_tampered) {
        return norm_;
    }
    double sum = 0;
    for (const auto& pair : data) {
        sum += pair.second * pair.second;
    }
    norm_ = std::sqrt(sum);
    norm_tampered = false;
    return norm_;
}

double dot(const SparseVector &a, const SparseVector &b) {
    double sum = 0;
    for (const auto& pair : a.data) {
        sum += pair.second * b[pair.first];
    }
    return sum;
}


double SparseVector::sum() {
    if (!sum_tampered) {
        return sum_;
    }
    double sum = 0;
    for (const auto& pair : data) {
        sum += pair.second;
    }
    sum_ = sum;
    sum_tampered = false;
    return sum_;
}

int SparseVector::size() const {
    return size_;
}


void SparseVector::insert(int index, double value) {
    if (value != default_value) {
        data[index] = value;
        norm_tampered = true;
        sum_tampered = true;
    }
}

double SparseVector::get(int index) const {
    if (data.find(index) == data.end()) {
        return default_value;
    }
    return data.at(index);
}

