//Copyright(C) 2022-2023 by Brian Bronz
#ifndef EVALUATE_H
#define EVALUATE_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <queue>
#include <deque>
#include "main.h"
using namespace std;

/**
 * @brief Using one of the implemented interpolations (Lagrange or Taylor) to compute the range function f(x), respectively f'(x)
 * 
 * @param a is the lower bound of the interval
 * @param b is the upper bound of the interval
 * @param coeffsFunc is the coefficients of the function f(x), respectively f^{'}(x)
 * @param formInterpolation is the form of the interpolation (Taylor or Lagrange)
 * @param k is the order of the convergence
 * @param n is the level of the convergence
 * @param dF is a boolean that indicates if the derivative function is used
 * @return Interval
 */
Interval Interpolation(const double a, const double b, const vector<double> coeffsFunc, const string formInterpolation, const int k, const int n, const bool dF){
    Interval I = Interval(a, b);
    if (formInterpolation == "Taylor"){
        //Taylor interpolation
        TaylorInterpolation T = TaylorInterpolation(coeffsFunc, Interval(0, 0), a, b);
        (dF)? I = T.getDf(I, n, k): I = T.getF(I, n, k);
    } else if (formInterpolation == "Lagrange") {
        //Lagrange interpolation
        LagrangeInterpolation L = LagrangeInterpolation(coeffsFunc, 0, a, b);
        (dF)? I = L.getDf(I, n, 3): I = L.getF(I, n, 3);
    } else {
        //Error
        return Interval(0, 0);
    }
    return I;
}

/**
 * @brief Compute the k-derivate of the function
 * 
 * @param coeffsFunc is the coefficients of the polynomial function f(x)
 * @param k is the number of derivate to calculate
 * @return vector<double> 
 */
vector<double> kDerivative(vector<double> coeffsFunc, const int k){
    //Set the maximum degree of the function
    int sizeCoeffs = coeffsFunc.size();
    //Check if the number of derivate is greater than the max degree of the function
    if (k >= sizeCoeffs){
        return {0};
    }

    //Calculate the k-derivate, resizing the coeffsFunc vector
    coeffsFunc.resize(sizeCoeffs - k);
    for (int i = 0; i < k; i++){
        //Calculate the new coeffsFunc of the function 
        for (int j = 0; j < sizeCoeffs - i; j++){
            coeffsFunc[j] = coeffsFunc[j] * (sizeCoeffs - j - 1);
        } 
        sizeCoeffs--;
    }
    return coeffsFunc;
}

/**
 * @brief Evaluate the function in a point 'x' using the Horner method
 * 
 * @param coeffsFunc is the coefficients of the function f(x)
 * @param xPoint is a double point where the function is evaluated
 * @return double 
 */
double functionValue(const vector<double> coeffsFunc, const double xPoint){
    double res = coeffsFunc[0];
    for (int i = 1; i < coeffsFunc.size(); i++){
        res = res * xPoint + coeffsFunc[i];
    }
    return res;
}

/**
 * @brief Get a new interval, evaluating the function in the midpoint of the interval
 * 
 * @param coeffsFunc is the coefficients of the function f(x)
 * @param I is the interval of the function f(x)
 * @return Interval
 */
Interval functionValue(const vector<double> coeffsFunc, Interval I){
     Interval res = Interval(coeffsFunc[0], coeffsFunc[0]);
     Interval I_m = Substraction(I, I.getMidpoint());
    for (int i = 1; i < coeffsFunc.size(); i++){
        Interval tempMul = Multiplication(res, I_m);
        Interval tempSum = Sum(tempMul, coeffsFunc[i]);
        res = tempSum;
    }
    if (res.getA() == res.getB()){
        res = Interval( - abs(res.getA()), abs(res.getB()));
    }
    return res;
}

/**
 * @brief Get all the subintervals, each of them has only one root
 * 
 * @param coeffsFunc is the coefficient of the function f(x)
 * @param initialBound is the interval containing all real roots
 * @param formInterpolation is a string for Taylor or Lagrange interpolation
 * @param k is the order of convergence
 * @param n is the level of convergence
 * @return vector<Interval> 
 */
vector<Interval> Eval(const vector<double> coeffsFunc, Interval initialBound, const string formInterpolation, const int k, const int n, ofstream &iterationFile){
    queue<Interval> Q;
    Q.push(initialBound);
    vector<Interval> Z;
    double counter = 0;
    //create a new file to store the results
    while(!Q.empty()){
        Interval J = Q.front();
        Q.pop();
        double a = J.a;
        double b = J.b;
        //Compute the range function f(x) in the interval [a, b]
        Interval funcInterval = Interpolation(a, b, coeffsFunc, formInterpolation, k, n, false);
        counter++;
        if (funcInterval.getA() < 0 && funcInterval.getB() > 0){
            //Compute the range function f'(x) in the interval [a, b]
            Interval dFuncInterval = Interpolation(a, b, coeffsFunc, formInterpolation, k, n, true);
            counter++;
            if (dFuncInterval.getA() < 0 && dFuncInterval.getB() > 0){
                //Divide the interval in two subintervals
                double m = (a + b) / 2;
                Q.push(Interval(a, m));
                Q.push(Interval(m, b));
            } else if (functionValue(coeffsFunc, a) * functionValue(coeffsFunc, b) <= 0) {
                //Store the subinterval that contains only one root
                Z.push_back(Interval(a, b));
            }
        }
    }
    iterationFile << counter << " ";
    return Z;
}
#endif