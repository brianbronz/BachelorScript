//Copyright(C) 2022-2023 by Brian Bronz
#include <iostream>
#include <ostream>
#include <cassert>
#include <string>
#include <vector>
#include <fstream>
#include "main.h"
#include "root_finding.c++"
using namespace std;
int main(){
    //read from file
    string inputLine;
    ifstream file;
    file.open("test_input_expression");
    ofstream outputFile;
    outputFile.open("roots.txt");
    //open the file to write the average time taken
    ofstream timeTaken;
    timeTaken.open("AverageTime.txt");
    ofstream myfile;
    myfile.open("iterations.txt");
    while(getline(file, inputLine)){
        outputFile << "expression: ";
        //the structure of each function must be:
        // "function" "interpolation form" "k" "n"
        //Get the expression
        int idxExpression = inputLine.find("\"", 1);
        string expression = inputLine.substr(1, idxExpression - 1);
        outputFile << expression << endl;
        //Get the interpolation form
        int idxInterpolationForm = inputLine.find("\"", idxExpression + 3);
        string interpolationForm = inputLine.substr(idxExpression + 3, idxInterpolationForm - idxExpression - 3);
        //Get the order of convergence
        int idxK = inputLine.find(" ", idxInterpolationForm + 3);
        int k = stoi(inputLine.substr(idxInterpolationForm + 3, idxK - idxInterpolationForm - 3));
        //Get the level of convergence
        int idxN = inputLine.find("\"", idxK + 3);
        int n = stoi(inputLine.substr(idxK + 2, idxN - idxK - 2));
        //Get the roots using the interpolation form
        if (interpolationForm == "Taylor"){
            FindingRoots(expression, "Taylor", k, n, outputFile, timeTaken, myfile);
        }
        else if (interpolationForm == "Lagrange"){
            timeTaken << endl;
            myfile << endl;
            FindingRoots(expression, "Lagrange", k, n, outputFile, timeTaken, myfile);
        } else {
            printf("Error: Taylor or Lagrange form not specified\n");
            break;
        }
        outputFile << "The order of convergence is: " << k << " " << "the level is: " << n << endl;
        outputFile << endl;
        //timeTaken << endl;
    }
    myfile.close();
    file.close();
    timeTaken.close();
}