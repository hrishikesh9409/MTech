#include <iostream>

using namespace std;

int main()
{
	int n, i;
	cin >> n;
	int sum = 0; i = 1;
	while(i <= n){
		sum = sum + i;
		i = i + 2;
	}
	cout << endl << sum << endl;
	return 0;
}