//Copyright(C) 2022-2023 by Brian Bronz
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include <fstream>
#include "main.h"
#include "Cauchy.c++"
#include "Evaluate.c++"
#include "Lagrange_form.c++"
#include "Parsing.c++"
#include "Taylor_form.c++"
#include "Newton.c++"
#include <ctime>
using namespace std;

/**
 * @brief Find all the real roots of a trigonometric polynomial functuon f(x)
 * 
 * @param func is the trigonometric polynomial function
 * @param formInterpolation is string that indicates the form of the interpolation, i.e. Taylor or Lagrange 
 * @param k is the level of convergence
 * @param n is the order of convergence
 * @param file is the file where the results are written
 */
void FindingRoots(string func, const string formInterpolation, const int k, const int n, ofstream &file, ofstream &timeFile, ofstream &iterationFile){
    clock_t start, end;
    int numIterations = 2;
    bool firstIteration = true;
    vector<double> timeTaken;
    //Sort the expression in case of a polynomial function
    func = sortPolynomialFunct(func);
    //Get the coefficients of the function
    const vector<double> coeffsFunc = parsePolynomial(func, "+");
    //Set the initial interval
    const Interval initialBound = setInitialInterval(coeffsFunc);
    while(numIterations > 0){
        //EVAL algorithm
        start = clock();
        vector<Interval> Z = Eval(coeffsFunc, initialBound, formInterpolation, k, n, iterationFile);
        //Get the real roots of the function
        const int maxIterations = 100;
        const double tolerance = 1e-10;
        const double epsilon = 1e-10;
        getRoots(coeffsFunc, Z, maxIterations, tolerance, epsilon,  firstIteration, file);
        Z.clear();
        end = clock();
        const double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        if (firstIteration){
            file << "Time taken by program is : " << fixed << time_taken << setprecision(5) << " sec " << endl;
            firstIteration = false;
        }
        timeTaken.push_back(time_taken);
        numIterations--;
    }
    double AverageTime = 0;
    for (int i = 0; i < timeTaken.size(); i++){
        AverageTime += timeTaken[i];
    }
    AverageTime = AverageTime / timeTaken.size();
    //write the average time taken in ms
    timeFile << fixed << AverageTime * 1000 << setprecision(5) << " ";
    //timeFile << "Average time taken by program is : " << fixed << AverageTime * 1000 << setprecision(5) << " ms " << endl;
    timeTaken.clear();
}