/**
 * @file   dst_maker.cxx
 * @brief  Creates an HPS Data Summary Tape
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date   December 20, 2013
 */

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <limits>
#include <getopt.h>
#include <unistd.h>

//------------//
//--- LCIO ---//
//------------//
#include <IO/LCReader.h>
#include <IOIMPL/LCFactory.h>
#include <EVENT/LCEvent.h>

//------------//
//--- ROOT ---//
//------------//
#include <TTree.h>
#include <TFile.h>

//-----------//
//--- DST ---//
//-----------//
#include "HpsEventBuilder.h"

//-----------------//
//--- HPS Event ---//
//-----------------//
#include "HpsEvent.h"

using namespace std;

void printUsage(); 

int main(int argc, char **argv) {

    clock_t initial_time = clock();

    string dst_file_name = "";  
    int total_events = kMaxInt;  
    bool ecal_only = false;
    // Parse any command line arguments.  If there are no valid command line 
    // arguments passed, print the usage and exit.  
    static struct option long_options[] = { 
        {"output",        required_argument, 0, 'o' },
        {"total_events",  required_argument, 0, 'n' },
        {"ecal_only",     no_argument,       0, 'e' },
        {"help",          no_argument,       0, 'h' },
        {0, 0, 0, 0}
    };
    int option_index = 0; 
    int option_char = 0; 
    while ((option_char = getopt_long(argc, argv, "o:n:eh", long_options, &option_index)) != -1) {
        switch (option_char) {
            case 'o': 
                dst_file_name = optarg;
                break; 
            case 'n': 
                total_events = atoi(optarg); 
                break; 
            case 'e':
                ecal_only = true;
                break;    
            case 'h': 
                printUsage(); 
                return EXIT_SUCCESS;    
            default: 
                printUsage(); 
                return EXIT_FAILURE; 
        }
    }

    // If an LCIO file is not specified, warn the user and exit the application
    if (argc-optind==0) {
        cerr << "[ DST MAKER ]: Please specify at least one LCIO file to process." << endl;
        cerr << "[ DST MAKER ]: Use the --help flag for usage" << endl;
        return EXIT_FAILURE;    
    }

    // If a DST file name is not specified, set it to one matching the first input 
    // file
    if (dst_file_name.empty()) {
        dst_file_name =  argv[optind];
        dst_file_name.replace(dst_file_name.begin() + dst_file_name.find(".slcio"),
                              dst_file_name.end(), ".root");
    }
    cout << "[ DST MAKER ]: Setting DST file name to " << dst_file_name << endl;

    // Open a ROOT file
    TFile* root_file = new TFile(dst_file_name.c_str(), "RECREATE");

    // Instantiate a ROOT tree along with the HPS Event branch which will 
    // encapsulate all event information
    TTree *tree = new TTree("HPS_Event", "HPS event tree"); 
    HpsEvent* hps_event = new HpsEvent();  
    tree->Branch("Event", "HpsEvent", &hps_event, 32000, 3); 

    // Instantiate the LCIO file reader
    IO::LCReader* lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader(); 

    // Instantiate the event builder which will be used to create HpsEvent
    // objects
    HpsEventBuilder* event_builder = new HpsEventBuilder(); 
    // Set whether the event builder should only write Ecal data
    event_builder->writeEcalOnly(ecal_only);    
    
    EVENT::LCEvent* event = NULL;
    int event_number = 0;
    // Loop over all of the input LCIO files
    while (optind<argc) {

        // Open the LCIO file.  If it can't be opened, throw an exception.
        lc_reader->open(argv[optind]);

        // Loop over all events in the LCIO file
        while ((event = lc_reader->readNextEvent()) != 0 && event_number < total_events ) {

            Int_t ObjectNumber = TProcessID::GetObjectCount();
          
            ++event_number;

            // Print the event number every 1000 events
            if (event_number%1000 == 0 ) {
                cout << "[ DST MAKER ]: Processing event number: " << event_number << endl;
            }

            // Create an HPS Event object 
            event_builder->makeHpsEvent(event, hps_event);
          
            // Write the HPS Event object to the ROOT tree
            tree->Fill();

            //Reset object count so we don't run out of TRef IDs
            TProcessID::SetObjectCount(ObjectNumber);
        }   

        // Close the LCIO file that was being processed
        lc_reader->close();

        // Increase the file index
        optind++;
    }

    cout << "Finished writing " << event_number << " events to ROOT Tree!" << endl;
    root_file->Write();
  
    // Clean up!  
    delete tree;  
    delete root_file; 
    delete hps_event;
    delete event_builder;

    cout << "Total run time: " << ((float) (clock() - initial_time))/CLOCKS_PER_SEC << " s" << endl;

    return EXIT_SUCCESS;
}

void printUsage() {
    cout << "Usage: dst_maker [OPTIONS] LCIO_INPUT_FILE [additional input files]" << endl;
    cout << "An LCIO_INPUT_FILE must be specified.\n" << endl;
    cout << "OPTIONS:\n "
         << "\t -o Output ROOT file name \n"
         << "\t -n The number of events to process \n"
         << "\t -b The strength of the magnetic field in Tesla \n"
         << "\t -g Run GBL track fit \n"
         << "\t -e Make a DST with Ecal data only \n"
         << "\t -h Display this help and exit \n"
         << endl;
}   
