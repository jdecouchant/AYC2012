#include "results.h"

/*************************************************************/
/*** Comparators                                           ***/
/*************************************************************/
// We redefine the comparison functions to be able to call
// the C++ sort function on our final results vector.

bool operator< (const Result& a, const Result& b) {
	if (a.refEnd == b.refEnd)
		return a.seqEnd < b.seqEnd;
	else 
		return a.refEnd < b.refEnd;
}

bool operator> (const Result& a, const Result& b) {
	if (a.refEnd == b.refEnd) 
		return a.seqEnd > b.seqEnd;
	else 
		return a.refEnd > b.refEnd;
}

/*************************************************************/
/*** deleteShiftableResults                                ***/
/*************************************************************/
// In order to obtain exactly the same results as the 
// reference implementation ones, we have to check if partial
// results of a diagonal can be shifted on the bottom diagonal
// or in the right one. Put correct results in sp->results.

void deleteShiftableResults(Sequence *sp) {
	
	// The partial results obtained so far.
	Result ** results_matrix = sp->results_matrix;
	
	// A vector that will contain final results.
	vector<Result> * results = new vector<Result>;
	
	bool notShiftable;
	Result *itnext;
		
	// For all results obtained in first diagonal, check only for diagonal on the right.
	for (Result *it = results_matrix[0]; it != 0; it = it->next) 
	{
		notShiftable = true;
		// Is it possible to shift the result?
		for (itnext = results_matrix[1]; itnext != 0; itnext = itnext->next) 
		{
			if( it->seqEnd < itnext->seqEnd  &&
			it->seqEnd - it->length >=  itnext->seqEnd - itnext->length - 1) 
			{
				notShiftable = false;
				break;
			}
		}
		// If not, add partial result to final results vector.
		if (notShiftable)
			results->push_back(*it);
	}
	
	// For all diagonals except the first one and the last one.
	// Check if it is possible to shift on the bottom diagonal and on the right one. 
	for (LLU i = 1; i < sp->nbr_diagos-1; ++i) 
	{
		
		// For all results obtained from this diagonal
		for (Result *it = results_matrix[i]; it != 0; it = it->next) 
		{
			notShiftable = true;
			// Is it possible to bottom-shift the result? 
			for (itnext = results_matrix[i-1]; itnext != 0; itnext = itnext->next) 
			{
				if( it->refEnd < itnext->refEnd && 
				it->refEnd - it->length >= itnext->refEnd - itnext->length  - 1 ) 
				{
					notShiftable = false;
					break;
				}
			}
			// Is it possible to right-shift the result?
			for (itnext = results_matrix[i+1]; itnext != 0; itnext = itnext->next) 
			{
				if( it->seqEnd < itnext->seqEnd  &&
				it->seqEnd - it->length >=  itnext->seqEnd - itnext->length - 1) 
				{
					notShiftable = false;
					break;
				}
			}
			// If not, add partial result to final results vector.
			if (notShiftable)
				results->push_back(*it);
		}
	}
	
	// For all results obtained in the last diagonal, check only for the bottom diagonal.
	for (Result *it = results_matrix[sp->nbr_diagos-1]; it != 0; it = it->next) 
	{
		notShiftable = true;
		// Is it possible to shift the result?
		for (itnext = results_matrix[sp->nbr_diagos-2]; itnext != 0; itnext = itnext->next) 
		{
			if( it->refEnd < itnext->refEnd && 
			it->refEnd - it->length >= itnext->refEnd - itnext->length  - 1 ) 
			{
				notShiftable = false;
				break;
			}
		}
		// If not, add partial result to final results vector.
		if (notShiftable)
			results->push_back(*it);
	}
	
	// Return the result in sequence properties
	sp->results = results;
}

/*************************************************************/
/*** printResults                                          ***/
/*************************************************************/
// This function prints the results obtained from a sequence.
// A mutex is needed because the print operation is critical. 
// In addition, results must be printed depending on the order
// of sequence files (we use a global counter to do so).

void printResults(Sequence *sp)
{
	// Take a lock on output rights
	pthread_mutex_lock (&mutex_output);
	
	// If there are sequences that wait to be printed, wait.
	while (canOutput != sp->id)
	{
		// Unlock to give them the right to print. 
		pthread_mutex_unlock (&mutex_output);
		
		// It is our turn, go !
		pthread_mutex_lock (&mutex_output);
	}
	
	// Print sequence name
	cout << sp->name << "\n";
	
	// Print final results of sequence 
	for(vector<Result>::iterator it = sp->results->begin(); it != sp->results->end(); it++)
	{
		cout	<< (*it).refEnd - (*it).length  << " "
		<< (*it).refEnd  << " "
		<< (*it).seqEnd - (*it).length << " "
		<< (*it).seqEnd << "\n";
	}
	
	// Allow next sequence results to be printed
	++canOutput;
	
	// Finally, unlock the output rights
	pthread_mutex_unlock (&mutex_output);
}
