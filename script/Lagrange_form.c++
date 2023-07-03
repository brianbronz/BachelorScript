//Copyright(C) 2022-2023 by Brian Bronz
#ifndef LAGRANGE_INTERPOLATION_H_INCLUDED
#define LAGRANGE_INTERPOLATION_H_INCLUDED
#include <string>
#include <cmath>
#include <vector>
#include <limits>
#include "Evaluate.c++"
#include "Interval.c++"
#include "Range_function.c++"
#include "main.h"
using namespace std;

/**
 * @brief Constructor for the class LagrangeInterpolation
 * 
 * @param coeffsFunc is the vector of coefficients of the function f(x)
 * @param approxInterval is the sum of the approximate part of the Lagrange polynomial
 * @param a is the lower bound of the interval
 * @param b is the upper bound of the interval
 */
LagrangeInterpolation::LagrangeInterpolation(const vector<double> coeffsFunc, const double approxInterval, const double a, const double b): RangeFunc(a, b){
    this->coeffsFunc = coeffsFunc;
    this->approxInterval = approxInterval;
};

/**
 * @brief Define the coefficients of the exact part of the Lagrange polynomial
 * 
 * @param j is the index of the Lagrange polynomial
 * @param I is the interval
 * @return vector<double> 
 */
vector<double> LagrangeInterpolation::h_j(const int j, Interval I){
    double r = I.getRadius();
    vector<double> cDerivative = kDerivative(coeffsFunc, 3 * j);
    //Compute f(a), f(m), f(b)
    double f_a = functionValue(cDerivative, I.getA());
    double f_m = functionValue(cDerivative, I.getMidpoint());
    double f_b = functionValue(cDerivative, I.getB());
    //coefficients d_{j,0}, d_{j,1}, d_{j,2}
    double d_j_0 = f_m;
    double d_j_1 = (f_b - f_a) / (2.0 * r); 
    double d_j_2 = (f_b - 2.0 * f_m + f_a) / (2.0 * r * r);
    //h_{j}(I) = d_j_2 * (x - m)^2 + d_j_1 * (x - m) + d_j_0
    return {d_j_0, d_j_1, d_j_2};
};

/**
 * @brief Define the T_{k,n}(I) for the Lagrange polynomial
 * 
 * @param n is the level of convergence
 * @param I is the interval
 * @return double 
 */
double LagrangeInterpolation::T_kn(const int n, Interval I){
    const double r = I.getRadius();
    double Omega3 = (r * r * r * sqrt(3.0))/ 27.0;
    const double initialOmega3 = Omega3;
    int j = 1;
    //sum_{j=1}^{n-1} Omega3^j * |h_{j}(I)|
    while (j < n && 3 * j <= coeffsFunc.size() - 1){
        //Compute the h_{j}(I)
        vector<double> temp_h_j = h_j(j, I);
        
        Interval tempInterval = quadratic_polynomials(I, temp_h_j);
        //Compute the magnitude of h_{j}(I)
        double magnitude_h_j = max(abs(tempInterval.getA()), abs(tempInterval.getB()));
        //Omega3^j
        if (j != 1){Omega3 *= initialOmega3;}
        approxInterval += Omega3 * magnitude_h_j;
        j++;
    }
    //Omega3^n * |f^{3n}(I)|
    if (3 * n <= coeffsFunc.size() - 1){
        vector<double> cDerivative = kDerivative(coeffsFunc, 3 * n);
        Interval tempInterval = functionValue(cDerivative, I);
        double magnitude_c_derivate = max(abs(tempInterval.getA()), abs(tempInterval.getB()));
        approxInterval += magnitude_c_derivate * Omega3 * initialOmega3;
    }
    //T_{3,n}(I):= sum_{j=1}^{n-1} Omega3^j * |h_{j}(I)| + Omega3^n * |f^{3n}(I)|
    return approxInterval;
};

/**
 * @brief Define the quadratic polynomials to obtain the interval for the exact part of the Lagrange polynomial
 * 
 * @param I is the interval
 * @param h_0 is the vector containing the coefficients of the exact part of the Lagrange polynomial
 * @return Interval
 */
Interval LagrangeInterpolation::quadratic_polynomials(Interval I, const vector<double> h_0){
    Interval interval(0, 0);
    //Set the radius of the interval
    double r = I.getRadius();
    double c0 = h_0[0];
    double c1 = h_0[1];
    double c2 = h_0[2];
    //Compute the interval
    double value_a = c0 + (- c1 + c2 * r) * r;
    double value_b = c0 + (c1 + c2 * r) * r;
    interval.a = min(value_a, value_b);
    interval.b = max(value_a, value_b);
    if (abs(c1) < 2 * abs(c2) * r){
        double gxValue = c0 - c1 * c1 / (4 * c2);
        if (c2 > 0){
            interval.a = gxValue;
        } else {
            interval.b = gxValue;
        }
    } 
    return interval;
};

/**
 * @brief Get the range function f(x)
 * 
 * @param I is an interval
 * @param n is an integer number for the level of convergence
 * @param k is an integer number for the order of convergence
 * @return vector<double> 
 */
Interval LagrangeInterpolation::getF(Interval I, const int n, const int k){
    vector<double> h_0 = h_j(0, I);
    approxInterval = T_kn(n, I);
    //Set the function coeffsFunc with the exact part of the function
    Interval exactInterval = quadratic_polynomials(I, h_0);
    return Interval(exactInterval.a - approxInterval, exactInterval.b + approxInterval);
}

/**
 * @brief Get the range function f^{'}(x)
 * 
 * @param I is an interval
 * @param n is an integer number for the level of convergence
 * @param k is an integer number for the order of convergence
 * @return vector<double>
 */
Interval LagrangeInterpolation::getDf(Interval I, const int n, const int k){
    const vector<double> h_0_d = h_j(0, I);
    vector<double> hDerivative = {h_0_d[1], 2 * h_0_d[2], 0};
    approxInterval = T_kn(n, I);
    const double flag = (3.0 * sqrt(3.0)) / I.getRadius();
    Interval exactInterval = quadratic_polynomials(I, hDerivative);
    return Interval(exactInterval.a - approxInterval * flag, exactInterval.b + approxInterval * flag);
}
#endif