#include <iostream>

using namespace std;

int sumDigits(int n){
	int sum = 0;
	int temp = n;
	int digit;
	while(temp != 0){
		digit = temp % 10;
		sum = sum + digit;
		temp = temp / 10;
	}
	return sum;
}

int main(){

	int n;
	cin >> n;
	int sum = sumDigits(n);
	cout << "\n" << sum << endl;

	return 0;
}