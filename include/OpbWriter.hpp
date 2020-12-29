#include <fstream>
#include <stdio.h>

namespace PresolveOpb 
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
        const papilo::Vec<T>& rhs = consmatrix.getRightHandSides();
        // const papilo::Vec<std::string>& consnames = prob.getConstraintNames();
        // const papilo::Objective<T>& obj = prob.getObjective();
        // const papilo::Vec<papilo::ColFlags>& col_flags = prob.getColFlags();

        // Write opb header
        file << "* #variable= " << consmatrix.getNCols() << " #constraint= " << consmatrix.getNRows() << std::endl;

        // Write obj function
        const papilo::Objective<T> objective = prob.getObjective();
        bool objSet = false;
        for (int i = 0; i < objective.coefficients.size(); ++i) {
            if (objective.coefficients[i] != 0) {
                if (!objSet) {
                    objSet = true;
                    file << "min: ";
                }
                if (objective.coefficients[i] > 0) file << "+";
                file << objective.coefficients[i] << " " << varnames[i] << " ";
            }
        }
        if (objSet) file << ";" << std::endl;
        // Write constraints
        for ( int i = 0; i < consmatrix.getNRows(); ++i) {
            // If constraint is <= flip to >=
            int signBit = 1;
            if (row_flags[i].test( papilo::RowFlag::kLhsInf)){
                signBit = -1;
            }

            // Get row
            const papilo::SparseVectorView<T> row = consmatrix.getRowCoefficients(i);
            const T* rowVals = row.getValues();
            const int* indices = row.getIndices();
            const auto len = row.getLength();

            for ( int j = 0; j < len; ++j) {
                // Puts "<coeff> <varname>"
                const int val = int(rowVals[j])*signBit;
                if (val > 0) file << "+";
                file << val << " " << varnames[indices[j]] << " ";
            }

            // Write relational operator. Since all coeffs are flipped if constraint is <=, always puts '>=' if not '='.
            if (!row_flags[i].test( papilo::RowFlag::kRhsInf) && !row_flags[i].test(papilo::RowFlag::kLhsInf)) {
                file << "= "; 
            }
            else if (row_flags[i].test(papilo::RowFlag::kRhsInf) || row_flags[i].test(papilo::RowFlag::kLhsInf)) {
                file << ">= ";
            }
            else {
                throw std::invalid_argument( "Row " + std::to_string(i) + " contains invalid constraint. LhsInf: " + std::to_string(row_flags[i].test( papilo::RowFlag::kLhsInf))
                                                                                                     + " RhsInf: " + std::to_string(row_flags[i].test( papilo::RowFlag::kRhsInf)));
            } 
            // else if (row_flags[i].test( papilo::RowFlag::kEquation)) file << "= ";
            file << (((int)lhs[i])*signBit) << " ;" << std::endl;
        }
        file.close();

    }        
};
}