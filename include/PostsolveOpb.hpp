#include "StdInclude.hpp" 
#include "BoostInclude.hpp"
#include "PapiloInclude.hpp"

namespace PresolveOpb 
{
template <typename T>
struct PostsolveOpb
{
    static papilo::Solution<T> postSolve(std::string postPath, papilo::Solution<T>& reducedSol, std::string reducedSolName) {
        // Load postsolve file
        std::ifstream ifs( postPath.c_str());
        boost::archive::text_iarchive oa(ifs);
        papilo::Postsolve<T> postsolveInstance;
        oa >> postsolveInstance;

        std::string origSolName = reducedSolName.substr(0, reducedSolName.find_last_of('.')) + ".orig.solution";

        std::string preFileExtension = ".pre";

        std::string::size_type i = origSolName.find(preFileExtension);
        if (i != std::string::npos) {
            origSolName.erase(i, preFileExtension.length());
        }



        std::cout << "ncols orig: " << postsolveInstance.nColsOriginal << std::endl;
        std::cout << "nrows orig: " << postsolveInstance.nRowsOriginal << std::endl;
        papilo::postsolve<T>(postsolveInstance, reducedSol, "", origSolName);
        return reducedSol;
    }
};


}