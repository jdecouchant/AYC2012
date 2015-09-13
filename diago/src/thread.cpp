#include "thread.h"

#if defined __SSE3__
	#include <pmmintrin.h>
	
	#if defined __GNUC__
		#include <immintrin.h>   // (Meta-header, for GCC only)
	#endif
#endif

/*************************************************************/
/*** diago_analysis                                        ***/
/*************************************************************/
// Search for common substring of refSeq and sp->otherSeq
// from character i in refSeq, and character j in sp->otherSeq.
// Results are inserted in listForResults.
//
// This function exists in 3 different implementations, depending of minMatchLength


// when minMatchLength > 16
void diago_analysis(LLU i, LLU j, Result **listForResults, const Sequence * sp)
{
	
	const char * otherSeq = sp->otherSeq;
	const LLU otherSeq_size = sp->otherSeq_size;
	
	char const * refSeqConst = refSeq;
	
	LLU length;
	char *si, *sj;
	
	char * refPos = (char *) &refSeqConst[i];
	char * otherPos = (char *) &otherSeq[j];
	
	LLU diago_pos = 0;
	const LLU diago_max = min(refSeq_size - i, otherSeq_size - j);
	
	const LLU minMatchLengthConst = minMatchLength;
	
	while (diago_pos < diago_max)
	{
		
		if (minMatchLengthConst > 16)
		{
			// jump until a common character is found
			while (diago_pos < diago_max && *((int64_t *) refPos) != *((int64_t *) otherPos)) 
			{
				refPos += minMatchLengthConst-7;
				otherPos += minMatchLengthConst-7;
				diago_pos += minMatchLengthConst-7;
			}
		} else if (minMatchLengthConst > 8)
		{
			// jump until a common character is found
			while (diago_pos < diago_max && *((int *) refPos) != *((int *) otherPos)) 
			{
				refPos += minMatchLengthConst-3;
				otherPos += minMatchLengthConst-3;
				diago_pos += minMatchLengthConst-3;
			}
		} else
		{
			// jump until a common character is found
			while (diago_pos < diago_max && *refPos != *otherPos) 
			{
				refPos += minMatchLength;
				otherPos += minMatchLength;
				diago_pos += minMatchLength;
			}
		}
		
		// go backward, and then forward, from this common character to find a substring
		if (diago_pos < diago_max)
		{
			length = 0;
			
			si = refPos;
			sj = otherPos;
			//find the first char
			while ((--si) > refSeqConst && (--sj) > otherSeq && *si == *sj) 
			{
				++length;
			}
			
			
			//find the last char
			#if defined __SSE3__
			++refPos;
			++otherPos;
			++diago_pos;
			
			while(diago_pos < diago_max && ((unsigned long int) refPos & (sizeof (__m128i) - 1)) != 0 )
			{
				if( *refPos != *otherPos)
					goto end_SSE_last_char;
				++refPos;
				++otherPos;
				++diago_pos;
				++length;
			}
			
			
			
			while (1) {
				if(diago_pos >= diago_max)
				{
					const LLU tt = diago_pos - diago_max;
					refPos -= tt;
					otherPos -= tt;
					length -= tt;
					diago_pos -= tt;
					
					break;
				}
				__m128i smm1 = _mm_loadu_si128 ((__m128i *) (refPos));
				__m128i smm2 = _mm_loadu_si128 ((__m128i *) (otherPos));
				
				#define MODE (_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY)
				const int ResultIndex = _mm_cmpistri (smm1, smm2, MODE);
				if (ResultIndex != 16)
				{
					refPos += ResultIndex;
					otherPos += ResultIndex;
					length += ResultIndex;
					diago_pos += ResultIndex;
					break;
				}
				refPos += 16;
				otherPos += 16;
				length += 16;
				diago_pos += 16;
			}
			end_SSE_last_char:
			
			#else //no SSE3
			++refPos;
			++otherPos;
			++diago_pos;

			while (diago_pos < diago_max && *refPos == *otherPos) 
			{
				++refPos;
				++otherPos;
				++diago_pos;
				++length;
			}
			#endif
			
			if (length + 1 >= minMatchLengthConst)
			{
				// Add partial result in head of list of results
				Result * r = (Result *) malloc(sizeof(Result));
				r->refEnd = i + diago_pos;
				r->seqEnd = j + diago_pos;
				r->length = length;
				r->next = *listForResults;
				*listForResults = r;
			}
		}
	}
}






/*************************************************************/
/*** create_new_task_list_from_new_sequence                ***/
/*************************************************************/
// Search for common substring of refSeq and sp->otherSeq
// from character i in refSeq, and character j in sp->otherSeq.
// Results are inserted in listForResults.

void create_new_task_list_from_new_sequence()
{
	Sequence *sp = get_next_sequence();
	
	if (sp == 0)
		return;
	
	LLU block_size = sp->nbr_diagos / (task_per_thread * max_thread);
	
	if (block_size < threshold) 
		block_size = threshold;
	
	// Be sure that the list of task is NULL.
	list_task = 0;
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
/*** get_task                                              ***/
/*************************************************************/
// If the list of task in not empty, take one and execute it.
// If the list is empty, if possible add new tasks to it. 
// As the list of task is shared, this function uses a mutex.

Task * get_task()
{
	Task *taskToDo = NULL;
	pthread_mutex_lock (&mutex_list_task);
	
	if (!list_task) {
		create_new_task_list_from_new_sequence();
	}

	if (list_task) {
		taskToDo = list_task;
		list_task = list_task->next;
	}
	
	pthread_mutex_unlock (&mutex_list_task);
	return taskToDo;
}

/*************************************************************/
/*** thread_diago_analysis                                 ***/
/*************************************************************/
// This is the main function that threads execute. 
// Threads take tasks if possible. When they study the last
// diagonals of a sequence, they are then in charge of merging 
// the results. 

void *thread_diago_analysis(void *arg)
{
	while (1)
	{
		// Try to get a task.
		Task *taskToDo = get_task();

		// Exit if there are no more tasks to treat.
		if (!taskToDo) 
			pthread_exit(0);
		
		// Get parameters, and then free task
		LLU line_index = taskToDo->line;
		LLU line_limit = taskToDo->line_max;
		Sequence * sp = taskToDo->sp;
		
#ifdef CLEAN
		free(taskToDo);
#endif
		
		// A pointer toward the list in which results corresponding to a diagonal must be inserted.
		Result ** result_in_matrix = &sp->results_matrix[line_index];
		
		const LLU refSeq_sizeConst = refSeq_size;
		
		LLU real_line = refSeq_sizeConst - line_index - 1;
		while (line_index < min(refSeq_sizeConst, line_limit))
		{
			*result_in_matrix = 0;
			
				diago_analysis(real_line, 0, result_in_matrix, sp);
			
			++line_index;
			++result_in_matrix;
			--real_line;
		}
		
		real_line = line_index - refSeq_sizeConst + 1;
		if (real_line > 0) 
		{
			while (line_index < min(sp->nbr_diagos, line_limit))
			{
				*result_in_matrix = 0;
					diago_analysis(0, real_line, result_in_matrix, sp);
				
				++line_index;
				++result_in_matrix;
				++real_line;
			}
		}
		
		// When all diagonals have been studied, it is time to end the study of this sequence. 
		// sem_trywait return -1 when the semaphore can't be decremented. 
		if (sem_trywait(&sp->sem_diagos) != 0)
		{
			deleteShiftableResults(sp);
			
			sort(sp->results->begin(), sp->results->end());
			
			printResults(sp);

			free_sequence(sp);
		}
	}
	
	return 0;
}

/*************************************************************/
/*** free_sequence                                         ***/
/*************************************************************/
// This procedure is used to free the memory used in a 
// sequence structure, when it is not needed anymore.

inline void free_sequence(Sequence *sp)
{
	free((char *)sp->otherSeq);
	
#ifdef CLEAN
	//delete all Result
	Result *it, *temp;
	for (LLU i = 0; i < sp->nbr_diagos; ++i) 
	{
		it = sp->results_matrix[i];
		while (it != 0)
		{
			temp = it;
			it = it->next;
			free(temp);
		}
	}
	
	//every elements of the 2 structure have already been freed
	free(sp->results_matrix);
	delete sp->results;
#endif
	
	sem_destroy(&sp->sem_diagos);
	
	delete sp;
}

