
#include <iostream>
#include <stdlib.h>

void runMinDstAnalysis(std::string root_file_name)
{
	if(getenv("HPS_DST_HOME") == NULL){
		std::cout << "Error! Variable HPS_DST_PATH is not set." << std::endl;
		return;
	} 

	std::string hps_dst_path(getenv("HPS_DST_HOME"));
	hps_dst_path += "/lib/libHpsEvent.so";
	// Check if the classes (HpsEvent, Track, EcalCluster, ...) are in 
	// the dictionary.  If not, load their definitions from libHpsEvent.so
	if(!TClassTable::GetDict("HpsEvent")){
		std::cout << "Class definitions were not found! Loading libHpsEvent.so"
			      << std::endl;
		gSystem->Load(hps_dst_path.c_str());
	}

	// Open the ROOT file
	TFile *file = new TFile(root_file_name.c_str());

	// Get the TTree "HPS_EVENT" containing the HpsEvent branch and all
	// other collections
	TTree *tree = (TTree*) file->Get("HPS_Event");

	// Create a pointer to an HpsEvent object in order to read the TClonesArrays
	// collections
	HpsEvent *hps_event = new HpsEvent();

	// Get the HpsEvent branch from the TTree and set the branch address to
	// the pointer created above
	TBranch *b_hps_event = tree->GetBranch("Event");
	b_hps_event->SetAddress(&hps_event);


	// Loop over all events
	for(int entry = 0; entry < tree->GetEntries(); ++entry){
		// Print the event number every 500 events
		if((entry+1)%500 == 0){
			std::cout << "Event: " << entry+1 << endl;
		}

		// Read the ith entry from the tree.  This "fills" HpsEvent and allows
		// access to all collections
		tree->GetEntry(entry);

		// Loop over all tracks in the event.  The number of tracks, clusters, etc. 
		// can be obtained from the event itself via getter methods 
		// e.g. getNumberOfTracks, getNumberOfClusters
		for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){

			// Get the nth tracks from the event. Similar methods exist for the other collections
			track = hps_event->getTrack(track_n);

		}
	}

	std::cout << "Analysis Complete!" << std::endl;
}
