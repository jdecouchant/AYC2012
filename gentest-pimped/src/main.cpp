#include <iostream>
#include <string>
#include <fstream>
#include "FileGenerator.h"

using namespace std;

int main( int argc, char* argv[] ) {
	srand( time( NULL ) );

	if( argc < 5 ) {
		cout << "command-line parameters:" << endl
				<< "\trefSeqSize seqSize #seq #files [filePrefix=data] [copyRefSubStringProbability=1]" << endl
				<< endl
				<< "example :" << endl << "./datagen 500 4000 2 1 data 0.5" << endl;
		return -1;
	}

	int refSeqSize = atoi( argv[1] ); //Maximum size of the sequences
	int seqSize = atoi( argv[2] ); //Maximum size of the sequences
	int numSeq = atoi( argv[3] ); //Maximum number of sequences
	int numFiles = atoi( argv[4] ); //The number of file

	string filePrefix = "data";
	if( argc >= 6 )
		filePrefix = argv[5];

	float copyRefSubStringProbability = 1;
	if( argc >= 7 )
		copyRefSubStringProbability = atof( argv[6] );

	cout << "generating ref seq (" << refSeqSize << " bases)" << " and " << numSeq << " sequences x " << numFiles
			<< " files (" << seqSize << " bases) with substring copy probability " << copyRefSubStringProbability << "..." << endl;

	FileGenerator fg( seqSize, refSeqSize, numSeq, numFiles, filePrefix, copyRefSubStringProbability );
	fg.generateReference();
	fg.generateFiles();
	return 0;
}

