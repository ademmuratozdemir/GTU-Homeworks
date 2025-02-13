#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H
#include <vector>
#include <string>
#include <ostream>

class SparseMatrix {
private:
    // inner data for 2D representation of doubles
    std::vector<std::vector<double>> data;
    double index(int i, int j) const;
    int get_cols() const;

public:
    SparseMatrix();
    SparseMatrix(std::vector<std::vector<double>> new_data);
    SparseMatrix(const std::string& filename);

    SparseMatrix operator+(const SparseMatrix& rhs) const;
    SparseMatrix operator-(const SparseMatrix& rhs) const;
    SparseMatrix operator-();
    SparseMatrix& operator=(const SparseMatrix& rhs);
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& rhs);
    //SparseMatrix operator*(const SparseMatrix& rhs) const;
};

#endif