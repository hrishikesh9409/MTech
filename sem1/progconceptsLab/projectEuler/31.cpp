#include <iostream>
#include <math.h>

using namespace std;

void factor(int n){
	int root = sqrt(n);
	int arr[root];
	int j = 0;
	int length;
	if(n % 2 == 0){
		//cout << "2 ";
		n = n/2;
		arr[j] = 2;
		j++;
		length = j;
	}
	for(int i = 3; i < root; i+2){
		while(n % i == 0){
			//cout << i << " ";
			n = n / i;
			arr[j] = i;
			j++;
			length = j;
		}
	}
	if(n > 2){
		//cout << n << " ";
		arr[j] = n;
		j++;
		length = j;
	}
	for(int k = 0; k < length; k++){
		cout << arr[k] << " ";
	}


}

int main(){

	int n = 1232424324;
	factor(n);
	cout << endl;
	return 0;
}