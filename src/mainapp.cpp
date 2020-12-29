#include "utils.hpp"
#include "OpbWriter.hpp"
#include "OpbParser.hpp"
#include <iostream>
#include <papilo/misc/MultiPrecision.hpp>
#include <papilo/core/Problem.hpp>
#include <papilo/core/ProblemBuilder.hpp>
#include <papilo/core/Presolve.hpp>

int main(int argc, char *argv[]){
   if (argc < 3) {
      throw "Usage: ./PresolveOpb <filename_in> <filename_out>";
   }
   const auto filenamein = argv[1];
   const auto filenameout = argv[2];
   std::cout << "Filename in given: " << filenamein << " Filename out given: " << filenameout << std::endl;
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
   // paramset.setParameter("sparsify.enabled", false);
   // paramset.setParameter("substitution.enabled", false);
   paramset.setParameter("parallelcols.enabled", false);
   paramset.setParameter("parallelrows.enabled", false);
   // paramset.setParameter("sparsify.maxscale", 1.0);

   papilo::PresolveResult<double> result = testPresolver.apply(testProblem);

   // Validation of problem format
   auto varlbounds = testProblem.getLowerBounds();
   for(double b : varlbounds) {
      if ((int)b != 0) throw std::invalid_argument("NOT BOOL! lower bound: " + std::to_string(b));
   }

   auto varubounds = testProblem.getUpperBounds();
   for(double b : varubounds) {
      if ((int)b != 1) throw std::invalid_argument("NOT BOOL! upper bound: " + std::to_string(b));
   }

   for(papilo::String name : testProblem.getVariableNames()) {
      if(name[0] != 'x') throw std::invalid_argument("Improper naming of var " + name);
   }

   // auto consmatrix = testProblem.getConstraintMatrix();
   // for(papilo::RowFlags row_flags : consmatrix.getRowFlags()) {
   //    if(row_flags.test(papilo::RowFlag::kLhsInf)) {
   //       throw std::invalid_argument("Some row has kLhsInf set. LhsInf: 1 RhsInf: " + std::to_string(row_flags.test(papilo::RowFlag::kRhsInf)));
   //    }
   // }

   //Write problem to file out
   RoundingSatPresolver::OpbWriter<double>::writeProbOpb(testProblem, filenameout);

   std::cout << "Status code of test problem: " << utils::as_integer(result.status) << std::endl;
   return 0;
}
