//Copyright(C) 2022-2023 by Brian Bronz
#ifndef INTERVAL_H
#define INTERVAL_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#include "main.h"
using namespace std;

/**
 * @brief Constructor for the class Interval
 * 
 * @param a is the lower bound of the interval
 * @param b is the upper bound of the interval
 */
Interval::Interval(const double a, const double b){
    this->a = a;
    this->b = b;
};

/**
 * @brief Get the lower bound of the interval
 * 
 * @return double 
 */
const double Interval::getA(){
    return this->a;
};

/**
 * @brief Get the upper bound of the interval
 * @return double 
 */
const double Interval::getB(){
    return this->b;
};

/**
 * @brief Get the midpoint of the interval
 * 
 * @return double 
 */
const double Interval::getMidpoint(){
    return (this->a + this->b) / 2;
};

/**
 * @brief Get the width of the interval
 * 
 * @return double 
 */
const double Interval::getWidth(){
    return this->b - this->a;
};

/**
 * @brief Get the radius of the interval
 * 
 * @return double 
 */
const double Interval::getRadius(){
    return (this->b - this->a) / 2;
};

/**
 * @brief Set the lower bound
 * 
 * @param leftExactPart is the left bound of the exact part
 * @param rightExactPart is the right bound of the exact part
 * @param leftApprPart is the left bound of the approximate part
 * @param rightApprPart is the right bound of the approximate part
 */
void Interval::setA(const double leftExactPart, const double rightExactPart, const double leftApprPart, const double rightApprPart){
    //Compute the min value of the exact part
    const double minExactPart = min(leftExactPart, rightExactPart);
    //Compute the min value of the approximate part
    const double minApprPart = min(leftApprPart, rightApprPart);
    this->a = minExactPart + minApprPart;
};


/**
 * @brief Set the upper bound of the interval
 * 
 * @param leftExactPart is the left bound of the exact part
 * @param rightExactPart is the right bound of the exact part
 * @param leftApprPart is the left bound of the approximate part
 * @param rightApprPart is the right bound of the approximate part
 */
void Interval::setB(const double leftExactPart, const double rightExactPart, const double leftApprPart, const double rightApprPart){
    //Compute the max value of the exact part
    const double maxExactPart = max(leftExactPart, rightExactPart);
    //Compute the max value of the approximate part
    const double maxApprPart = max(leftApprPart, rightApprPart);
    this->b = maxExactPart + maxApprPart;
}; 

/**
 * @brief Sum of an interval and a constant
 * 
 * @param I is the interval
 * @param cons is the constant
 * @return Interval 
 */
Interval Sum(Interval I, const double cons){
    return Interval(I.getA() + cons, I.getB() + cons);
}

/**
 * @brief Sum of two intervals
 * 
 * @param I is the first interval
 * @param J is the second interval
 * @return Interval 
 */
Interval Sum(Interval I, Interval J){
    return Interval(I.getA() + J.getA(), I.getB() + J.getB());
}

/**
 * @brief Multiplication of an interval and a constant
 * 
 * @param I is the interval
 * @param cons is the constant
 * @return Interval 
 */
Interval MultiConstant(Interval I, const double cons){
    return Interval(I.getA() * cons, I.getB() * cons);
}

/**
 * @brief Multiplication of two intervals
 * 
 * @param I is the first interval
 * @param J is the second interval
 * @return Interval 
 */
Interval Multiplication(Interval I, Interval J){
    //Compute the four products, using the extremes of the intervals
    const double ac = I.getA() * J.getA();
    const double ad = I.getA() * J.getB();
    const double bc = I.getB() * J.getA();
    const double bd = I.getB() * J.getB();
    //Compute the min and max of the four products
    const double min_value = min(min(ac, ad), min(bc, bd));
    const double max_value = max(max(ac, ad), max(bc, bd));
    return Interval(min_value, max_value);
}

/**
 * @brief Substraction of an interval and a constant
 * 
 * @param I is the interval
 * @param cons is the constant
 * @return Interval 
 */
Interval Substraction(Interval I, const double cons){
    return Interval(I.getA() - cons, I.getB() - cons);
}
#endif