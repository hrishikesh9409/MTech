#include <iostream>

using namespace std;

/*int sumPrime(int n){
	int i = 0, sum = 0;
	while(i < n){
		for(int j = 2; j < i/2; i++){
			if(i % j == 0){
				sum = sum + i;
			}
		}
		i++;
	}
	return sum;
}*/

int sumPrime(int n){
	int p = 100;nn
	int a[p];
	int i,j,k, sum;
	i = 1; j = 1; k = 0; sum = 0;

	while(i < p){
/*		if(i == 0){
			continue;
		}
		if(i == 1){
			continue;
		}
		else{*/
			while(j <= i){
				if(i % j != 0){
					a[k] = i;
				}
				k++;
				j++;
			}	
		//}
		i++;
		j = 1;
	}

	for(int l = 0; l < n; l++){
		sum = sum + a[l];
	}

	return sum;

}

int main(){

	int n;
	cin >> n;
	int sum = sumPrime(n);
	cout << endl << sum;
	return 0;
}