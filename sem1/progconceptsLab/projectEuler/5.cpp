#include <iostream>

using namespace std;

int main(){

	int num = 0;
	int i = 2;
	int j = 2;
	int count = 0;
	while(1){
		while(j <= 10){
			if(i % j == 0){
				count = count + 1;
				continue;
			}
			if(count == 8){
				num = i;
				break;
			}
			j++;
		}
		i++;
	}	
	cout << endl << num << endl;
	return 0;
}