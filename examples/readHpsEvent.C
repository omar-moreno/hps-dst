/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

void readHpsEvent(std::string root_file)
{

    // Check if the classes (HpsEvent, Track, EcalCluster, ...) are in 
    // the dictionary.  If not, load their definitions in libHpsEvent.so
    //if(!TClassTable::GetDict("HpsEvent")){
    //    std::cout << "Class definitions were not found! Loading HpsEvent.so"
    //              << std::endl;
    //    gSystem->Load("../lib/HpsEvent.so");
    //}

    // Open the ROOT file 
 /*   TFile *file = new TFile(root_file.c_str());

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
        
        // Read the ith entry from the tree.  This "fills" HpsEvent and allows
        // access to all collections
        tree->GetEntry(entry); 

        // If the event does not contain two tracks, skip it
        if(hps_event->getNumberOfTracks() != 2) continue; 

        // Get the collection of track objects from the event
    } */
}
