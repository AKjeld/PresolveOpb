#include "utils.hpp"
#include "OpbWriter.hpp"
#include "OpbParser.hpp"
#include "ParamParser.hpp"
#include "PostsolveOpb.hpp"
#include "RoundingSatSolutionParser.hpp"
#include "PostsolveWriter.hpp"
#include "StdInclude.hpp"
#include "PapiloInclude.hpp"
#include "BoostInclude.hpp"
#include "ArgParser.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]){
   // Parse command line
   po::variables_map optsvm = PresolveOpb::ArgParser::parseArgs(argc, argv);

   if(optsvm.count("help") || !optsvm.count("filein")) {
      return 1;
   }
   const std::string filenamein = optsvm["filein"].as<std::string>();

   if(optsvm.count("postsolve")) {
      //postsolve

      // Find postsolve path
      std::string postsolvePath;
      if(optsvm.count("postfile")) postsolvePath = optsvm["postfile"].as<std::string>();
      else postsolvePath = filenamein.substr(0, filenamein.find_last_of('.')) + ".postsolve";

      std::cout << "Looking for postsolve file at: " << postsolvePath << std::endl;
      std::cout << "Looking for solution file at:  " << filenamein << std::endl;

      // Parse solution (specific roundingsat format)
      papilo::Solution<double> reducedSol = RoundingSatParser::SolutionParser<double>::parseSol(filenamein);

      // Execute postsolve
      PresolveOpb::PostsolveOpb<double>::postSolve(postsolvePath, reducedSol, filenamein);

      return 0;
   }


   std::string filenameout = filenamein.substr(0, filenamein.find_last_of('.'))+".pre.opb";
   if (optsvm.count("fileout")) {
      filenameout = optsvm["fileout"].as<std::string>();
   }
   const std::string filenamepost = filenameout.substr(0, filenameout.find_last_of('.')) + ".postsolve";

   std::cout << "Filename in given: " << filenamein << " Filename out given: " << filenameout << std::endl;
   papilo::ProblemBuilder<double> probBuilder;

   probBuilder = PresolveOpb::OpbParser<double>::parseProbOpb(filenamein);

   auto prob = probBuilder.build();

   papilo::Presolve<double> presolver;
   presolver.addDefaultPresolvers();
   
   // Get params
   papilo::ParameterSet paramset = presolver.getParameters();

   // Read parameters.opb.txt and apply
   std::map <std::string, std::string> params = PresolveOpb::ParamParser::ParseParamFile("../parameters.opb.txt");
   std::map <std::string, std::string>::iterator itr;

   for (itr = params.begin(); itr != params.end(); ++itr) { 
      paramset.parseParameter((itr->first).c_str(), (itr->second).c_str());
   }

   papilo::PresolveResult<double> result = presolver.apply(prob);


   if (utils::as_integer(result.status) <= 2) {
      // Validation of problem format
      auto varlbounds = prob.getLowerBounds();
      for(double b : varlbounds) {
         if ((int)b != 0) throw std::invalid_argument("NOT BOOL! lower bound: " + std::to_string(b));
      }

      auto varubounds = prob.getUpperBounds();
      for(double b : varubounds) {
         if ((int)b != 1) throw std::invalid_argument("NOT BOOL! upper bound: " + std::to_string(b));
      }

      for(papilo::String name : prob.getVariableNames()) {
         if(name[0] != 'x') throw std::invalid_argument("Incorrect naming of var " + name);
      }

      // auto consmatrix = testProblem.getConstraintMatrix();
      // for(papilo::RowFlags row_flags : consmatrix.getRowFlags()) {
      //    if(row_flags.test(papilo::RowFlag::kLhsInf)) {
      //       throw std::invalid_argument("Some row has kLhsInf set. LhsInf: 1 RhsInf: " + std::to_string(row_flags.test(papilo::RowFlag::kRhsInf)));
      //    }
      // }

      //Write problem to file out
      PresolveOpb::PostsolveWriter<double>::writePostsolve(result, filenamepost);

      //Write postsolve instance
      PresolveOpb::OpbWriter<double>::writeProbOpb(prob, filenameout);
   }

   std::cout << "Status code of test problem: " << utils::as_integer(result.status) << std::endl;

   return 0;
}
