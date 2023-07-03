//Copyright(C) 2022-2023 by Brian Bronz
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include "main.h"
using namespace std;

/**
 * @brief Find the real roots of a function using the Newton method
 * 
 * @param coeffsFunc is the coefficients of the function f(x)
 * @param Z is a vector containing the subintervals
 * @param maxIterations is the maximum number of iterations
 * @param tolerance is the tolerance of the method
 * @param epsilon is the tolerance of the derivative
 * @param firstIteration is a boolean that indicates if it is the first iteration and if the results must be written in a file
 * @param file is the file where the results are written
 */
void getRoots(const vector<double> coeffsFunc, vector<Interval> Z, const int maxIterations, const double tolerance, const double epsilon, const bool firstIteration, ofstream &file){
    const vector<double> cDerivative = kDerivative(coeffsFunc, 1);;
    for (int i = 0; i < Z.size(); i++){
        double x0 = Z[i].getMidpoint();
        double res = functionValue(cDerivative, x0);
        if (res == 0){
            //change the initial guess
            x0 = Z[i].getA();
        }
        double x_old = x0;
        double x1;
        double iteration = 0;
        //Set the bounds of the interval
        const double a = Z[i].getA();
        const double b = Z[i].getB();
        while (iteration < maxIterations){
            //Compute the value of the function and its derivative
            double y0 = functionValue(coeffsFunc, x_old);
            double yDerivative = functionValue(cDerivative, x_old);
            if (yDerivative == 0 || abs(yDerivative) < epsilon){ break;}
            //Compute the new value of x
            x1 = x_old - y0 / yDerivative;
            if (abs(x1 - x_old) < tolerance){break;}
            //Update the values of x_old and iteration
            x_old = x1;
            if (x1 < a || x1 > b || x_old > b || x_old < a){break;}
            iteration++;
        }
        if (iteration == maxIterations && abs(x1 - x_old) > tolerance){
            break;
        } else {
            //write the root and the interval in the file
            if (firstIteration){
                file << "The root is " << x1 << " in the interval [" << a << ", " << b << "]" " in "<< iteration << " iterations" << endl;
            }
        }
    }
} 
