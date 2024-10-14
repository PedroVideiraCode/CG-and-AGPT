#include "Vector.hpp"
#include <stdexcept>

using namespace std;

Vector::Vector(int s) {
    if (s < 0) throw length_error{"Invalid Size!"};
    elem = new double[s];
    sz = s;
    memset(elem, 0, sz*sizeof(double));
}

Vector::~Vector()
{
    delete[] elem;
}

double& Vector::operator[](int i)
{
    if (i < 0 || size() <= i) 
        throw out_of_range{ "Vector::operator[]" };

    return elem[i];
}

int Vector::size() const {
    return sz;
}