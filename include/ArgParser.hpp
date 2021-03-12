#include "StdInclude.hpp"
#include "BoostInclude.hpp"

namespace po = boost::program_options;

namespace PresolveOpb
{
struct ArgParser
{
    static po::variables_map parseArgs(int argc, char* argv[]) {
        po::options_description desc("All options");
        desc.add_options()
            ("help", "Display all options")
            ("filein,I", po::value< std::string>(), "Path to file in (.opb)")
            ("fileout,O", po::value< std::string>(), "Path to file out (default <file-name>.pre.opb")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help") || !vm.count("filein")) {
            std::cout << desc << std::endl;
        }
        return vm;
    }
};
}