#include "StdInclude.hpp" 
#include "BoostInclude.hpp"
#include "PapiloInclude.hpp"

namespace PresolveOpb 
{
template <typename T>
struct PostsolveOpb
{
    static papilo::Solution<T> postSolve(std::string postPath, papilo::Solution<T>& reduced_sol) {
        // Load postsolve file
        std::ifstream ifs( postPath.c_str());
        boost::archive::text_iarchive oa(ifs);
        papilo::Postsolve<T> postsolveInstance;
        oa >> postsolveInstance;

        std::cout << "ncols orig: " << postsolveInstance.nColsOriginal << std::endl;
        std::cout << "nrows orig: " << postsolveInstance.nRowsOriginal << std::endl;
        papilo::postsolve<T>(postsolveInstance, reduced_sol);
        return reduced_sol;
    }
};


}