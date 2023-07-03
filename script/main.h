//Copyright(C) 2022-2023 by Brian Bronz
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
#ifndef MAIN_H
#define MAIN_H
using namespace std;
//declare all classes
class Interval{
    public:
        double a;
        double b;

        Interval(const double a, const double b);
        const double getA();
        const double getB();
        const double getMidpoint();
        const double getWidth();
        const double getRadius();
        void setA(const double leftExactPart, const double rightExactPart, const double left_app_part, const double right_app_part);
        void setB(const double leftExactPart, const double rightExactPart, const double left_app_part, const double right_app_part);
};

class RangeFunc{
    protected:
        double a;
        double b;

        RangeFunc(const double a, const double b);
        virtual Interval getDf(Interval I, const int n, const int k);
        virtual Interval getF(Interval I, const int n, const int k);
};

class TaylorInterpolation: public RangeFunc{
    private:
        vector<double> coeffsFunc;
        Interval approximateInterval = Interval(0, 0);

    public:
        TaylorInterpolation(const vector<double> coeffsFunc, Interval S_kn, const double a, const double b);
        Interval g_k1(vector<double> coeffsFunc, const int k, Interval I, const bool Df);
        Interval S_kn(const int k, const int n, Interval I, vector<double> coeffsFunc, const bool Df);
        virtual Interval getDf(Interval I, const int n, const int k);
        virtual Interval getF(Interval I, const int n, const int k);
};

class LagrangeInterpolation: public RangeFunc{
    private:
        vector<double> coeffsFunc;
        double approxInterval;

    public:
        LagrangeInterpolation(const vector<double> coeffsFunc, const double approxInterval, double a, double b);
        vector<double> h_j(const int j, Interval I);
        double T_kn(const int n, Interval I);
        Interval quadratic_polynomials(Interval I, const vector<double> h_0);
        virtual Interval getDf(Interval I, const int n, const int k);
        virtual Interval getF(Interval I, const int n, const int k);
};

//declare all methods here
//basic methods
int factorial(const int n); 
double checkSign(const double value, const string sign); 

vector<vector<double> > parseFunc(const string func, vector<double> coeffsFunc, vector<double> powers, int idxVector, const int idx_x, const int i, const string sign);
vector<double> parsePolynomial(const string func, string sign);
string sortPolynomialFunct(string func);

//Descarte's rule
int NumPositiveRoots(const vector<string> vectorSign);
int NumNegativeRoots(const vector<string> vectorSign, const vector<double> vectorPower); 
bool existRealRoots(const string func, string sign);

//Cauchy's bounds
Interval setInitialInterval(const vector<double> coeffsFunc); //Tested (simple)

//Eval algorithm
Interval Interpolation(const double a, const double b, const vector<double> coeffsFunc, const string formInterpolation, const int k, const int n, const bool dF);
vector<double> kDerivative(vector<double> coeffsFunc, const int k);
double functionValue(const vector<double> coeffsFunc, const double xPoint);
Interval functionValue(const vector<double> coeffsFunc, Interval I);
vector<Interval> Eval(const vector<double> coeffsFunc, Interval initialBound, const string formInterpolation, const int k, const int n);

Interval Sum(Interval I, const double cons);
Interval Sum(Interval I, Interval J);
Interval MultiConstant(Interval I, const double cons);
Interval Multiplication(Interval I, Interval J);
Interval Substraction(Interval I, const double cons);
//Newton's method
void getRoots(const vector<double> coeffsFunc, vector<Interval> Z, const int maxIterations, const double tolerance, const double epsilon, const bool firstIteration, ofstream &file);

//Root finding algorithm
void FindingRoots(string func, const string formInterpolation, const int k, const int n, ofstream &file);
#endif