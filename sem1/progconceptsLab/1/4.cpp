#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	int a,b,c;
	a = 0; b = 1; c = 1;
	int n, sum = 0;
	cin >> n;
	if(n == 1)
		cout << a;
	if(n == 2)
		cout << a << " " << b;
	if(n == 3)
		cout << a << " " << b << " " << c;
	else{
		cout << a << " " << b << " " << c;
		for(int i = 0; i < n; i++){
			sum = a + b + c;
			cout << " " << sum;
			a = b;
			b = c;
			c = sum;
		}
	}
	return 0;
}