#include <stdio.h>

long long int dudu = 12;

// prototype de la fonction ecrite en assembleur
long long int plus(long long int a, long long int b);
/*
int plus(long long int a, long long int b)
{
	return a + b;
}*/

int main(void)
{
	long long int a = 5;
	long long int b = 4;
	long long int x = plus(a, b);
	while(x > 43)
		x--;
	printf("La somme vaut %li\n", x);
	return 0;
}
