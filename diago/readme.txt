---------------------------------------------------
Jérémie Decouchant & David Glesser - Team 'A Cadoc'
---------------------------------------------------


In this file, we introduce the substring finding problem of the Accelerate Your Code 2012 contest, and our approach for solving it.  


	1. Substring finding problem
	----------------------------

Solving the problem consists in finding common substrings, of more than minMatchLength characters, that are present in a reference sequence (refSeq) and in another sequence among those available.  


	2 . Traditionnal approach, and its drawbacks
	--------------------------------------------

To do so, using dynamic programming is a traditional approach : after allocating a matrix, the characters of refSeq index lines and those of the studied sequence index columns.

The basic algorithm treats sequences sequentially, and fills a matrix depending on the equality of characters, searching for a value greater than minMatchLength. To compute the value (i,j) only value at (i-1,j-1), and (refSeq[i], otherSeq[j]) are necessary. Thus, the matrix can be filled diagonal per diagonal.

The memory needs of this method makes it unusable. Our program computes implicitly this matrix treating diagonals blocks of consecutive diagonals.


	3. Our approach : jobs, tasks and interleaving
	----------------------------------------------

We can distinguish three main jobs when treating a sequence: parsing it, treating its diagonals, and computing/printing results. While treating diagonals can be directly parallelized, the two others are not. Our solution is to interleave the treatment of sequences.

We use a pool of task to keep threads busy. Here are the important ideas: when the size of the pool is empty, a thread tries to fill it with new tasks, parsing a new sequence. Else it can take a task and execute it, if it is the last one concerning a sequence then the thread computes/prints final results.

To conclude, we'd like to thank Intel for this stimulating contest, in which we learned a lot.  
