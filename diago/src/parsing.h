#pragma once

#include "global.h"

/*************************************************************/
/*** get_sequence                                          ***/
/*************************************************************/
// Parse a file from memory, and build a RawSequence structure
// from it. 

RawSequence * get_sequence(char **s);

/*************************************************************/
/*** parse_next_file                                       ***/
/*************************************************************/
// Parse the next file to study, fill currentSequences, and
// set currentSequence to its first element.

void parse_next_file();

/*************************************************************/
/*** create_sequence                                       ***/
/*************************************************************/
// Create a Sequence structure from a sequence.

Sequence * create_sequence(const char * otherSeq, LLU otherSeq_size);

/*************************************************************/
/*** parse_first_sequence                                  ***/
/*************************************************************/
// Parse the first sequence of a file. Should be called only
// once per file.

void parse_first_sequence(int files_sizeX, char ** filesX);

/*************************************************************/
/*** finish_get_first_sequence                             ***/
/*************************************************************/
// Return the first sequence. Should be called first and only 
// once !

Sequence * finish_get_first_sequence();

/*************************************************************/
/*** get_next_sequence                                     ***/
/*************************************************************/
// Return the next sequence, and NULL if there are no more
// sequences.

Sequence * get_next_sequence();





