#include "parsing.h"

/*************************************************************/
/*** Global variables linked to sequence files             ***/
/*************************************************************/

// global variables for sequence analysis
//current file analyzed
int currentFile = 4; // start from the position in arguments of program
//total number of size
int files_size;
//array of filenames
char ** files;
//current avalaible sequences
map<string,RawSequence *> currentSequences;
//current sequence
map<string,RawSequence *>::iterator currentSequence;

// A counter that will be incremented and used to give an unique id to each new sequence
LLU prec_id = 0;


/*************************************************************/
/*** get_sequence                                          ***/
/*************************************************************/
// Parse a file from memory, and build a RawSequence structure
// from it. 

RawSequence * get_sequence(char **s)
{
	
	
	// Get name
	string name;
	if (**s=='>') 
	{
		++*s;
		
		// We have deleted this test : && **s != '\0') {
		// because we make the assumption that files are well written
		while (**s != '\n') 
		{
			name.push_back(**s);
			++*s;
		}
	}
	
	
	// Get sequence
	
	// Size of the "string"
	LLU sequence_size = 0;
	// Size of the array that contains the string
	LLU sequence_real_size = 2048*32;
	char * sequence = (char *) malloc(sizeof(char)*sequence_real_size);
	++*s;
	
	while (**s != '\0') 
	{
		if (**s == '>') 
		{
			--s;
			break;
		}
		else if (**s != '\n')
		{
			sequence[sequence_size] = **s;
			++sequence_size;
			if (sequence_size >= sequence_real_size)
			{
				sequence_real_size *= 2;
				sequence = (char *)realloc(sequence, sequence_real_size);
			}
		}
		// ignore every other chars (newline, etc)
		
		++*s;
	}
	sequence[sequence_size+1] = 0;
	
	
	return new RawSequence({sequence_size, sequence, name});
	
}


/*************************************************************/
/*** parse_next_file                                       ***/
/*************************************************************/
// Parse the next file to study, fill currentSequences, and
// set currentSequence to its first element.

void parseNextFile()
{
	// analyse file
	int fd = open (files[currentFile], O_RDONLY);
	struct stat sb;
	fstat (fd, &sb);
	char * seqFile = (char *) mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	madvise(seqFile, sb.st_size, POSIX_MADV_WILLNEED);
	
	// Push all sequence in the map, to sort them as the reference program
	char * seqFileP = seqFile;
	while (*seqFileP != '\0')
	{
		RawSequence * other = get_sequence(&seqFileP);
		currentSequences[other->name] = other;
	}
	
	munmap(seqFile, sb.st_size);
	close(fd);
	
	currentSequence = currentSequences.begin();
}


/*************************************************************/
/*** create_sequence                                       ***/
/*************************************************************/
// Create a Sequence structure from a sequence.

Sequence * createSequence()
{
	
	LLU otherSeq_size = currentSequence->second->sequence_size;
	
	// Compute the number of diagonals that must be studied
	LLU nbr_diagos = otherSeq_size + refSeq_size;
	
	// Compute the size diagonal blocks will have
	LLU block_size = nbr_diagos / (task_per_thread * max_thread);
	if (block_size < threshold) 
	{
		block_size = threshold;
	}
	
	// Compute the number of blocks it represents
	LLU block_nbr = ceil((long double) nbr_diagos / (long double) block_size); 
	
	//number of diago that have been analyzed so far
	sem_t sem_diagos;
		
	Sequence *sp = new Sequence({
		prec_id++, // id,
		otherSeq_size,
		nbr_diagos,
		0,
		(Result **) malloc(sizeof(Result *)*nbr_diagos), //results_matrix,
		currentSequence->second->sequence, //otherSeq,
		currentSequence->first, //name,
		sem_diagos
	});
	
	// Initialize the semaphore for merging results
	sem_init(&sp->sem_diagos, 0, block_nbr - 1);
	
#ifdef CLEAN
	delete currentSequence->second;
#endif
	
	return sp;
}


/*************************************************************/
/*** parse_first_sequence                                  ***/
/*************************************************************/
// Parse the first sequence of a file. Should be called only
// once per file.

void parse_first_sequence(int files_sizeX, char ** filesX)
{
	//set global variables
	files_size = files_sizeX;
	files = filesX;
	
	//no files
	if (currentFile >= files_size)
		return;
	
	//analyze first file
	parseNextFile();
	
}


/*************************************************************/
/*** finish_get_first_sequence                             ***/
/*************************************************************/
// Return the first sequence. Should be called first and only 
// once !

Sequence * finish_get_first_sequence()
{
	return createSequence();
}


/*************************************************************/
/*** get_next_sequence                                     ***/
/*************************************************************/
// Return the next sequence, and NULL if there are no more
// sequences.

Sequence * get_next_sequence()
{
	if( currentFile >= files_size)
		return 0;
	
	++currentSequence;
	
	while (currentSequence == currentSequences.end())
	{
		currentSequences.clear();
		
		++currentFile;
		if (currentFile >= files_size)
			return 0;
		
		parseNextFile();
	}
	
	// Sequence creation
	return createSequence();
}







