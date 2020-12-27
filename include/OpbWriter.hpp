#include <fstream>
#include <stdio.h>

namespace RoundingSatPresolver 
{

template <typename T>
struct OpbWriter
{
    static void writeProbOpb(papilo::Problem<T> prob, std::string filename) {
        std::fstream file;
        file.open(filename.c_str());
        if(!file.fail()) {
            file.close();
            remove(filename.c_str());
        }
        file.close();
        file.open(filename.c_str(), std::fstream::out);

        const papilo::ConstraintMatrix<T>& consmatrix = prob.getConstraintMatrix();
        const papilo::Vec<std::string>& varnames = prob.getVariableNames();
        const papilo::Vec<T>& lhs = consmatrix.getLeftHandSides();
        const papilo::Vec<papilo::RowFlags>& row_flags = prob.getRowFlags();
        // const papilo::Vec<T>& rhs = consmatrix.getRightHandSides();
        // const papilo::Vec<std::string>& consnames = prob.getConstraintNames();
        // const papilo::Objective<T>& obj = prob.getObjective();
        // const papilo::Vec<papilo::ColFlags>& col_flags = prob.getColFlags();

        // Write opb header
        file << "* #variable= " << consmatrix.getNCols() << " #constraint= " << consmatrix.getNRows() << std::endl;

        // Write constraints
        for ( int i = 0; i < consmatrix.getNRows(); ++i) {
            const papilo::SparseVectorView<T> row = consmatrix.getRowCoefficients(i);
            const T* rowVals = row.getValues();
            const int* indices = row.getIndices();
            const auto len = row.getLength();

            for ( int j = 0; j < len; ++j) {
                const int val = int(rowVals[j]);
                if (val > 0) {
                    file << "+";
                }
                file << val << " " << varnames[indices[j]] << " ";
            }

            // Write relational operator
            if (row_flags[i].test( papilo::RowFlag::kRhsInf)) file << ">= ";
            else file << "= "; 
            // else if (row_flags[i].test( papilo::RowFlag::kEquation)) file << "= ";
            // else throw std::invalid_argument( "Row " + std::to_string(i) + " contains invalid constraint");
            file << (int)lhs[i] << " ;" << std::endl;
        }
        file.close();

    }        
};
}