#include <iostream>
#include <ostream>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include <random>
using namespace std;

int main(){
    ofstream outputFile;
    outputFile.open("test_input_expression");
    //the structure of each function must be:
    int numb = 140;
    int n = 100;
    for (int p = 0; p < 1; p++){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(1, 10);
        vector<double> vectorCoefficients;
        for (int i = 0; i <= numb; i++){
            //random value between 100 150
            double coefficients = dis(gen);
            //Set the pre cision to 0 decimal places
            coefficients = (int)coefficients;
            vectorCoefficients.push_back(coefficients);
        }
        vector<string> sign;
        for (int i = 0; i <= numb; i++){
            if (rand() % 2 == 0){
                sign.push_back(" - ");
            } else {
                sign.push_back(" + ");
            }
        }
    //create a set of functions starting with 20 coefficients and then decrease the number of coefficients, which the first coefficient is always 1
        string function = "";
        n = 20;
        for (int i = numb; i >= 0; i--){
            string sign2 = sign[i];
            if (i == 0){
                function = function + sign2 + to_string(vectorCoefficients[i]);
            } else if (i == 1){
                function = function + sign2 + to_string(vectorCoefficients[i]) + "x";
            } else {
                function = function + sign2 + to_string(vectorCoefficients[i]) + "x^" + to_string(i);
            }
            n--;
        }
        function = function.substr(1);
        for (int i = 10; i > 0; i--){
             outputFile << "\""<< function <<"\"" << " " << "\"" << "Lagrange" << "\"" << " " << "\"" << to_string(1) << "\"" << " " << "\"" << to_string(i) <<"\"" << endl;
            outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(2) << "\"" << " " << "\"" << to_string(i) <<"\"" << endl;
            outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(3) << "\"" << " " << "\"" << to_string(i) <<"\"" << endl;
            outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(4) << "\"" << " " << "\"" << to_string(i) <<"\"" << endl;   
        }
    }
    
    /* while(n > 0){
        string function = "";
        for (int i = numb; i >= 0; i--){
            //the coefficients is a number between 0 and 100
            //the sign of the coefficient is randomly chosen
            double coefficients = vectorCoefficients[i];
            if (i == 0){
                function = function + sign[i] + to_string(coefficients);
            } else if (i == 1){
                function = function + sign[i] + to_string(coefficients) + "x";
            } else {
                if (i == numb){
                    coefficients = coefficients / 10;
                }
                function = function + sign[i] + to_string(coefficients) + "x^" + to_string(i);
            }
        }
        function = function.substr(1);
        outputFile << "\""<< function <<"\"" << " " << "\"" << "Lagrange" << "\"" << " " << "\"" << to_string(1) << "\"" << " " << "\"" << to_string(n) <<"\"" << endl;
        outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(2) << "\"" << " " << "\"" << to_string(n) <<"\"" << endl;
        outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(3) << "\"" << " " << "\"" << to_string(n) <<"\"" << endl;
        outputFile << "\""<< function <<"\"" << " " << "\"" << "Taylor" << "\"" << " " << "\"" << to_string(4) << "\"" << " " << "\"" << to_string(n) <<"\"" << endl;
        n--;
    } */
    outputFile.close();
}