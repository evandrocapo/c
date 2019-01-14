#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

void conta(int *base, int *exp);
int expo(int base, int exp);


void main(){
	int resultado,base,exp;
	resultado = 0;
	conta(&base,&exp);
	resultado = expo(base, exp);
	system("pause");

}

void conta(int *base, int *exp){
	int resultado;
	resultado = 0;
	printf("Digite o valor da base: ");
	scanf("%d", &*base);
	printf("Digite o valor do expoente: ");
	scanf("%d", &*exp);


}


int expo(int base, int exp){
	int i, resultado;
	resultado = 0;
	for (i = 0; i < exp; i++){
		if (i == 0){
			resultado = base*base;
		}
		else{
			resultado = resultado *base;
		}
	}
	printf("Base: %d \nExpoente: %d\nResultado: %d\n", base, exp, resultado);

	return resultado;
}
