//Copyright(C) 2022-2023 by Brian Bronz
#ifndef RANGE_FUNCTION_H
#define RANGE_FUNCTION_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include "main.h"
using namespace std;

/**
 * @brief Construct a new RangeFunc::RangeFunc object
 * 
 * @param a is the lower bound of the interval
 * @param b is the upper bound of the interval
 */
RangeFunc::RangeFunc(const double a, const double b){
            this->a = a;
            this->b = b;
};

/**
 * @brief Get the derivative function
 * 
 * @param I is the interval
 * @param n is the level of the convergence
 * @param k is the order of the convergence
 * @return vector<double> 
 */
Interval RangeFunc::getDf(Interval I, const int n, const int k){
    return Interval(0, 0);
};

/**
 * @brief Get the function
 * 
 * @param I is the interval
 * @param n is the level of the convergence
 * @param k is the order of the convergence
 * @return vector<double> 
 */
Interval RangeFunc::getF(Interval I, const int n, const int k){
    return Interval(0, 0);
};
#endif 