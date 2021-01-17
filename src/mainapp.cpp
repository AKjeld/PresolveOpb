#include "utils.hpp"
#include "OpbWriter.hpp"
#include "OpbParser.hpp"
#include "ParamParser.hpp"
#include "StdInclude.hpp"
#include "PapiloInclude.hpp"

int main(int argc, char *argv[]){
   std::cout << argc << std::endl;
   
   if (argc < 3) {
      throw std::invalid_argument("Usage: ./PresolveOpb <filename_in> <filename_out>");
   }
   const auto filenamein = argv[1];
   const auto filenameout = argv[2];
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
      PresolveOpb::OpbWriter<double>::writeProbOpb(prob, filenameout);
   }

   std::cout << "Status code of test problem: " << utils::as_integer(result.status) << std::endl;

   return 0;
}
