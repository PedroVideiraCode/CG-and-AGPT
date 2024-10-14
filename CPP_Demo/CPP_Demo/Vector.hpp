#pragma once

class Vector {
public:
    Vector(int s);
    ~Vector();

    double& operator[](int i);
    int size() const;
private:
    double* elem;  // elem points to an array of sz doubles
    int sz;
};