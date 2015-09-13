/**
 * Generate random sequences
 * usage: testgen nbr_sequences lenght_sequences
 * 
 * nbr_sequences should be < MAX_INT
 * lenght_sequences should be < MAX_LONG_LONG
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 

char symboles[4] = {'G', 'T', 'A', 'C'};

int main(int argc, char* argv[]){
	
	if(argc != 3) {
		printf("usage: %s nbr_sequences lenght_sequences\n", argv[0]);
		return 0;
	}
	
	int nbr_sequence = atoi(argv[1]);
	long long lenght_sequences = atoll(argv[2]);
	
	//initialize a seed
	srand((unsigned)time(NULL));
	
	for(int seq = 0; seq < nbr_sequence;seq++) {
		printf(">Sequence N°%i\n", seq);
		for(long long w = 0; w < lenght_sequences; w++) {
			printf("%c", symboles[rand()%4]);
		}
		printf("\n");
	}
	
	return 0;
}