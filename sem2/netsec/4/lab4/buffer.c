#include <stdio.h>

int add(int x, int y){
	int a = 0xfeed;
	int b = 0xdead;
	int z = x + y;
	return z;
}

int main(){
	add(6,8);
	return 0xf00d;
}