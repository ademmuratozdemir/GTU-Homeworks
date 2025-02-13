#include <iostream>
#include "SparseVector.h"
#include "SparseMatrix.h"

int main()
{
    /*
    SparseVector vec(std::vector<double>{0, 0, 5, 9, 11, 0, 2, 0, 0, 3});
    std::cout << vec << '\n';

    SparseVector vec1("asd");
    std::cout << vec1 << '\n';

    std::cout << vec + vec1 << '\n';

    SparseVector subs(std::vector<double>{0, 1, 2});
    subs = vec + vec + vec + vec - vec1;
    std::cout << subs << '\n';
    std::cout << -subs << '\n';
    */

    SparseMatrix mat(std::vector<std::vector<double>>{{}, {}, {0, 0, 5, 9, 11, 0, 2, 0, 0, 3}, {}, {}, {1}});
    std::cout << mat << '\n';
    std::cout << mat + mat << '\n';
    std::cout << -mat << '\n';
    std::cout << mat * mat << '\n';

    SparseMatrix mat1(std::vector<std::vector<double>>({{1, 2, 3}, {4, 5, 6}}));
    SparseMatrix mat2(std::vector<std::vector<double>>({{7, 8}, {9, 10}, {11, 12}}));
    std::cout << mat1 * mat2 << '\n';

}