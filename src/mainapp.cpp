#include "utils.hpp"
#include "OpbWriter.hpp"
#include <iostream>
#include <papilo/misc/MultiPrecision.hpp>
#include <papilo/core/Problem.hpp>
#include <papilo/core/ProblemBuilder.hpp>
#include <papilo/core/Presolve.hpp>

int main(int argc, char *argv[]){
   
   papilo::Quad test = 12323;
   papilo::ProblemBuilder<double> testBuilder;


   testBuilder.setNumCols(3);
   testBuilder.setNumRows(2);

   testBuilder.setColUb(0, 1);
   testBuilder.setColUb(1, 1);
   testBuilder.setColUb(2, 1);
   testBuilder.setColLb(0, 0);
   testBuilder.setColLb(1, 0);
   testBuilder.setColLb(2, 0);

   testBuilder.setColIntegral(0, true);
   testBuilder.setColIntegral(1, true);
   testBuilder.setColIntegral(2, true);

   testBuilder.setColName(0, "x1");
   testBuilder.setColName(1, "x2");
   testBuilder.setColName(2, "x3");


   testBuilder.setRowLhs(0, 2);
   testBuilder.setRowLhs(1, 2);

   testBuilder.setRowRhsInf(0, true);
   testBuilder.setRowRhsInf(1, true);

   // testBuilder.addColEntries()

   testBuilder.addEntry(0,0,1);
   testBuilder.addEntry(0,1,1);
   testBuilder.addEntry(1,1,-2);
   testBuilder.addEntry(1,2,1);

   testBuilder.setProblemName("Test Problem Unsat");

   auto testProblem = testBuilder.build();

   RoundingSatPresolver::OpbWriter<double>::writeProbOpb(testProblem, "test.txt");

   // utils::print_problem(testProblem);

   papilo::Presolve<double> testPresolver;
   testPresolver.addDefaultPresolvers();
   
   // Ensure no vars are changed to be non binary
   papilo::ParameterSet paramset = testPresolver.getParameters();
   paramset.setParameter("substitution.binarieswithints", false);

   papilo::PresolveResult<double> result = testPresolver.apply(testProblem);

   RoundingSatPresolver::OpbWriter<double>::writeProbOpb(testProblem, "test2.txt");

   std::cout << "Status code of test problem: " << utils::as_integer(result.status) << std::endl;
   return 0;
}
