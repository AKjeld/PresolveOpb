#include <fstream>
#include <iterator>
#include <string>
#include <stdio.h>
#include <papilo/core/ProblemBuilder.hpp>


namespace RoundingSatPresolver
{
template <typename T>
struct OpbParser
{
    static papilo::ProblemBuilder<T> parseProbOpb(std::string filename) {
        std::ifstream file (filename.c_str(), std::ifstream::in);
        if (file.fail()){
            file.close();
            throw filename + " does not exist!";
        }
        std::cout << "Parsing ...";
        // Create Problem builder
        papilo::ProblemBuilder<T> probBuilder;
        probBuilder.setProblemName(filename);

        int nCols;
        int nRows;
        // Get dimensions of problem
        for(int i = 0; i < 5; ++i) {
            std::string temp;
            file >> temp;
            if(i == 2) nCols = std::stoi(temp);
            if(i == 4) nRows = std::stoi(temp);
        }
        std::cout << "  Parser found: " << nCols << " vars" << std::endl;
        std::cout << "  Parser found: " << nRows << " constraints" << std::endl;

        // set dims
        probBuilder.setNumCols(nCols);
        probBuilder.setNumRows(nRows);

        // set all vars to bool
        for (int i = 0; i < probBuilder.getNumCols(); ++i ) {
            probBuilder.setColUb(i, 1);
            probBuilder.setColLb(i, 0);
            probBuilder.setColIntegral(i, true);
        }

        std::string s;
        std::string commentLineTemp;
        std::map<std::string, int> varMap;
        int coeff;
        int row = 0;
        int newcol = 0;

        while (file >> s) {        
            // Handle coefficient case
            if (s[0] == '+' || s[0] == '-') coeff = std::stoi(s);
            // Handle bound
            else if (s == ">=") {
                probBuilder.setRowRhsInf(row, true);
                file >> s;
                probBuilder.setRowLhs(row, (T)std::stoi(s));
            }
            else if (s == "=") {
                file >> s;
                probBuilder.setRowLhs(row, (T)std::stoi(s));
                probBuilder.setRowRhs(row, (T)std::stoi(s));
            }
            // Handle var
            else if (s[0] == 'x') {
                auto addr = varMap.find(s);
                if (addr == varMap.end()) {
                    varMap[s] = newcol++;
                    addr = varMap.find(s);
                    probBuilder.setColName(addr->second, s);
                }
                int col = addr->second;
                probBuilder.addEntry(row, col, (T)coeff);
            }
            else if (s == ";") ++row;
            else if (s == "*") std::getline(file, commentLineTemp);
        }

        file.close();
        std::cout << "Done!" << std::endl;
        return probBuilder;
    }
};
}