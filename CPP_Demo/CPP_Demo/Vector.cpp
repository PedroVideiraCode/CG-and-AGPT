#include "Vector.hpp"
#include <stdexcept>
#include "Container.hpp"

using namespace std;

Vector::Vector(int s) {
    if (s < 0) throw length_error{"Invalid Size!"};
    elem = new double[s];
    sz = s;
    memset(elem, 0, sz*sizeof(double));
}

Vector::Vector(const Vector& a) // copy constructor
    :elem{ new double[a.sz] }, // allocate space for elements
    sz{ a.sz }
{
    for (int i = 0; i != sz; ++i) // copy elements
        elem[i] = a.elem[i];
}

Vector::Vector(Vector&& a)
    :elem{ a.elem }, // "grab the elements" from a
    sz{ a.sz }
{
    a.elem = nullptr; // now a has no elements
    a.sz = 0;
}

Vector& Vector::operator=(const Vector& a) // copy assignment
{
    double* p = new double[a.sz];
    for (int i = 0; i != a.sz; ++i)
        p[i] = a.elem[i];
    delete[] elem;      // delete old elements
    elem = p;
    sz = a.sz;
    return *this;
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