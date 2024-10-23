#ifndef _CONTAINER_H_
#define _CONTAINER_H_

class container {
public:
    virtual double& operator[](int) = 0; // pure virtual function
    virtual int size() const = 0;        // const member function
    virtual ~container() {}              // destructor

};

#endif