#include <iostream>
#include <math.h>

using namespace std;

int main(){

	float value = 80 + pow(2, 4);
	value = fmod(value, pow(2, 7));
	cout << value << endl << endl;
	return 0;
}