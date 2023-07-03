//Copyright(C) 2022-2023 by Brian Bronz
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include "Interval.c++"
using namespace std;

/**
 * @brief Determine the initial interval that contains all the real roots
 * 
 * @param coeffsFunc is the vector containing the coefficients of a trigonometric polynomial function f(x) of at least degree 2
 * @return Interval
 */
Interval setInitialInterval(const vector<double> coeffsFunc){
    double maxValue = 0;
    //Find the maximum ratio between the coefficient associated 
    //to the 'x' variable with the highest degree and the other coefficients
    for (int i = 1; i < coeffsFunc.size(); i++){
        if (abs(coeffsFunc[i] / coeffsFunc[0]) > maxValue){
            maxValue = abs(coeffsFunc[i] / coeffsFunc[0]);
        }
    }
    //Set the bounds of the interval
    return Interval(- (maxValue + 1), maxValue + 1);
}