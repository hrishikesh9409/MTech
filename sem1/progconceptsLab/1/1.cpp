#include <iostream>

using namespace std;

bool isPalindrome(int n){
	int digit, sum, temp, rev;
	temp = n;
	rev = 0;
	sum = 0;
	while(temp != 0){
		digit = temp % 10;
		rev = (rev * 10) + digit;
		temp = temp / 10;
	}
	if(n == rev)
		return true;
	else 
		return false;
}

int main()
{
	int n;
	bool var;
	cin >> n;
	var = isPalindrome(n);
	cout <<	"\n" << var;
	return 0;
}