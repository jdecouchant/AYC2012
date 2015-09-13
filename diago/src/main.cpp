#include "global.h"
#include "parsing.h"
#include "results.h"
#include "thread.h"

/*************************************************************/
/*** Declaration and affectation of global variables       ***/
/*************************************************************/

LLU minMatchLength;
LLU max_thread;

const char * refSeq;
LLU refSeq_size;

Task *list_task = NULL;
pthread_mutex_t mutex_list_task = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_output = PTHREAD_MUTEX_INITIALIZER;
LLU canOutput = 0;

/// Global variable of this file

int argc;
char** argv;

// Semaphore for the end of the reference parsing
sem_t sem_analyseRefSeq;
// Semaphore for the end of the initialisation
sem_t sem_initFinished;


/*************************************************************/
/*** init_ref_seq                                          ***/
/*************************************************************/
// Initialize the reference sequence

void init_ref_seq()
{
	// Third command line argument : file of reference sequence
	int fd = open (argv[3], O_RDONLY);
	
	// Map this file in memory
	struct stat sb;
	fstat (fd, &sb);
	char * refSeqFile = (char *) mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	madvise(refSeqFile, sb.st_size, POSIX_MADV_WILLNEED);
	
	// Analyse it, and affect global variables
	char * refSeqFileP = refSeqFile;
	RawSequence * ref = get_sequence(&refSeqFileP);
	
	refSeq = ref->sequence;
	refSeq_size = ref->sequence_size;
	
#ifdef CLEAN
	delete (ref);
#endif
	
	close(fd);
	munmap(refSeqFile, sb.st_size);
}


/*************************************************************/
/*** init_finish_first_sequence                            ***/
/*************************************************************/
// Finish initialize the first Sequence
// Before this function the first sequence is only initialize
// We need to do this because a Sequence structure
// need the reference sequence to be initialized to be valid

void init_finish_first_sequence()
{
	// Get first sequence to study.
	Sequence *sp = finish_get_first_sequence();
	
	// Create tasks for this first sequence.	
	LLU block_size = sp->nbr_diagos / (task_per_thread * max_thread);
	
	if (block_size < threshold)
		block_size = threshold;
	
	LLU i = 0;
	while (i < sp->nbr_diagos)
	{
		Task * newTask = (Task *) malloc(sizeof(Task));
		newTask->line = i;
		i += block_size;
		newTask->line_max = i;
		newTask->sp = sp;
		newTask->next = list_task;
		list_task = newTask;
	}
}


/*************************************************************/
/*** thread_diago_analysis_and_analyse_ref                 ***/
/*************************************************************/
// Analyse reference sequence, then launch thread_diago_analysis

void * thread_diago_analysis_and_analyse_ref(void*)
{
	init_ref_seq();
	
	sem_post(&sem_analyseRefSeq);
	
	sem_wait(&sem_initFinished);
	
	thread_diago_analysis(0);
	
	return 0;
}


/*************************************************************/
/*** thread_diago_analysis_and_analyse_first_seq           ***/
/*************************************************************/
// Analyse first input sequence, then launch thread_diago_analysis

void * thread_diago_analysis_and_analyse_first_seq(void*)
{
	
	parse_first_sequence(argc, argv);
	
	sem_wait(&sem_analyseRefSeq);
	
	init_finish_first_sequence();
	
	// We have to wake up main() and thread_diago_analysis_and_analyse_ref()
	sem_post(&sem_initFinished);
	sem_post(&sem_initFinished);
	
	thread_diago_analysis(0);

	return 0;
}


/*************************************************************/
/*** Main                                                  ***/
/*************************************************************/

int main(int argcT, char* argvT[])
{
	argc = argcT;
	argv = argvT;
	
	// First command line argument : number of worker threads to use
	max_thread = atol(argv[1]);

	// Second command line argument : window size
	minMatchLength = atol(argv[2]);
	
	
	
	if (max_thread > 1)
	{
		// Allocate memory for threads.
		pthread_t *threads = (pthread_t *) malloc(max_thread * sizeof(pthread_t));
		
		// Initialize semaphores for the initialisation
		sem_init(&sem_initFinished, 0, 0);
		sem_init(&sem_analyseRefSeq, 0, 0);
		
		// The 2 first thread will not only compute
		// they will also analyse the first sequences in parallel
		pthread_create (&threads[0], NULL, thread_diago_analysis_and_analyse_ref, (void *) 0);
		pthread_create (&threads[1], NULL, thread_diago_analysis_and_analyse_first_seq, (void *) 0);
		
		// We wait for the initialization to finish
		sem_wait(&sem_initFinished);
		
		
		// Start other threads
		for (LLU i = 2; i < max_thread; ++i)
		{
			pthread_create (&threads[i], NULL, thread_diago_analysis, (void *) 0);
		}
		
		
		// Wait for all threads to finish.
		for (LLU i = 0; i < max_thread; ++i)
		{
			pthread_join (threads[i], NULL);
		}
		
		// Free the memory allocated for threads.
#ifdef CLEAN
		free(threads);
#endif
		sem_destroy(&sem_initFinished);
		sem_destroy(&sem_analyseRefSeq);
		
	} else // no need to start thread
	{
		init_ref_seq();
		parse_first_sequence(argc, argv);
		init_finish_first_sequence();
		
		thread_diago_analysis(0);
	}
	
	free((char *)refSeq);
	
	return 0;
}
