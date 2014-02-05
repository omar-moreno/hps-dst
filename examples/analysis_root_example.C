/**
 * 	@section purpose:
 *		A simple analysis demonstrating the use of a DST to make simple plots
 *		of Ecal and SVT physics objects
 *
 *  @author: 	Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *  @date: March 19, 2013
 *  @version: v1.0
 */	
 
#include <cstdlib>
 
void runAnalysis(std::string root_file_name, std::string pdf_file_name){
	
    const double param = 2.99792458e-04;

    // Check if the path to hps-dst has been set
	if(getenv("HPS_DST_HOME") == NULL){
		std::err << "Error! Environmental variable HPS_DST_HOME is not set."
				  << "\nExiting ..." << std::endl;
		return EXIT_FAILURE;
	} 

	std::string hps_dst_path(getenv("HPS_DST_HOME"));
	hps_dst_path += "/build/lib/libHpsEvent.so";
	// Check if the classes (HpsEvent, Track, EcalCluster, ...) are in 
	// the dictionary.  If not, load their definitions from libHpsEvent.so
	if(!TClassTable::GetDict("HpsEvent")){
		std::cout << "Class definitions were not found! Loading libHpsEvent.so"
			      << std::endl;
		gSystem->Load(hps_dst_path.c_str());
	}

 	//-- Setup ROOT histograms ---//
 	//----------------------------//

	// Create a canvas and set its characteristics
 	TCanvas *canvas = new TCanvas("canvas", "Track Momentum", 700, 700);
 	setupCanvas(canvas);

 	// Ecal
 	TH2F* h_hit_pos = new TH2F("h_hit_pos", "Ecal Hit Positions", 47, -23, 24, 12, -6, 6);
 	setup2DHistogram(h_hit_pos, "Ecal Hit Index - x", "Ecal Hit Index - y");
 	TH1F* h_cluster_energy = new TH1F("h_cluster_energy", "Ecal Cluster Energy", 100, 0, 5.5);
 	setup1DHistogram(h_cluster_energy, "Ecal Cluster Energy [GeV]");

 	// Tracking
 	TH1F *h_p   = new TH1F("h_p",  "Momentum - All Tracks", 100, 0, 5.5);
 	setup1DHistogram(h_p, "Momentum [GeV]");
 	TH1F *h_px  = new TH1F("h_px", "p_{x} - All Tracks", 100, 0, 5.5);
 	setup1DHistogram(h_px, "p_{x} [GeV]");
 	TH1F *h_py  = new TH1F("h_py", "p_{y} - All Tracks", 40, -.2, .2);
 	setup1DHistogram(h_py, "p_{y} Momentum [GeV]");
 	TH1F *h_pz  = new TH1F("h_pz", "p_{z} - All Tracks", 40, -.2, .2);
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
    EcalCluster* ecal_cluster = 0;
    EcalHit* ecal_hit = 0;
	double pt, px, py, pz, p;
	double cluster_energy;
	int index_x, index_y;

	//--- Analysis ---//
	//----------------//

	// Loop over all events
    for(int entry = 0; entry < tree->GetEntries(); ++entry){

    	// Print the event number every 500 events
    	if((entry+1)%500 == 0){
    		std::cout << "Event: " << entry+1 << endl;
    	}

        // Read the ith entry from the tree.  This "fills" HpsEvent and allows
        // access to all collections
        tree->GetEntry(entry);

        // Loop over all of the Ecal clusters in the event
        for(int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n){

        	// Get an Ecal cluster from the event
        	ecal_cluster = hps_event->getEcalCluster(cluster_n);

        	// Get the Ecal cluster energy
        	cluster_energy = ecal_cluster->getEnergy();

        	// Fill the cluster energy plot
        	h_cluster_energy->Fill(cluster_energy);

        	// Get the Ecal hits used to create the cluster
        	TRefArray* ecal_hits = ecal_cluster->getEcalHits();

        	// Loop through all of the Ecal hits and plot their positions
        	for(int hit_n = 0; hit_n < ecal_hits->GetEntries(); ++hit_n){

        		// Get an Ecal hit from the cluster
        		ecal_hit = (EcalHit*) ecal_hits->At(hit_n);

        		// Get the crystal index of the ecal hit
        		index_x = ecal_hit->getXCrystalIndex();
        		index_y = ecal_hit->getYCrystalIndex();

        		// Fill the Ecal hit position plot
        		h_hit_pos->Fill(index_x, index_y, 1);

        	}
        }

		// Loop over all tracks in the event
        for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){

        	// Get a track from the event
        	track = hps_event->getTrack(track_n);

        	// Calculate momentum
        	px = track->getPx();
        	py = track->getPy();
        	pz = track->getPz();
			p = sqrt(px*px + py*py + pz*pz);
        	
            // Fill the plots
			h_p->Fill(p);
			h_px->Fill(px);
			h_py->Fill(py);
			h_pz->Fill(pz);
        }
    }

	// Save all plots to a single pdf file
	h_hit_pos->Draw("colz");
	canvas->Print( (pdf_file_name + "(").c_str());
	h_cluster_energy->Draw("");
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
//--- Functions ---//
//-----------------//

// Note: These functions will be placed in utility classes included with the
//	     DST at some point.

void setupCanvas(TCanvas* canvas){

	canvas->SetFillColor(0);
 	canvas->SetBorderMode(0);
 	canvas->SetBorderSize(0);
 	canvas->SetFrameFillColor(0);
 	canvas->SetFrameBorderMode(0);

}

void setup1DHistogram(TH1 *histo, string x_axis_title){

	histo->SetStats(0);
	histo->GetXaxis()->SetTitle(x_axis_title.c_str());
	histo->GetXaxis()->SetTitleSize(0.03);
	histo->GetXaxis()->SetLabelSize(0.03);
	histo->GetYaxis()->SetTitleSize(0.03);
	histo->GetYaxis()->SetLabelSize(0.03);

}

void setup2DHistogram(TH1* histo, string x_axis_title, string y_axis_title){

	histo->GetYaxis()->SetTitle(y_axis_title.c_str());
	setup1DHistogram(histo, x_axis_title);
}
 
//---------------------//
