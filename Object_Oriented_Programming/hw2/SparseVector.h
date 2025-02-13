#ifndef SPARSEVECTOR_H
#define SPARSEVECTOR_H
#include <vector>
#include <string>
#include <ostream>

class SparseVector {
private:
    // inner data for 1D representation of doubles
    std::vector<double> data;

public:
    SparseVector();
    SparseVector(std::vector<double> new_data);
    SparseVector(const std::string& filename);

    SparseVector operator+(const SparseVector& rhs) const;
    SparseVector operator-(const SparseVector& rhs) const;
    SparseVector operator-();
    SparseVector& operator=(const SparseVector& rhs);
    double dot(const SparseVector& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const SparseVector& rhs);
};

#endif