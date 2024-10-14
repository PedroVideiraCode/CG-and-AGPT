#include <iostream>
#include <string>
#include <cstdlib>
#include <random>
#include <ctime>
#include <stdio.h>
#include <math.h>

#include "Vector.hpp"	// get Vector’s interface
#include <cmath>	// get the the standard-library math function interface including sqrt()

using namespace std;

//int main() {
//
//								bool		// Boolean, possible values are true or false
//								char		// character, for example, 'a', 'z' and '9'
//								int 		// integer, for example, -273, 42 and 1066
//								double		// double-precision floating-point number, for example, -273.15, 3.14, and 299793.0
//								unsigned	// non-negative integer, for example, 0, 1, and 999
//
//							using std::cout;
//							cout << "The size of a bool is:\t\t" << sizeof(bool) << " bytes.\n";
//							cout << "The size of a char is:\t\t" << sizeof(char) << " bytes.\n";
//							cout << "The size of a short int is:\t" << sizeof(short) << " bytes.\n";
//							cout << "The size of an int is:\t\t" << sizeof(int) << " bytes.\n";
//							cout << "The size of a long int is:\t" << sizeof(long) << " bytes.\n";
//							cout << "The size of a float is:\t\t" << sizeof(float) << " bytes.\n";
//							cout << "The size of a double is:\t" << sizeof(double) << " bytes.\n";
//							return 0;
//
//								x + y     // plus
//								+ x      // unary plus
//								x−y     // minus
//								- x      // unary minus
//								x∗y     // multiply
//								x / y     // divide
//								x % y     // remainder (modulus) for integers
//								x == y    // equal
//								x != y    // not equal
//								x<y     // less than
//								x>y     // greater than
//								x <= y    // less than or equal
//								x >= y    // greater than or equal
//								x& y     // bitwise and
//								x | y     // bitwise or
//								x ^ y     // bitwise exclusive or
//								~x      // bitwise complement
//								x && y    // logical and
//								x || y    // logical or
//								x += y;  // x = x + y
//								++x;   // increment: x = x + 1
//								x -= y;  // x = x - y
//								--x;   // decrement: x = x - 1
//								x *= y;  // scaling: x = x * y
//								x /= y;  // scaling: x = x / y
//								x %= y;  // x = x % y
//
//
//	 1 --------------------------------------------
//	using std::cout;
//	cout << "What is your name:";
//	string name1;
//	string name2;
//	cin >> name1 >> name2;
//	cout << "Hello " << name1<< " " << name2 << "!" "\n";
//
//	 2 --------------------------------------------
//	using std::cout;
//	string response;
//	int number1;
//	int number2;
//	cout << "First Number:";
//	cin >> response;
//	number1 = std::stoi(response);
//	cout << "Second Number:";
//	cin >> response;
//	number2 = std::stoi(response);
//	float multiplication = number1 * number2;
//	float division = number1 / number2;
//	int sum = number1 + number2;
//	int sub = number1 - number2;
//	int remainder = number1 % number2;
//	cout << "Multiplication:" << multiplication << "\nDivision:" << division << "\nSum:" << sum << "\nSubtract:" << sub << "\nRemainer:" << remainder;
//
//	 3 --------------------------------------------
//	using std::cout;
//	string response;
//	float r;
//	float angle;
//	cout << "Convert polar coordinates (r, θ) into cartesian coordinates (x, y)\n";
//	cout << "r:";
//	cin >> response;
//	r = std::stoi(response);
//	cout << "Angle:";
//	cin >> response;
//	angle = std::stoi(response);
//	float x = r * cos(angle);
//	float y = r * sin(angle);
//	cout << "X is equal:" << x << "\nY is equal:" << y;
//
//	 4 --------------------------------------------
//	using std::cout;
//	string response;
//	int number1;
//	int number2;
//	cout << "Prime numbers between a given interval\n";
//	cout << "First Number:";
//	cin >> response;
//	number1 = std::stoi(response);
//	cout << "Second Number:";
//	cin >> response;
//	number2 = std::stoi(response);
//	for (number1; number1 <= number2; number1++)
//	{
//		int divCount = 0;
//		for (int i = 1; i <= 10; i++)
//		{
//			if(number1 % i == 0)
//			{
//				++divCount;
//			}
//		}
//		if (divCount < 2)
//		{
//			cout << "\nPrime number:" << number1;
//		}
//	}
//
//	 5 --------------------------------------------
//	using std::cout;
//	string response;
//	int number1;
//	cout << "Do a Square\n";
//	cout << "How Large:";
//	cin >> response;
//	number1 = std::stoi(response);
//	for (size_t i = 0; i < number1; i++)
//	{
//		for (size_t i = 0; i < number1; i++)
//		{
//			if (i+1 == number1)
//			{
//				cout << "*\n";
//			}
//			else
//			{
//				cout << "* ";
//			}
//		}
//	}
//
//	 6 --------------------------------------------
//	using std::cout;
//	string response;
//	int number1;
//	cout << "Do a Square\n";
//	cout << "How Large:";
//	cin >> response;
//	number1 = std::stoi(response);
//	for (size_t i = 0; i < number1; i++)
//	{
//		if (i == 0 || i + 1 == number1)
//		{
//			for (size_t i = 0; i < number1; i++)
//			{
//				if (i + 1 == number1)
//				{
//					cout << "*\n";
//				}
//				else
//				{
//					cout << "* ";
//				}
//			}
//		}
//		else
//		{
//			for (size_t i = 0; i < number1; i++)
//			{
//				if(i == 0)
//				{
//					cout << "* ";
//				}
//				else
//				{
//					if (i + 1 == number1)
//					{
//						cout << "*\n";
//					}
//					else
//					{
//						cout << "  ";
//					}
//				}
//			}
//		}
//	}
//
//	 7 --------------------------------------------
//	using std::cout;
//	string response;
//	int number1;
//	cout << "Game Lotery\n";
//	cout << "How many games:";
//	cin >> response;
//	number1 = std::stoi(response);
//
//	std::random_device rd;
//	std::default_random_engine eng(rd());
//	std::uniform_int_distribution<int> dist(0, 2);  // Random numbers between 1 and 100
//
//	for (size_t i = 0; i < number1; i++)
//	{
//		if (dist(eng) == 0)
//		{
//			cout << "Game " << (i + 1) << ": Result: |1  |\n";
//		}
//		else
//		{
//			if (dist(eng) == 1)
//			{
//				cout << "Game " << (i + 1) << ": Result: | X |\n";
//			}
//			else
//			{
//				cout << "Game " << (i + 1) << ": Result: |  2|\n";
//			}
//		}
//	}
//
//	8 --------------------------------------------
//
//	using std::cout;
//	using std::cin;
//	using std::string;
//	string response;
//	int day, year, month, monthCount;
//	day = 1;
//
//
//	cout << "Year:";
//	cin >> response;
//	year = std::stoi(response);
//
//	cout << "Month:";
//	cin >> response;
//	month = std::stoi(response) - 1;
//
//	if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11)
//	{
//		monthCount = 31;
//	}
//	else
//	{
//		if (month == 3 || month == 5 || month == 8 || month == 10 )
//		{
//			monthCount = 30;
//		}
//		else
//		{
//			monthCount = 28;
//		}
//	}
//	// Inicialize a estrutura `std::tm` com zeros
//	std::tm date = {};
//
//	// Ajustar para os valores corretos
//	date.tm_mday = day;
//	date.tm_mon = month;   // `tm_mon` deve ser de 0 a 11
//	date.tm_year = year - 1900; // `tm_year` é o ano desde 1900
//
//	// Função `mktime` ajusta e normaliza a data
//	mktime(&date);
//
//	// Saída do dia da semana (0 = Domingo, 1 = Segunda, ..., 6 = Sábado)
//	std::cout << "Dom\tSeg\tTer\tQua\tQui\tSex\tSab\n";
//
//	for (size_t i = 0; i < 5; i++)
//	{
//		for (size_t a = 0; a < 7; a++)
//		{
//			if (i == 0)
//			{
//				if (date.tm_wday == a)
//				{
//					std::cout << day << "\t";
//					++day;
//					date.tm_mday = day;
//					mktime(&date);
//
//					if (a + 1 == 7)
//					{
//						std::cout << "\n";
//					}
//				}
//				else
//				{
//					std::cout << "\t";
//				}
//			}
//			else
//			{
//				if (monthCount >= day)
//				{
//					std::cout << day << "\t";
//					++day;
//					date.tm_mday = day;
//					mktime(&date);
//
//					if (a + 1 == 7)
//					{
//						std::cout << "\n";
//					}
//				}
//			}
//		}
//	}
//
//	9 --------------------------------------------
//
//	using std::cout;
//	using std::cin;
//	using std::string;
//	string response;	
//	float number;
//	int numberOfDivisions;
//	int decimalPlaces;
//	static float responseSquere;
//
//	cout << "Give me a Number:";
//	cin >> response;
//	number = std::stoi(response);
//
//	cout << "How many times should i do the squere root ?:";
//	cin >> response;
//	numberOfDivisions = std::stoi(response);
//
//	cout << "Decimal places:";
//	cin >> response;
//	decimalPlaces = std::stoi(response);
//
//	for (size_t i = 0; i < numberOfDivisions; i++)
//	{
//		if (i == 1)
//		{
//			responseSquere = number / 2;
//		}
//		else
//		{
//			responseSquere = (responseSquere + (number / responseSquere)) / 2;
//		}
//	}
//	cout << "Result:" << responseSquere;
//
//	
//
//	return 0;
//}