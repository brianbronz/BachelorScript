//Copyright(C) 2022-2023 by Brian Bronz
#ifndef TAYLOR_INTERPOLATION_H
#define TAYLOR_INTERPOLATION_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include "Evaluate.c++"
#include "Range_function.c++"
#include "Interval.c++"
#include "simpleOperations.c++"
using namespace std;

/**
 * @brief Constructor for the class TaylorInterpolation
 * 
 * @param coeffsFunc is the coefficients of the function f(x)
 * @param approximateInterval is the interval of the remainder
 * @param a is the lower bound of the interval
 * @param b is the upper bound of the interval
 */
TaylorInterpolation::TaylorInterpolation(const vector<double> coeffsFunc, Interval approximateInterval, const double a, const double b) : RangeFunc(a, b){
    this->coeffsFunc = coeffsFunc;
    this->approximateInterval = approximateInterval;
};

/**
 * @brief Compute the interval of the exact part of the function f(x) in the interval I
 * 
 * @param coeffsFunct is the coefficients of the Taylor polynomial
 * @param k is the order of the convergence
 * @param I is the interval
 * @param Df is true if the derivative of the function is needed
 * @return Interval
 */
Interval TaylorInterpolation::g_k1(vector<double> coeffsFunct, const int k, Interval I, const bool Df){
    //Set the maximum degree of the function
    int sizeCoeffs = coeffsFunct.size();
    //Calculate the k-derivate
    const double midpoint = I.getMidpoint();
    vector<double> temp(k + 1, 0);
    int i = 0;
    int factValue = 1;
    while (i <= k && sizeCoeffs > 0){
        //Calculate the new coeffsFunc of the function 
        double functionVal = functionValue(coeffsFunct, midpoint);
        temp[i] = functionVal;
        for (int j = 0; j < sizeCoeffs; j++){
                coeffsFunct[j] = coeffsFunct[j] * (sizeCoeffs - j - 1);
        }
        //Calculate f^{(i)}(m)/i!
        if (Df){ // for the derivative => g_k1^{1}
            factValue = factValue * (i + 1);
            temp[i] = (i + 1) * temp[i] / factValue;
        } else { // for the original function => g_k1^{0}
            if (i != 0){factValue = factValue * i;}
            temp[i] = temp[i] / factValue;
        }
        //Remove the last coefficient and decrease the degree of the function
        coeffsFunct.pop_back();
        sizeCoeffs--;
        i++;
    }
    coeffsFunct = temp;
    reverse(coeffsFunct.begin(), coeffsFunct.end());
    return functionValue(coeffsFunct, I);
}
/**
 * @brief Compute the approximation of the function f(x) in the interval I
 * 
 * @param k is the order of the convergence
 * @param coeffsFunct is the coefficients of the Taylor polynomial
 * @param r is the radius of the interval
 * @param n is the level of the Taylor polynomial
 * @param I is the interval
 * @return Interval
 */
Interval TaylorInterpolation::S_kn(const int k, const int n, Interval I, vector<double> coeffsFunct, const bool Df){
    int sizeCoeffs = coeffsFunct.size();
    //Calculate the k-derivate
    vector<double> temp;
    //|f^{i}(m)|
    const double r = I.getRadius(); //The radius of the interval
    const double midpoint = I.getMidpoint();
    double r_i_k = 1;
    int i = k;
    int factVal = factorial(k);
    while (i <= n && sizeCoeffs != 0){
        //Calculate the new coeffsFunc of the function 
        double functionVal = functionValue(coeffsFunct, midpoint);
        temp.push_back(abs(functionVal));
        for (int j = 0; j < sizeCoeffs; j++){
                coeffsFunct[j] = coeffsFunct[j] * (sizeCoeffs- j - 1);
        }
        //Calculate f^{(i)}(m)/i!
        if (Df){
            factVal = factVal * (i + 1);
            temp[i - k] = (i + 1) * temp[i - k] / factVal;
        } else {
            if (i != k){factVal = factVal * i;}
            temp[i - k] = temp[i - k] / factVal;
            
        }
        //Remove the last coefficient and decrease the degree of the function
        coeffsFunct.pop_back();
        sizeCoeffs--;
        i++;
    }
    coeffsFunct = temp;
    //r^k
    double rk = r;
    //c_i * r^i
    double cr = r_i_k * coeffsFunct[0];
    for (int i = 1; i < coeffsFunct.size(); i++){
        r_i_k = r_i_k * r;
        cr = cr + coeffsFunct[i] * r_i_k; 
        if (i < k){rk = rk * r;}
    }

    Interval interval_rk = MultiConstant(Interval( - 1, 1), rk);
    return MultiConstant(interval_rk, cr);
};

/**
 * @brief Get the range function of the function f(x)
 * 
 * @param I is the interval
 * @param n is the level of convergence
 * @param k is the order of the convergence
 * @return Interval
 */
Interval TaylorInterpolation::getF(Interval I, const int n, const int k){
    const Interval exactInterval = g_k1(coeffsFunc, k - 1, I, false);
    coeffsFunc = kDerivative(coeffsFunc, k);
    approximateInterval = S_kn(k, n, I, coeffsFunc, false);
    return Sum(exactInterval, approximateInterval);
};

/**
 * @brief Get the range function of the first derivative f^{'}(x)
 * 
 * @param I is the interval
 * @param n is the level of convergence
 * @param k is the order of the convergence
 * @return Interval
 */
Interval TaylorInterpolation::getDf(Interval I, const int n, const int k){
    const vector<double> initialCoeffsFunc = coeffsFunc;
    coeffsFunc = kDerivative(coeffsFunc, 1);
    const Interval exactInterval = g_k1(coeffsFunc, k - 1, I, true);
    coeffsFunc = kDerivative(initialCoeffsFunc, k + 1);
    approximateInterval = S_kn(k + 1, n, I, coeffsFunc, true);
    return Sum(exactInterval, approximateInterval);
};
#endif
