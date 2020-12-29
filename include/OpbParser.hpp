#include <fstream>
#include <iterator>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <papilo/core/ProblemBuilder.hpp>


namespace PresolveOpb
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
        std::cout << "Parsing ..." << std::endl;
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

        std::string commentLineTemp;
        bool loadingObj = false;
        int coeff;
        int newcol = 0;
        
        // Mapping vars to col index
        std::map<std::string, int> varMap;
        std::string s;

        // Get objective function if one is present:
        while (file >> s) {
            if (s == "*" || s[0] == '*') std::getline(file, commentLineTemp);
            // Load objective function
            else if (s == "min:") loadingObj = true;
            else if (s == ";") {
                loadingObj = false;
                break;
            }
            else if (loadingObj) {
                if (s[0] == '-' || s[0] == '+' || isdigit(s[0])) coeff = std::stoi(s);
                else {
                    auto addr = varMap.find(s);
                    if (addr == varMap.end()) {
                        varMap[s] = newcol++;
                        addr = varMap.find(s);
                        probBuilder.setColName(addr->second, s);
                    }
                    int col = addr->second;
                    probBuilder.setObj(col, (T)coeff);
                }
            }
            // else {
            //     file.close();
            //     file.open(filename.c_str(), std::ifstream::in);
            //     break;
            // }
        }

        int row = 0;

        while (file >> s) {        
            // Handle coefficient case
            if (s[0] == '+' || s[0] == '-' || isdigit(s[0])) coeff = std::stoi(s);
            // Handle bound
            else if (s == ">=") {
                probBuilder.setRowRhsInf(row, true);
                probBuilder.setRowLhsInf(row, false);
                file >> s;
                probBuilder.setRowLhs(row, (T)std::stoi(s));
            }
            // Note: Case not in normal opb files
            else if (s == "<=") {
                probBuilder.setRowRhsInf(row, false);
                probBuilder.setRowLhsInf(row, true);
                file >> s;
                probBuilder.setRowLhs(row, (T)std::stoi(s));
            }
            else if (s == "=") {
                probBuilder.setRowRhsInf(row, false);
                probBuilder.setRowLhsInf(row, false);
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
            // If comment or objective function push iterator past line
            else if (s == "*" || s[0] == '*' || s[0] == 'm') std::getline(file, commentLineTemp);
        }

        file.close();
        std::cout << "Done!" << std::endl;
        return probBuilder;
    }
};
}