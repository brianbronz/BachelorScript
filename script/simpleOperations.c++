//Copyright(C) 2022-2023 by Brian Bronz
#ifndef simpleOperations_H
#define simpleOperations_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

/**
 * @brief Calculate the factorial of a number
 * 
 * @param n is a number
 * @return int
 */
int factorial(const int n){
    int res = 1;
    //Calculate the factorial from 1 to n
    for (int i = 1; i <= n; i++){
        res = res * i;
    }
    return res;
};

/**
 * @brief Check the sign of a number
 * 
 * @param value is the value of the number
 * @param sign is the sign of the number
 * @return double  
 */
double checkSign(const double value, const string sign){
    return (sign == "-") ?  (- value) : (value);
};
#endif