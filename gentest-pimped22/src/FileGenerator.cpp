/*
 * \file FileGenerator.cpp
 * \author Cédric Andreolli
 */

#include "FileGenerator.h"

using namespace std;

FileGenerator::FileGenerator( int seqSize, int refSeqSize, int numSeq, int numFiles, std::string filePrefix,
		float copyRefSubStringProbability ) :
		_seqSize( seqSize ), _refSeqSize( refSeqSize ), _numSeq( numSeq ), _numFiles( numFiles ), _filePrefix(
				filePrefix ), _copyRefSubStringProbability( copyRefSubStringProbability ) {
}

char FileGenerator::getRandomLetter() {
	static const char alphabet[] = { 'A', 'T', 'C', 'G' };
	return alphabet[rand() % sizeof(alphabet)];
}

void FileGenerator::outputSequence( ostream &os, const string &name, const string &sequence ) {
	os << ">" << name << endl;

	int numLines = (sequence.size() / 70) + 1;
	for( int i = 0 ; i < numLines ; i++ ) {
		os << sequence.substr( i * 70, 70 ) << endl;
	}
}

void FileGenerator::generateReference() {
	for( int i = 0 ; i < _refSeqSize ; i++ ) {
		const char letter = getRandomLetter();
		_refString += letter;
	}

	ofstream refFile( ("_a"+_filePrefix + "_ref.txt").c_str() );

	ostringstream refSequenceName;
	refSequenceName << _filePrefix << "_seq_ref" << " " << _refSeqSize;
	outputSequence( refFile, refSequenceName.str(), _refString );
}

void FileGenerator::generateFiles() {
	for( int currentFile = 0 ; currentFile < _numFiles ; currentFile++ ) {
		ostringstream inputFilename;
		inputFilename << _filePrefix << "_input_" << currentFile << ".txt";
		ofstream inputFile( inputFilename.str().c_str() );

		for( int currentSeq = 0 ; currentSeq < _numSeq ; currentSeq++ ) {
			string sequence;
			while( sequence.size() < _seqSize ) {
				double draw = double( rand() ) / RAND_MAX;
				if( draw < _copyRefSubStringProbability ) {
					const int maxSubstringLength = min( int( _seqSize - sequence.size() ), _refSeqSize );
					const int subStringLength = max( 1, rand() % maxSubstringLength );
					const int subStringStartRightBoundary = _refSeqSize - subStringLength;
					const int subStringStart = rand() % subStringStartRightBoundary;

					sequence += _refString.substr( subStringStart, subStringLength );
				}
				else {
					sequence += getRandomLetter();
				}
			}

			ostringstream sequenceName;
			sequenceName << _filePrefix << "_seq_" << (currentFile + 1) << "_" << (currentSeq + 1) << " " << _seqSize << "@cprob " << _copyRefSubStringProbability;
			outputSequence( inputFile, sequenceName.str(), sequence );
			inputFile << endl;
		}
	}
}

FileGenerator::~FileGenerator() {
}

