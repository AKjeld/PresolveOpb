#pragma once
#include <iostream>
#include <papilo/core/Problem.hpp>

#include <papilo/core/ConstraintMatrix.hpp>

namespace utils {

    template <typename Enumeration>
    auto as_integer(Enumeration const value)
        -> typename std::underlying_type<Enumeration>::type
    {
        return static_cast<typename std::underlying_type<Enumeration>::type>(value);
    }

    template <typename T>
    void print_problem(papilo::Problem<T> prob) {
        const papilo::ConstraintMatrix<T>& consmatrix = prob.getConstraintMatrix();
        const papilo::Vec<std::string>& consnames = prob.getConstraintNames();
        const papilo::Vec<std::string>& varnames = prob.getVariableNames();
        const papilo::Vec<T>& lhs = consmatrix.getLeftHandSides();
        const papilo::Vec<T>& rhs = consmatrix.getRightHandSides();
        const papilo::Objective<T>& obj = prob.getObjective();
        const papilo::Vec<papilo::ColFlags>& col_flags = prob.getColFlags();
        const papilo::Vec<papilo::RowFlags>& row_flags = prob.getRowFlags();

        std::cout << "Print Problem: " << prob.getName() << std::endl;

        std::cout << "n vars: " << consmatrix.getNCols() << std::endl;
        std::cout << "m constraints: " << consmatrix.getNRows() << std::endl;
        std::cout << "Variable Names: " << std::endl;
        
        std::cout << "  ";
        for(papilo::String varname : varnames) {
            std::cout << varname << ", ";
        }
        std::cout << std::endl;

        const auto cols = *consmatrix.getColumns();

        const auto getRowZeroCoeff = consmatrix.getRowCoefficients(0);
        const auto len = getRowZeroCoeff.getLength();

        const T* rowZeroCoeffVal = getRowZeroCoeff.getValues();
        for(int i = 0; i < len; ++i) {
            const auto test = int(rowZeroCoeffVal[i]);
        }

        


        std::cout << "Constraint matrix:" << std::endl;
        for ( int i = 0; i < consmatrix.getNRows(); ++i) {
            const papilo::SparseVectorView<T> row = consmatrix.getRowCoefficients(i);
            const T* rowVals = row.getValues();
            const int* indices = row.getIndices();
            const auto len = row.getLength();

            for ( int j = 0; j < len; ++j) {
                std::cout << std::setw(7) << varnames[indices[j]] << ":" << int(rowVals[j]);
            }
            std::cout << std::endl;
        }

    }

}