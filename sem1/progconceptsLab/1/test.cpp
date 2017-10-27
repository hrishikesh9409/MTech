#include <iostream>

using namespace  std;

int isPrime(int i){
	int flag = 0;
	for(int j = 2; j < i/2; j++){
		if(j % i == 0){
			flag = 1;
			break;
		}
	}
	if(flag == 0)
		return 1;
	else 
		return 0;
}

int sumPrimes(int n){

	int sum, i,j,k, temp, counter, flag;
	sum = 0; counter = 0;
	i = 2;j = 2;
	temp = n;flag = 0;
	while(counter != temp){
		if(isPrime(i) == 1){
			sum = sum + i;
			counter++;
		}
	}
	return sum;
}


int main(){

	int n;
	cin >> n;
	int sum = sumPrimes(n);
	cout << endl << sum << endl;
	return 0;
}