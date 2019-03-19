//============================================================================
// Name        : the_greatest_common_factor.cpp
// Author      : Kirill_Mikhaylov
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
//using namespace std;

int main() {
	int a, b; // 2 натуральных числа
	std::cin >> a >> b;
	while (a > 0 && b > 0) {
		if (a > b) {
		a = a%b;
		} else {
			b = b%a;
		}
	}
	std::cout << a+b;
	return 0;
}
