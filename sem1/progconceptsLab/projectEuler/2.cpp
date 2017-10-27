#include <iostream>

using namespace std;

int main(){

	int a = 1;
	int b = 2;
	long int c = 0;
	long int sum = b;
	while(c < 4000000){
		c = a + b;
		if(c % 2 == 0){
			sum = sum + c;
		}
		a = b;
		b = c;
	}
	cout << "\n\n" << sum << "\n\n";
	return 0;
}