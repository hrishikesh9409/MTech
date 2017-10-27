#include <iostream>
#include <math.h>

using namespace std;

int main(){

	int n = 13195;
	int root = sqrt(n);
	int arr[root];
	int j = 0; int length;
	int prime[root];
	for(int i = 1; i <= sqrt(n); i++){
		if(n % i == 0){
			arr[j] = i;
			j++;
			length = j;
		}
	}
	j = 0;
	for(int i = 0; i < length; i++){
		cout << arr[i] << " "; 
	}
	for(int k = 0; k < length; k++){
		for(int i = 1; i <= sqrt(arr[k]); i++){
			if(arr[i] % i == 0){
				prime[j] = i;
				j++;	
				length = j;
			}
		}
	}
	return 0;
}