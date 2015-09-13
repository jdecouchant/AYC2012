#pragma once

#include "global.h"

/*************************************************************/
/*** Comparators                                           ***/
/*************************************************************/
// We redefine the comparison functions to be able to call
// the C++ sort function on our final results vector.

bool operator<(const Result& a,const Result& b);
bool operator>(const Result& a,const Result& b);

/*************************************************************/
/*** deleteShiftableResults                                ***/
/*************************************************************/
// In order to obtain exactly the same results as the 
// reference implementation ones, we have to check if partial
// results of a diagonal can be shifted on the bottom diagonal
// or in the right one. Put correct results in sp->results.

void deleteShiftableResults(Sequence *sp);

/*************************************************************/
/*** printResults                                          ***/
/*************************************************************/
// This function prints the results obtained from a sequence.
// A mutex is needed because the print operation is critical. 
// In addition, results must be printed depending on the order
// of sequence files (we use a global counter to do so).

void printResults(Sequence *sp);
