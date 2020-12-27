#include "utils.hpp"
#include "OpbWriter.hpp"
#include "OpbParser.hpp"
#include <iostream>
#include <papilo/misc/MultiPrecision.hpp>
#include <papilo/core/Problem.hpp>
#include <papilo/core/ProblemBuilder.hpp>
#include <papilo/core/Presolve.hpp>

int main(int argc, char *argv[]){
   if (argc < 2) {
      throw "no filename given. Usage: ./RoundingSatWithPreprocessor <filename>";
   }
   const auto filenamein = argv[1];
   const auto filenameout = argv[2];
   std::cout << "Filename given: " << filenamein << std::endl;
   papilo::ProblemBuilder<double> testBuilder;

   testBuilder = RoundingSatPresolver::OpbParser<double>::parseProbOpb(filenamein);

   auto testProblem = testBuilder.build();

   // RoundingSatPresolver::OpbWriter<double>::writeProbOpb(testProblem, "test.txt");

   // utils::print_problem(testProblem);

   papilo::Presolve<double> testPresolver;
   testPresolver.addDefaultPresolvers();
   
   // Ensure no vars are changed to be non binary
   papilo::ParameterSet paramset = testPresolver.getParameters();
   paramset.setParameter("substitution.binarieswithints", false);

   papilo::PresolveResult<double> result = testPresolver.apply(testProblem);
   auto varlbounds = testProblem.getLowerBounds();
   for(double b : varlbounds) {
      if ((int)b != 0) std::cout << "NOT BOOL! lower bound: " << b << std::endl;
   }

   auto varubounds = testProblem.getUpperBounds();
   for(double b : varubounds) {
      if ((int)b != 1) std::cout << "NOT BOOL! upper bound: " << b << std::endl;
   }



   RoundingSatPresolver::OpbWriter<double>::writeProbOpb(testProblem, filenameout);

   std::cout << "Status code of test problem: " << utils::as_integer(result.status) << std::endl;
   return 0;
}
