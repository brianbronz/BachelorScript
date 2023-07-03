//Copyright(C) 2022-2023 by Brian Bronz
#ifndef PARSING_H
#define PARSING_H
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

/**
 * @brief Parse the expression
 * 
 * @param func is the function to parse
 * @param coeffsFunc is the vector of the coefficients function
 * @param powers is the vector representing the powers of the variable
 * @param idxVector is the index of the vector
 * @param idx_x is the index of the x variable
 * @param i is the index of the function
 * @param sign is the sign of the coefficient
 * @return vector<vector<double> > 
 */
vector<vector<double> > parseFunc(const string func, vector<double> coeffsFunc, vector<double> powers, int idxVector, const int idx_x, const int i, const string sign){
    int idxSpace = func.find(' ', idx_x);
    double expValue;
    //Check if the index of the x variable precedes the space, if yes the exponent is 1
    (idx_x + 1 != idxSpace && idxSpace != -1) ? 
        expValue = stoi(func.substr(idx_x + 2, idxSpace - idx_x - 2)) : expValue = 1;
    //Check if in the vector powers there is a value equal to the exponent value + 1 such that you have two consecutive exponents
    if (idxVector != 0 && powers[idxVector - 1] != expValue + 1){
        //add 0 to the vector coeffsFunc and the expValue to the vector powers
        const int difference = powers[idxVector - 1] - expValue ;
        int j = 1;
        //Add 0 values to the vector coeffsFunc and add exponent values to the vector powers
        //To obtain an expression like 2x^3 + 0x^2 + 0x^1 + 0
        while (j < difference){
            powers.push_back(powers[idxVector - 1] - j);     
            coeffsFunc.push_back(0);       
            idxVector++;
            j++;
        }
    }
    //get the coefficient of a single term
    double coeffValue;
    if (func[i + 1] == ' '){
        (idx_x - i == 2) ? coeffValue = 1 : coeffValue = stod(func.substr(i + 2, idx_x - i - 2));
    } else {
        (func[i] == 'x')? coeffValue = 1 :
            coeffValue = stod(func.substr(i, idx_x - i));
        
    }
    const vector<double> idxToStore = {(double) idxVector, (double) idxSpace}; 
    coeffsFunc.push_back(checkSign(coeffValue, sign));
    powers.push_back(expValue);
    return {coeffsFunc, powers, idxToStore};
}

/**
 * @brief Parse and obtain the coefficients of the polynomial function f(x)
 * 
 * @param func is the function to parse
 * @param sign is the first sign of the function
 * @return vector<double> 
 */
vector<double> parsePolynomial(const string func, string sign = "+"){
    vector<double> coeffsFunc, powers; 
    int idxVector = 0;
    for (int i = 0; i < func.length(); i++){
        //Find the sign
        (func[i] == '+' || func[i] == '-') ? sign = func[i] : sign = "+";
        //Find the coefficient starting from the value i
        int idx_x = func.find('x', i);
        int idxSpace = -1;
        if (idx_x != -1){
            //If there is x, set a vector with the coefficient and the exponent
            const vector<vector<double> > cDerivative = parseFunc(func, coeffsFunc, powers, idxVector, idx_x, i, sign);
            coeffsFunc = cDerivative[0];
            powers = cDerivative[1];
            idxVector = cDerivative[2][0];
            idxSpace = cDerivative[2][1];
        } else {
            //If there is no x; push the constant and the exponent 0
            const double cons = stod(func.substr(i + 2));
            coeffsFunc.push_back(checkSign(cons, sign));
            powers.push_back(0);
            break;
        }
        idxVector++;
        if (idxSpace != -1){
            i = idxSpace;
        } else {
            break;
        }
    }
    //Sort the expression by the powers
    int maxDegree = powers[0];
    vector<double> coeffsToStore(maxDegree + 1), powersToStore(maxDegree + 1);
    int currentIdx = 0;
    //Create a vector with the powers to calculate
    while(maxDegree >= 0){
        coeffsToStore[currentIdx] = 0;
        powersToStore[currentIdx] = maxDegree;
        maxDegree--;
        currentIdx++;
    }
    int i = 0;
    //Compute the coefficients
    while(i < powersToStore.size()){
        bool findExp = false;
        int idxToChange;
        int j = 0;
        while(j < powers.size()){
            if (powersToStore[i] == powers[j]){
                findExp = true;
                idxToChange = j;
                break;
            }
            j++;
        }
        //If the power is found, change the value of the coefficient
        if (findExp){
            powersToStore[i] = powers[idxToChange];
            coeffsToStore[i] = coeffsFunc[idxToChange];
        }
        i++;
    }
    coeffsFunc.clear();
    powers.clear();
    powersToStore.clear();
    return coeffsToStore;
}

/**
 * @brief Sort the expression according to the powers
 * 
 * @param func is the function to parse
 * @return string 
 */
string sortPolynomialFunct(string func){
    string sign = "+";
    string sortedFunc;
    //Count the number of variables x
    int numXvariable = count(func.begin(), func.end(), 'x');
    //Initialize the vectors to store the coefficients and the exponents of size numXvariable + 1
    vector<double> coeffsFunc(numXvariable + 1, 0), exp(numXvariable + 1, 0);
    vector<string> vectorSign(numXvariable + 1, "+");
    int currentIdx = 0;
    double expValue, coeffsValue;
    for (int i = 0; i < func.length(); i++){
        //Get the sign of the current term
        (func[i] == '+' || func[i] == '-') ? sign = func[i] : sign = "+";
        vectorSign[currentIdx] = sign;
        int idx_x = func.find('x', i);
        if (idx_x == -1){
            //If there is no x; push the constant
            exp[currentIdx] = 0;
            (func[i] == '+' || func[i] == '-') ? coeffsValue = stod(func.substr(i + 2)) : coeffsValue = stod(func.substr(i));
            coeffsFunc[currentIdx] = checkSign(coeffsValue, sign);
            break;
        } else {
            int idxSpace;
            //Check if f[i] is a sign and find the next space
            (func[i] == '+' || func[i] == '-') ? idxSpace = func.find(' ', i + 2) : idxSpace = func.find(' ', i);
            if (idx_x > idxSpace && idx_x != -1 && idxSpace != -1){
                //Check for the coefficient
                (func[i] == '+' || func[i] == '-') ? coeffsValue = stod(func.substr(i + 2, idxSpace)) : coeffsValue = stod(func.substr(i, idxSpace));
                coeffsFunc[currentIdx] = coeffsValue;
                exp[currentIdx] = 0;
                i = idxSpace;
            } else {
                if (func[i] == '+' || func[i] == '-'){
                    //If there is a coefficient; example + 2x or - x
                    (i + 2 == idx_x) ? coeffsValue = 1 : coeffsValue = stod(func.substr(i + 2, idx_x - 1));
                } else {
                    //If it isn't a sign; check for the coefficient example 2x or x
                    (i == idx_x) ? coeffsValue = 1 : coeffsValue = stod(func.substr(i, idx_x - i));
                }
                coeffsFunc[currentIdx] = coeffsValue;
                //Check if there is an exponent
                int idxExp = func.find('^', idx_x);
                if (idxExp == idx_x + 1){
                    //If there is an exponent; example x^2
                    expValue = stod(func.substr(idxExp + 1, idxSpace));
                    exp[currentIdx] = expValue;
                } else {
                    //If there is no exponent; example x
                    exp[currentIdx] = 1;
                }
                i = idxSpace;
            }
        }
        currentIdx++;
        if (i == -1){break;}
    }
    //Sort the vectors by the exponent
    for (int i = 0; i < exp.size(); i++){
        for (int j = i + 1; j < exp.size(); j++){
            if (exp[i] < exp[j]){
                swap(exp[i], exp[j]);
                swap(coeffsFunc[i], coeffsFunc[j]);
                swap(vectorSign[i], vectorSign[j]);
            }
        }
    }
    int i = 0;
    //Create the string function with the ordered expression
    while (i < exp.size()){
        string sign = vectorSign[i];
        if (exp[i] == 0){
            (sortedFunc == "")?
                sortedFunc = sign + " " + to_string(coeffsFunc[i]):
                sortedFunc += " " + sign + " " + to_string(coeffsFunc[i]);
        } else if (exp[i] == 1){
            (sortedFunc == "")?
                sortedFunc = sign + " " + to_string(coeffsFunc[i]) + "x":
                sortedFunc += " " + sign + " " + to_string(coeffsFunc[i]) + "x";
        } else {
            (sortedFunc == "")?
                sortedFunc = sign + " " + to_string(coeffsFunc[i]) + "x^" + to_string(exp[i]):
                sortedFunc += " " + sign + " " + to_string(coeffsFunc[i]) + "x^" + to_string(exp[i]);
        }
        i++;
    }
    return sortedFunc;
}

/**
 * @brief Sort and add the coefficients of the same exp
 * 
 * @param func is the polynomial function
 * @param approxFunc is the approximate polynomial function
 * @return string to represent the polynomial 
 */
string sortTrigFunc(string func, string approxFunc){
    vector<string> vectorSign;
    string sign = "+";
    string trigFunc = func.substr(1, func.find(')') - 1);
    trigFunc = sortPolynomialFunct(trigFunc);
    //Parse the coefficients of the trigonometric function
    vector<double> coeffsTrigFunc = parsePolynomial(trigFunc, "+");
    //Parse the coefficients of the approximate function
    vector<double> approxTrigFuncArg = parsePolynomial(approxFunc, "+");
    reverse(coeffsTrigFunc.begin(), coeffsTrigFunc.end());
    reverse(approxTrigFuncArg.begin(), approxTrigFuncArg.end());
    //Multiply the two vectors
    vector<double> res(coeffsTrigFunc.size() + approxTrigFuncArg.size() - 1, 0);
    //Iterate through all the coefficients of the first part and multiply them with the coefficients of the approximate function
    for (int i = 0; i < coeffsTrigFunc.size(); i++){
        for (int j = 0; j < approxTrigFuncArg.size(); j++){
            res[i + j] += coeffsTrigFunc[i] * approxTrigFuncArg[j];
        }
    }
    reverse(res.begin(), res.end());
    string actExpr = "";
    //Res vector is from the highest exponent to the lowest exponent
    for (int i = res.size() - 1; i >= 0; i--){
        if (res[i] == 0){continue;}
        double value = res[i];
        string sign = "+";
        if (value < 0){
            sign = "-";
            value = abs(value);
        }
        if (i == 0){
            actExpr += " " + sign + " " + to_string(value);
        } else if (i == 1){
            actExpr += " " + sign + " " + to_string(value) + "x";
        }else if (i == res.size() - 1){
            actExpr = sign + " " + to_string(value) + "x^" + to_string(i);
        } else {
            actExpr += " " + sign + " " + to_string(value) + "x^" + to_string(i);
        }
    }
    //Remove the first space
    return actExpr.substr(1);
}
#endif
