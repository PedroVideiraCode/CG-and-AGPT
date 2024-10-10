#include "Vector.hpp"

Vector::Vector(int s)		// definition of the constructor
    :elem{ new double[s] }, sz{ s } 	// initialize members
{
}

double& Vector::operator[](int i) {
    return elem[i];		// definition of subscripting
}

int Vector::size() const {
    return sz;
}