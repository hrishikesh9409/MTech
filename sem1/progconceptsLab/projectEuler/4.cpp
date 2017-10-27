#include <iostream>
#include <bits/stdc++.h>

void palindrome(int i, int j){
	int n = i * j;
	int f;
	int p[20];
	int rev = 0, remainder, temp;
	temp = n;
	while(n != 0){
		remainder = n % 10;
		rev = rev * 10 + remainder;
		n = n / 10;		
	}
	if(temp == rev){
		if(i > 900 && j > 900)
			//cout << "j = " << j << "	i = " << i << endl;
			f = i * j;
			cout << f << endl;
	}
}

int main(){

	int n;
	int i = 10;
	int j = 10;
	for(int i = 100; i < 1000; i++){
		for(int j = 100; j < 1000; j++){
			palindrome(i, j);
		}
	}
	return 0;
}