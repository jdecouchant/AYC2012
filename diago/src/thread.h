#pragma once

#include "global.h"
#include "results.h"
#include "parsing.h"

/*************************************************************/
/*** create_new_task_list_from_new_sequence                ***/
/*************************************************************/
// Search for common substring of refSeq and sp->otherSeq
// from character i in refSeq, and character j in sp->otherSeq.
// Results are inserted in listForResults.

void create_new_task_list_from_new_sequence();

/*************************************************************/
/*** get_task                                              ***/
/*************************************************************/
// If the list of task in not empty, take one and execute it.
// If the list is empty, if possible add new tasks to it. 
// As the list of task is shared, this function uses a mutex.

Task * get_task();

/*************************************************************/
/*** thread_diago_analysis                                 ***/
/*************************************************************/
// This is the main function that threads execute. 
// Threads take tasks if possible. When they study the last
// diagonals of a sequence, they are then in charge of merging 
// the results. 

void *thread_diago_analysis(void *arg);

/*************************************************************/
/*** free_sequence                                         ***/
/*************************************************************/
// This procedure is used to free the memory used in a 
// sequence structure, when it is not needed anymore.

inline void free_sequence(Sequence *sp);
