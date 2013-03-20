/**
 * 	@section purpose:
 *		Looks at two track events and creates some simple plots.
 *
 *  @author: 	Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *  @date: March 14, 2013
 *  @version: v1.0
 */				

//--- C++ ---//
#include <iostream>
#include <cmath>

//--- ROOT ---//
#include <TH1F.h>
#include <TCanvas.h>


//--- LCIO ---//
#include <IO/LCReader.h>
#include <IOIMPL/LCFactory.h>
#include <EVENT/LCEvent.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/LCCollectionVec.h>

using namespace std; 

void printUsage();
void setup1DHistogram(TH1F* histo, string x_axis_title);

int main( int argc, char **argv)
{
	string trackCollectionName = "MatchedTracks"; 
    string lcio_file_name = "";
    string pdf_file_name = "default.pdf";
	int option_char;

	// Parse any command line arguments. If there are no valid command line
	// arguments given, print the usage.
    while((option_char = getopt(argc, argv, "i:p:")) != -1){
		switch(option_char){
			case 'i':
				lcio_file_name = optarg;
				break;
			case 'o':
				pdf_file_name = optarg;
				break;
			default:
				printUsage(); 
				return(2);  
		}
	}

    // If an lcio input file was not specified, exit gracefully
    if(lcio_file_name.length() == 0){
    	cout << "\nPlease specify an LCIO file to process.\n Use the -h flag "
    		 <<	"for usage\n" << endl;
    	return(2);
    }

    const double param = 2.99792458e-04;
    const float b_field = -0.491;

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

	// Create the LCIO reader and open the LCIO file
	// TODO: handle IO exception correctly
	IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader(); 
	lc_reader->open(lcio_file_name.c_str());
	
	EVENT::LCEvent *event  = 0;
    IMPL::TrackImpl* track = 0;
    IMPL::LCCollectionVec* tracks = 0;
	double pt, px, py, pz, p;
	int eventNumber = 0;

	//--- Analysis ---//
	//----------------//

	// Loop over all events in the file
	while( (event = lc_reader->readNextEvent()) ){
		++eventNumber;
		// Print the event number every 500 events
		if(eventNumber%500 == 0){
			cout << "Event: " << eventNumber << endl;
		}

		// TODO: Check that the event has the collection of tracks ("MatchedTracks").
		// Get the collection of tracks from the event
        tracks = (IMPL::LCCollectionVec*) event->getCollection(trackCollectionName);
		
		// Check that the event contains only two tracks, if it doesn't skip the event
		if(tracks->getNumberOfElements() != 2) continue;

		// Loop over all tracks in the event (Should use an iterator instead)
        for(int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n){ 
				
            // Get a track from the LCIO collection
            track = (IMPL::TrackImpl*) tracks->getElementAt(track_n);
		
			// Calculate the transverse momentum of the track
			pt = abs((1/track->getOmega())*b_field*param);
			cout << "Track pt: " << pt << endl;

			// Calculate the momentum components
			px = pt*cos(track->getPhi());
			py = pt*sin(track->getPhi());
			pz = pt*track->getTanLambda();
			cout << "Track px: " << px << " Track py: " << py
				 << " Track pz: " << pz << endl;

			// Calculate the momentum of the track
			p = sqrt(px*px + py*py + pz*pz);
			cout << "Track momentum: " << p << endl;

			// Fil the plots
			h_pt->Fill(pt);
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

	return 0;
}

void printUsage()
{
	cout << "Usage: TwoTrackAnalysis_example [options]\nOptions:\n"
		 << "\t-i <Input LCIO file name> \n"
		 << "\t-p <Output pdf file name> \n"
		 << endl;
}

void setup1DHistogram(TH1F *histo, string x_axis_title){

	histo->SetStats(0);
	histo->GetXaxis()->SetTitle(x_axis_title.c_str());
	histo->GetXaxis()->SetTitleSize(0.03);
	histo->GetXaxis()->SetLabelSize(0.03);
	histo->GetYaxis()->SetTitleSize(0.03);
	histo->GetYaxis()->SetLabelSize(0.03);

}

