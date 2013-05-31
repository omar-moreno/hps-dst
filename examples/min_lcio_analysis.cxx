/**
 *	@section purpose:
 *		An example demonstrating how to read an LCIO file using the LCIO C++
 *		API.  This file should serve as a starting point for more complex
 *		analyses.
 *
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date:		May 21, 2013
 *	@version:   v1.0
 *
 */

//--- C++ ---//
#include <iostream>

//--- LCIO ---//
#include <IO/LCReader.h>
#include <IOIMPL/LCFactory.h>
#include <EVENT/LCEvent.h>
#include <Exceptions.h>
#include <IMPL/LCCollectionVec.h>

using namespace std;

int main( int argc, char **argv )
{
	// Parse any command line arguments.  If there are no valid command line
	// arguments given, print the usage.
	// Note: For this simple example, the user will only be required to pass
	//       the name of the *.slcio file as a command line argument.  The
	// 		 command line arguments are then parsed using getopt.  An even
	//       simpler but less clean solution would be to hard code the name
	// 		 of the file into the code directly but I don't recommend this.
	string lcio_file_name = "";
	int option_char;
	while((option_char = getopt(argc, argv, "i:")) != -1){
		switch(option_char){
			case 'i':
				lcio_file_name = optarg;
				break;
			default:
				cout << "Usage: min_lcio_analysis [options] \nOptions:\n"
				     << "\t-i <Input LCIO file name> \n"
				     << endl;
				return(2);
		}
	}

	// Collection names
	string track_col_name = "MatchedTracks";

	// Create the LCIO reader and open the LCIO file specified by the user.
	// If the file doesn't exist or can't be opened, notify the user and exit
	IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader();
	try{
		lc_reader->open(lcio_file_name.c_str());
	} catch(IO::IOException exception){
		cout << "File " << lcio_file_name << " cannot be opened!" << endl;
		return(2);
	}


	// Loop over all events in the file until the end of file is reached.
	EVENT::LCEvent *event = 0;
	IMPL::LCCollectionVec* tracks = 0;
	while( (event = lc_reader->readNextEvent()) ){

		// Get the collection of tracks from the event. If the event doesn't
		// have the specified collection, skip the rest of the event
		try{
			tracks = (IMPL::LCCollectionVec*) event->getCollection(track_col_name);
		} catch(EVENT::DataNotAvailableException exception){
			cout << "Collection " << track_col_name << " was not found. "
				 << "Skipping event ..." << endl;
			continue;
		}
	}

	return 0;

}
