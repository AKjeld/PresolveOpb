#include "StdInclude.hpp"
#include "BoostInclude.hpp"
#include "PapiloInclude.hpp"
#include "utils.hpp"

namespace RoundingSatParser
{
template <typename T>
struct SolutionParser
{
    static papilo::Solution<T> parseSol(std::string solutionFilePath) {
        // Load solution file
        std::ifstream ifs(solutionFilePath.c_str());
        if (ifs.fail()) {
            ifs.close();
            throw solutionFilePath + " does not exist!";
        }

        std::string tempLine;
        std::string s;
        papilo::Vec<T> vec = {};

        while (ifs >> s) {
            if (s[0] == '-') {
                vec.push_back(0);
            }
            else if (s[0] == '+' || s[0] == 'x') {
                vec.push_back(1);
            }
            else if (s == "v") {
                continue;
            }
            else {
                std::getline(ifs, tempLine);
            }   
        }
        // utils::printVector<T>(vec);
        papilo::Solution<T> reducedSol = papilo::Solution<T>(vec);
        return reducedSol;
    }
};
}