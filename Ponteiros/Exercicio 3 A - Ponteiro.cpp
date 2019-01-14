#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

void receber(int *x);

void main(){
	int x;
	receber(&x);
	printf("%d\n", x);
	system("pause");
}

void receber(int *x){
	printf("Digite um valor:");
	scanf("%d", &*x);
	if (*x < 0){
		*x = *x*(-1);
	}
}