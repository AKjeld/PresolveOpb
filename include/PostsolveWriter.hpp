#include "StdInclude.hpp"
#include "PapiloInclude.hpp"
#include "BoostInclude.hpp"
   
namespace PresolveOpb
{
template <typename T>
struct PostsolveWriter
{
    static void writePostsolve(papilo::PresolveResult<T> result, std::string filename) {
        std::ofstream ofs( filename.c_str() );
        boost::archive::text_oarchive oa( ofs );

        oa << result.postsolve;
    }

};
}
