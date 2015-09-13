/*
 * \file FileGenerator.h
 * \brief Contains the class used to generate some test sets for the Intel contest V3
 * \author Cédric Andreolli
 *
 */

#ifndef FILEGENERATOR_H_
#define FILEGENERATOR_H_

#include <ostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
/*!
 * \class FileGenerator
 * \brief This class allows to generate  set of test files for the Intel contest v3
 */
class FileGenerator {
private:
	int _seqSize;/*!< size of a sequence*/
	int _refSeqSize;/*!< size of the reference sequence*/
	int _numSeq;/*!< number of sequences per file*/
	int _numFiles;/*!< number of files that will be generated*/

	float _copyRefSubStringProbability;

	std::string _refString;/*!< reference sequence*/
	std::string _filePrefix;
public:
	/*!
	 * \brief Default constructor
	 * \param maxSize : The maximum size of a sequence
	 * \param maxSeq : The maximum number of sequences on the same file
	 * \param numFiles : The desired number of file
	 */
	FileGenerator( int seqSize, int refSeqSize, int numSeq, int numFiles, std::string filePrefix, float copyRefSubStringProbability );
	/*!
	 * \brief Generates the reference file named refseqgen.txt
	 */
	void generateReference();
	/*!
	 * \brief Generates the test files named inputgen_i.txt where i is the id number of the generated file.
	 */
	void generateFiles();

	virtual ~FileGenerator();

protected:
	static char getRandomLetter();
	static void outputSequence( std::ostream &os, const std::string &name, const std::string &sequence );
};

#endif /* FILEGENERATOR_H_ */
