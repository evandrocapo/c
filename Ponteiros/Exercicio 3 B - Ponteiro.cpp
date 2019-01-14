#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

int receber();

void main(){
	int x;
	x = receber();
	printf("%d\n", x);
	system("pause");
}

int receber(){
	int x;
	printf("Digite um valor:");
	scanf("%d", &x);
	if (x < 0){
		x = x*(-1);
	}
	return x;
}