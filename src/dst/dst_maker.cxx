/**
 *	@section purpose: 
 *		Creates an HPS Data Summary Tape
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: December 20, 2013
 *	@version: 1.0
 *
 */

//--- C++ ---//
//-----------//
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>

//--- LCIO ---//
//------------//
#include <IO/LCReader.h>
#include <IOIMPL/LCFactory.h>
#include <EVENT/LCEvent.h>

//--- ROOT ---//
//------------//
#include <TTree.h>
#include <TFile.h>

//--- DST ---//
//-----------//
#include <HpsEventBuilder.h>

//--- HPS Event ---//
//-----------------//
#include <HpsEvent.h>

using namespace std;

void printUsage(); 

int main(int argc, char **argv)
{

	clock_t initial_time = clock();

	string lcio_file_name;
	string dst_file_name; 	
	int option_char;
	int n_events = -1; 	
	double b_field = numeric_limits<double>::quiet_NaN();  
	bool do_gbl = false;
	// Parse any command line arguments.  If an invalid argument is passed, 
	// print the usage
	while((option_char = getopt(argc, argv, "o:n:b:g:h")) != -1){
		switch(option_char){
			case 'o': 
				dst_file_name = optarg;
				break; 
			case 'n': 
				n_events = atoi(optarg); 
				break; 
			case 'b':
				b_field = atof(optarg);
				break;	
			case 'g':
				do_gbl = true;
				break;	
			case 'h': 
				printUsage(); 
				return EXIT_SUCCESS; 	
			default: 
				printUsage(); 
				return EXIT_FAILURE; 
		}
	}

	// If an LCIO file is not specified, exit the program
	if(argc-optind==0){
		cerr << "Please specify at least one LCIO file to process."
			<< "\nUse the -h flag for usage" << endl;
		return EXIT_FAILURE; 	
	}

	// If a DST file name is not specified, set a default name
	if(dst_file_name.length() == 0){
		cout << "Setting DST name to default value: hps_dst.root" << endl;
		dst_file_name = "hps_dst.root"; 
	}

	// If a B-field isn't set, exit the program
	if(isnan(b_field)){ 
		cerr << "Please specify the magnetic field strength in Tesla."
			<< "\nUse the -h flag for usage" << endl;
		return EXIT_FAILURE;
	}

	// Open a ROOT file
	TFile* root_file = new TFile(dst_file_name.c_str(), "RECREATE");

	// Create a ROOT tree along with the HPS Event branch which
	// will encapsulate all event information
	TTree *tree = new TTree("HPS_Event", "HPS event tree"); 
	HpsEvent* hps_event = new HpsEvent();  
	tree->Branch("Event", "HpsEvent", &hps_event, 32000, 3); 

	// Create the LCIO file reader
	IO::LCReader* lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader(); 

	EVENT::LCEvent* event = NULL;
	HpsEventBuilder* event_builder = new HpsEventBuilder(); 	
	event_builder->setBField(b_field); 
	event_builder->setGblFlag(do_gbl);
	int event_number = 0;
	clock_t initial_event_time;
	clock_t total_time = 0;

	while (optind<argc)
	{
		lc_reader->open(argv[optind]);

		// Loop over all events in the LCIO file
		while((event = lc_reader->readNextEvent()) != 0){

			initial_event_time = clock();

			++event_number; 
			if((event_number - 1) == n_events) break; 

			// Print the event number every 1000 events
			if(event_number%1000 == 0 ){
				cout << "Processing event number: " << event_number << endl;
			}

			event_builder->makeHpsEvent(event, hps_event); 
			tree->Fill();

			total_time += clock() - initial_event_time;
		}	

		lc_reader->close();
		optind++;
	}

	cout << "Finished writing " << event_number << " events to ROOT Tree!" << endl;
	root_file->Write();
	root_file->Close();

	delete hps_event;
	delete event_builder;

	cout << "Total run time: " << ((float) (clock() - initial_time))/CLOCKS_PER_SEC
		<< " s" << endl;
	cout << "Average time/event: " << (((float) total_time)/CLOCKS_PER_SEC)/event_number
		<< " s" << endl;

	return EXIT_SUCCESS;
}

void printUsage()
{
	cout << "Usage: dst_maker [OPTIONS] LCIO_INPUT_FILE [additional input files]" << endl;
	cout << "An LCIO_INPUT_FILE must be specified.\n" << endl;
	cout << "OPTIONS:\n "
		<< "\t -o Output ROOT file name \n"
		<< "\t -n The number of events to process \n"
		<< "\t -b The strength of the magnetic field in Tesla \n"
		<< "\t -g Run GBL track fit \n"
		<< "\t -h Display this help and exit \n"
		<< endl;
}	
