/**
 * pour compiler :
 * gcc -O3 -march=native sttni.c
 */

#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <pmmintrin.h>
/*extern "C" 
{*/
	//SSE2
	#include <emmintrin.h>
	#include <mmintrin.h>
	//SSE3
	#include <pmmintrin.h>
	#include <immintrin.h>   // (Meta-header, for GCC only)
/*}

// WITHOUT extern "C"
#include <smmintrin.h>
*/

/* a la place de _SIDD_UBYTE_OPS
 * SIDD_SBYTE_OPS
 * SIDD_UWORD_OPS
 * SIDD_SWORD_OPS
 */


#define MODE (_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY)
int _STTNI_strdiff (const char *p1, size_t as, const char *p2, size_t bs) {
	
	__m128i smm1 = _mm_loadu_si128 ((__m128i *) p1);
	__m128i smm2 = _mm_loadu_si128 ((__m128i *) p2);
	int ResultIndex;
	
	int sum = 0;
	
	while (1) {
		if(sum >= as)
			goto ass;
		if(sum >= bs)
			goto bss;
		
		ResultIndex = _mm_cmpistri (smm1, smm2, MODE);
		if (ResultIndex != 16) { return sum+ResultIndex; }
		p1 = p1+16;
		p2 = p2+16;
		sum += 16;
		smm1 = _mm_loadu_si128 ((__m128i *)p1);
		smm2 = _mm_loadu_si128 ((__m128i *)p2);
	}
	
	
	ass:
	return as;
	bss:
	return bs;
}




void printBits64(int64_t a)
{
	int i  , k , mask;
	
	for( i =63 ; i >= 0 ; i--)
	{
		if(!((i+1)%8))
			printf(" ");
		mask = 1 << i;
		k = a & mask;
		if( k == 0)
			printf("0");
		else
			printf("1");
	}
}

void printBits32(int32_t a)
{
	int i  , k , mask;
	
	for( i =31 ; i >= 0 ; i--)
	{
		if(!((i+1)%8))
			printf(" ");
		mask = 1 << i;
		k = a & mask;
		if( k == 0)
			printf("0");
		else
			printf("1");
	}
}

void print128(__m128i value) {
	int64_t *v64 = (int64_t*) &value;
	printBits64(v64[0]);
	printf("\n");
	printBits64(v64[1]);
	printf("\n");
}



// size_t sse2Strlen(const char *str)
int _SSE2_strdiff (const char *p1, size_t as, const char *p2, size_t bs)
{
	__m128i zero = _mm_setzero_si128();
	int offset = 0;
	__m128i xmm1;
	const char *char_ptr;
	int mask;
	
	
	__m128i smm1 = _mm_loadu_si128 ((__m128i *) p1);
	__m128i smm2 = _mm_loadu_si128 ((__m128i *) p2);
	
	xmm1 =  _mm_cmpeq_epi8(smm1, smm2);
	print128(xmm1);
	
	printf("_mm_movemask_epi8 ");
	int dada = _mm_movemask_epi8(xmm1);
	printf("dada: %X\n", dada);
	char *d = (char *) &dada;
	printf("d: %i\t%i\n", ~d[0],16-~d[1]);
	
	printBits32(dada);
	printf("\n");
	
	printf("bi: %i\n", __builtin_ctz(dada));
	
	
	/*
	for(;;) {
		xmm1 = _mm_cmpeq_epi8(p1, p2);
		if ((mask = _mm_movemask_epi8(xmm1)) != 0) {
			return (char *) (char_ptr) + __builtin_ctz(mask) - str;
		}
		char_ptr += 16;
	}*/
	
	return 0;
}









int man_strdiff (const char *p1, size_t as, const char *p2, size_t bs)
{
	int sum = 0;
	while(1)
	{
		if(sum >= as)
			goto ass;
		if(sum >= bs)
			goto bss;
		if( *p1 != *p2)
			return sum;
		else
		{
			sum++;
			p1++;
			p2++;
		}
	}
	ass:
	return as;
	bss:
	return bs;
}

int main(void)
{
	char * a = "0123456789012345";
	
	char * b = "0123456789012345aa";
	
	size_t as = strlen(a);
	size_t bs = strlen(b);
	printf("%li\t%li\n", as, bs);
	printf("%i\n", _SSE2_strdiff(a,as, b,bs));
	printf("%i\n", _STTNI_strdiff(a,as, b,bs));
	printf("%i\n", man_strdiff(a,as, b,bs));
	
	
	return 0;
}


