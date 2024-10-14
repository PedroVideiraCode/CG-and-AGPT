#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <ctime>
#include <stdio.h>
#include <math.h>
#include <stdexcept>

#include "Vector.hpp"	// get Vector’s interface
#include <cmath>	// get the the standard-library math function interface including sqrt()

using namespace std;

int main() {



	Vector v(6);

	try {
		v[7] = 87;
	}

	catch (out_of_range)
	{
		cout << "You are dumb" << endl;
	}
}