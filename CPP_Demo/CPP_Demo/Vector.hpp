#pragma once

class Vector {
private:
    double* elem; // elem points to an array of sz doubles
    int sz;
public:
    Vector(int s);                          // constructor: establish invariant, acquire resources

    ~Vector() { delete[] elem; }            // destructor: release resources

    Vector(Vector&& a); // move constructor
    Vector& operator=(Vector&& a); // move assignment

    Vector(const Vector& a);                // copy constructor
    Vector& operator=(const Vector& a);     // copy assignement

    double& operator[](int i);
    const double& operator[](int i) const;

    int size() const;
};