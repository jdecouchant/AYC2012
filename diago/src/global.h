#pragma once

/****************************************************************/
/*** This file must be included before everything.            ***/
/*** It contains global variable, types and general inludes.  ***/
/***                                                          ***/
/****************************************************************/

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <pthread.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h> 
#include <string.h>


using namespace std;


#define LLI long long int
#define LLU long long unsigned


// Raw structure of a sequence, only the name and the sequence of symbols
typedef struct RawSequenceN 
{
	LLU sequence_size;
	char *sequence;
	string name;
} RawSequence;


// Results are put in such a structure
// It can represent a list or just an element
typedef struct ResultN
{
	LLU refEnd;
	LLU seqEnd;
	LLU length;
	struct ResultN *next;
} Result;


// Sequence structure
typedef struct SequenceN
{
	// Unique Id
	const LLU id;
	
	// Size of the sequence
	const LLU otherSeq_size;
	
	// Number of diagonals (otherSeq_size + refSeq_size - 1)
	const LLU nbr_diagos;
	
	// Final list of results
	vector<Result> * results;
	
	// Intermediary results matrix
	Result ** results_matrix;
	
	// "String" of the sequence
	const char * otherSeq;
	
	// Sequence name
	const string name;
	
	//number of diago that have been analyzed so far
	sem_t sem_diagos;
	
} Sequence;



// Task structure
// a Task is given to a thread, and the thread do it
typedef struct TaskN
{
	LLU line;
	LLU line_max;
	Sequence * sp;
	TaskN * next;
} Task;



/// Warning! These are global variables that should be initialized at the beginning of main.cpp


#define CLEAN

// Minimum size of searched sub strings, sometimes called "N"
extern LLU minMatchLength;

// Maximum number of working threads
extern LLU max_thread;

#define threshold 2048
#define task_per_thread 16

// "String" of the reference sequence
extern const char * refSeq;
// Size of the reference sequence
extern LLU refSeq_size;

// List of current tasks
extern Task *list_task;
// Mutex to protect list_task
extern pthread_mutex_t mutex_list_task;

// Only one thread can ouptut something at a time
extern pthread_mutex_t mutex_output;

// Id of the sequence for which the results can be printed
extern LLU canOutput;

