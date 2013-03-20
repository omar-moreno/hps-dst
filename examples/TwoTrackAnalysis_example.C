/**
 * 	@section purpose:
 *		Looks at two track events and creates some simple plots
 *
 *  @author: 	Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *  @date: March 19, 2013
 *  @version: v1.0
 */	
 
 
void runTwoTrackAnalysis(std::string root_file_name,
							std::string pdf_file_name){
	
    const double param = 2.99792458e-04;
    const float b_field = -0.491;

	// Check if the classes (HpsEvent, Track, EcalCluster, ...) are in 
    // the dictionary.  If not, load their definitions from libHpsEvent.so
 	if(!TClassTable::GetDict("HpsEvent")){
 		std::cout << "Class definitions were not found! Loading libHpsEvent.so"
 				  << std::endl;
 		gSystem->Load("../lib/libHpsEvent.so");
 	}
 	
 	//-- Setup ROOT histograms ---//
 	//----------------------------//

 	    // Create a canvas and set its characteristics
 	    TCanvas *canvas = new TCanvas("canvas", "Track Momentum", 700, 700);
 	    canvas->SetFillColor(0);
 	    canvas->SetBorderMode(0);
 	    canvas->SetBorderSize(0);
 	    canvas->SetFrameFillColor(0);
 	    canvas->SetFrameBorderMode(0);

 	    // Create histograms and set its characteristics
 		TH1F *h_pt  = new TH1F("h_pt", "Transverse Momentum - All Tracks", 100, 0, 5.5);
 		setup1DHistogram(h_pt, "Transverse Momentum [GeV]");
 		TH1F *h_p   = new TH1F("h_p",  "Momentum - All Tracks", 100, 0, 5.5);
 		setup1DHistogram(h_p, "Momentum [GeV]");
 		TH1F *h_px  = new TH1F("h_px", "p_{x} - All Tracks", 100, 0, 5.5);
 		setup1DHistogram(h_px, "p_{x} [GeV]");
 		TH1F *h_py  = new TH1F("h_py", "p_{y} - All Tracks", 50, 0, 1);
 		setup1DHistogram(h_py, "p_{y} Momentum [GeV]");
 		TH1F *h_pz  = new TH1F("h_pz", "p_{z} - All Tracks", 50, 0, 1);
 		setup1DHistogram(h_pz, "p_{z} Momentum [GeV]");

 		//-----------------------------//


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

    SvtTrack *track = 0;
	double pt, px, py, pz, p;
    // Loop over all events
    for(int entry = 0; entry < tree->GetEntries(); ++entry){
		// Print the event number every 500 events
    	if((entry+1)%500 == 0){
    		std::cout << "Event: " << entry+1 << endl;
    	}

        // Read the ith entry from the tree.  This "fills" HpsEvent and allows
        // access to all collections
        tree->GetEntry(entry);

        // If the event does not contain two tracks, skip it
        if(hps_event->getNumberOfTracks() != 2) continue;

		// Loop over all tracks in the event
        for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){

        	// Get the track from the event
        	track = hps_event->getTrack(track_n);

        	// Calculate momentum
        	px = track->getPx();
        	py = track->getPy();
        	pz = track->getPz();
			std::cout << "Track px: " << px << " Track py: " << py
				 << " Track pz: " << pz << std::endl;

			p = sqrt(px*px + py*py + pz*pz);
			cout << "Track momentum: " << p << endl;

        	// Fill the plots
			h_p->Fill(p);
			h_px->Fill(px);
			h_py->Fill(py);
			h_pz->Fill(pz);
        }
    }

	// Save all plots to a single pdf file
	h_pt->Draw("");
	canvas->Print( (pdf_file_name + "(").c_str());
	h_p->Draw("");
	canvas->Print( (pdf_file_name + "(").c_str());
	h_px->Draw("");
	canvas->Print( (pdf_file_name + "(").c_str());
	h_py->Draw("");
	canvas->Print( (pdf_file_name + "(").c_str());
	h_pz->Draw("");
	canvas->Print( (pdf_file_name + ")").c_str());

}

void setup1DHistogram(TH1F *histo, string x_axis_title){

	histo->SetStats(0);
	histo->GetXaxis()->SetTitle(x_axis_title.c_str());
	histo->GetXaxis()->SetTitleSize(0.03);
	histo->GetXaxis()->SetLabelSize(0.03);
	histo->GetYaxis()->SetTitleSize(0.03);
	histo->GetYaxis()->SetLabelSize(0.03);

}
 
