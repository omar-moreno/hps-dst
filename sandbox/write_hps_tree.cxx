
/*
 *  write_hps_tree.cxx
 *      author: Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *     created: February 19, 2013
 *	   purpose:
 *				Write a simple ROOT Tree which contains information
 *				about tracks, hit and Ecal clusters.
 */				

//--- C++ ---//
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

//--- ROOT ---//
#include <TFile.h>
#include <TTree.h>

//--- LCIO ---//
#include <IO/LCReader.h>
#include <IOIMPL/LCFactory.h>
#include <EVENT/LCEvent.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/ClusterImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <UTIL/LCTOOLS.h>

using namespace std; 

void printUsage();
int getHitLayer(const double *position);

int main(int argc, char **argv)
{

	string root_file_name;
	string lcio_file_name;
	float b_field = -0.491; 
	bool dump_event = false;
	int option_char;

	while((option_char = getopt(argc, argv, "i:o:b:d")) != -1){
		switch(option_char) { 
			case 'i':
				lcio_file_name = optarg;  
				break;
			case 'o':
				root_file_name = optarg;
				break;
			case 'b':
				b_field = atof(optarg);
				break;
			case 'd':
				dump_event = true;
				break;
			case 'h':
				printUsage();
				exit(1);
			default:
				printUsage();
				exit(1);
		}
	}

	double pt; 
	const double param = 2.99792458e-04; 

	if(lcio_file_name.length() == 0){ 
		cout << "Please specify a LCIO file to process.\n Use the -h flag for usage" << endl;
		exit(1);
	} else if(root_file_name.length() == 0){
		root_file_name = "default.root";
	}

	cout << "Output will be written to " << root_file_name << endl;


	// Create a ROOT file	
	TFile *root_file = new TFile(root_file_name.c_str(), "RECREATE");


	//-- Parameters that will be stored in the tree --//
	//------------------------------------------------//

	int event_number = 0;		// Event number
	int run_number = 0;			// Run number
	int n_tracks = 0;			// Number of tracks 
	int n_clusters = 0; 

	vector< double > d0;			// Impact parameter of the track in (r-phi)
	vector< double > phi;			// Phi of the track at reference point
	vector< double > omega;			// Signed curvature of the track in [1/mm]
	vector< double > tan_lambda;	// Dip angle of the track in r-z
	vector< double > z0;			// Impact parameter of the track in r-z
	vector< double > px;			// x component of track momentum
	vector< double > py;			// y component of track momentum
	vector< double > pz;			// z component of track momentum
	vector< double > trk_chi2;		// Chi^2 of the track fit
	vector< int >    trk_ndf;		// Number of degrees of freedom of the track fit

	vector< vector< double > >  hit_layer;  // The SVT layer (1-5) the hit corresponds to 
	vector< vector< double > >  hit_x;      // The uncorrected x position of the hit
	vector< vector< double > >  hit_y;      // The uncorrected y position of the hit
	vector< vector< double > >  hit_z;      // The uncorrected z position of the hit
	vector< vector< double > >  hit_x_err;  // The uncorrected error on the x position
	vector< vector< double > >  hit_y_err;  // The uncorrected error on the y position
	vector< vector< double > >  hit_z_err;  // The uncorrected error on the z position

	vector< double > cluster_e;     // Ecal cluster energy
	vector< double > cluster_x;     // Ecal cluster x position
	vector< double > cluster_y;     // Ecal cluster y position
	vector< double > cluster_z;     // Ecal cluster z position
	vector< int >    cluster_nhits; // Ecal cluster hit count


	// Create a ROOT tree
	TTree *tree	= new TTree("HPS", "HPS event tree");
	tree->Branch("event_number",	&event_number,	"event_number/I"	);
	tree->Branch("run_number",		&run_number,	"run_number/I"		);
	tree->Branch("n_tracks",		&n_tracks,		"n_tracks/I"		);
	tree->Branch("n_clusters",		&n_clusters,	"n_clusters/I"		);
	tree->Branch("d0",				"vector<double>",   &d0				);
	tree->Branch("phi",				"vector<double>",   &phi			);
	tree->Branch("omega",			"vector<double>",   &omega			);
	tree->Branch("tan_lambda",		"vector<double>",   &tan_lambda		);
	tree->Branch("z0",				"vector<double>",   &z0				);
	tree->Branch("px",				"vector<double>",   &px				);	
	tree->Branch("py",				"vector<double>",   &py				);	
	tree->Branch("pz",				"vector<double>",   &pz				);	
	tree->Branch("trk_chi2",		"vector<double>",   &trk_chi2		);
	tree->Branch("trk_ndf",			"vector<int>",      &trk_ndf		); 
	tree->Branch("hit_layer",		"vector< vector<double> >", &hit_layer );
	tree->Branch("hit_x",			"vector< vector<double> >", &hit_x	   );
	tree->Branch("hit_y",			"vector< vector<double> >", &hit_y	   );
	tree->Branch("hit_z",			"vector< vector<double> >", &hit_z	   );
	tree->Branch("hit_x_err",		"vector< vector<double> >", &hit_x_err );
	tree->Branch("hit_y_err",		"vector< vector<double> >", &hit_y_err );
	tree->Branch("hit_z_err",		"vector< vector<double> >", &hit_z_err );
	tree->Branch("cluster_e",       "vector<double>",   &cluster_e      );
	tree->Branch("cluster_x",       "vector<double>",   &cluster_x      );
	tree->Branch("cluster_y",       "vector<double>",   &cluster_y      );
	tree->Branch("cluster_z",       "vector<double>",   &cluster_z      );
	tree->Branch("cluster_nhits",   "vector<int>",      &cluster_nhits  );


	// Create the LCIO reader
	IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader();
	lc_reader->open(lcio_file_name.c_str());

	IMPL::TrackImpl* track;
	IMPL::LCCollectionVec* tracks;
	EVENT::LCEvent* event;
	IMPL::ClusterImpl* cluster;
	IMPL::LCCollectionVec* clusters;
	EVENT::TrackerHitVec hits;
	int n_hits = 0; 
	while( (event = lc_reader->readNextEvent()) != 0 ){

		if(dump_event){
			UTIL::LCTOOLS::dumpEvent(event);
			exit(0);
		}

		event_number = (int) event->getEventNumber();
		run_number	 = (int) event->getRunNumber();	

		// Clear the vectors of their previously stored data
		d0.clear(); phi.clear(); omega.clear(); tan_lambda.clear(); z0.clear();
		px.clear(); py.clear(); pz.clear(); trk_chi2.clear(); trk_ndf.clear();
		cluster_e.clear(); cluster_x.clear(); cluster_y.clear(); 
		cluster_z.clear(); cluster_nhits.clear(); hit_layer.clear(); 
		hit_x.clear(); hit_y.clear(); hit_z.clear(); hit_x_err.clear(); 
		hit_y_err.clear(); hit_z_err.clear();

		// Get the collection of tracks from the event	
		tracks = (IMPL::LCCollectionVec*) event->getCollection("MatchedTracks");

		n_tracks = (int) tracks->getNumberOfElements();

		// Resize the hit data vectors to the total number of tracks
		hit_layer.resize(n_tracks);
		hit_x.resize(n_tracks); 	
		hit_y.resize(n_tracks); 	
		hit_z.resize(n_tracks); 	
		hit_x_err.resize(n_tracks); 	
		hit_y_err.resize(n_tracks); 	
		hit_z_err.resize(n_tracks); 	

		// Loop over the tracks and fill the tree
		for(int i = 0; i < n_tracks; i++){
			track = (IMPL::TrackImpl*) tracks->getElementAt(i); 

			// Fill track parameters
			d0.push_back(track->getD0());
			phi.push_back(track->getPhi());
			omega.push_back(track->getOmega());
			tan_lambda.push_back(track->getTanLambda());
			z0.push_back(track->getZ0());
			trk_chi2.push_back(track->getChi2());
			trk_ndf.push_back(track->getNdf());

			pt = abs((1/track->getOmega())*b_field*param);
			px.push_back(pt*cos(track->getPhi()));
			py.push_back(pt*sin(track->getPhi()));
			pz.push_back(pt*track->getTanLambda());

			hits = track->getTrackerHits();
			n_hits = (int) hits.size();

			// Resize the hit data vectors to the total number of hits per track
			hit_layer[i].resize(n_hits);
			hit_x[i].resize(n_hits); 	
			hit_y[i].resize(n_hits); 	
			hit_z[i].resize(n_hits); 	
			hit_x_err[i].resize(n_hits); 	
			hit_y_err[i].resize(n_hits); 	
			hit_z_err[i].resize(n_hits); 		

			for(int hit_n = 0; hit_n < n_hits; ++hit_n){
				
				hit_layer[i][hit_n] = getHitLayer(hits[hit_n]->getPosition());
				
				hit_x[i][hit_n] = hits[hit_n]->getPosition()[0];
				hit_y[i][hit_n] = hits[hit_n]->getPosition()[1];
				hit_z[i][hit_n] = hits[hit_n]->getPosition()[2];

				hit_x_err[i][hit_n] = sqrt(hits[hit_n]->getCovMatrix()[0]);
				hit_y_err[i][hit_n] = sqrt(hits[hit_n]->getCovMatrix()[2]);
				hit_z_err[i][hit_n] = sqrt(hits[hit_n]->getCovMatrix()[5]);

			}

			n_hits = 0; 
		
		}

		// Get the collection of Ecal clusters from the event 
		clusters = (IMPL::LCCollectionVec*) event->getCollection("EcalClusters");

		n_clusters = (int) clusters->getNumberOfElements(); 

		// Loop over the clusters and fill the tree
		for(int cluster_n = 0; cluster_n < n_clusters; ++cluster_n){
			cluster = (IMPL::ClusterImpl*) clusters->getElementAt(cluster_n);

			// Fill cluster parameters
			cluster_e.push_back(cluster->getEnergy());
			cluster_x.push_back(cluster->getPosition()[0]);
			cluster_y.push_back(cluster->getPosition()[1]);
			cluster_z.push_back(cluster->getPosition()[2]);
			cluster_nhits.push_back(cluster->getCalorimeterHits().size());

		}

		tree->Fill();
		n_tracks = 0;
		pt = 0; 
	}

	lc_reader->close();
	root_file->Write();
	root_file->Close();

	cout << "Finished writing ROOT Tree!" << endl;

	return 0; 
}

void printUsage()
{
	cout << "Usage: write_hps_tree [options]\nOptions:\n "
		<< "\t-i <Input LCIO file name> \n"
		<< "\t-o <Output ROOT file name> \n" 
		<< "\t-b <Magnetic field strength> [T] \n"
		<< "\t-d Dump the event structure and exit\n" 
		<< "\t-h Show usage\n" << endl;
}

int getHitLayer(const double *position){
	
	if(position[0] < 110) return 1;
	if(position[0] > 250 && position[0] < 310) return 2;
	if(position[0] > 450 && position[0] < 510) return 3;
	if(position[0] > 650 && position[0] < 710) return 4;
	if(position[0] > 850 && position[0] < 910) return 5;
	return 0; 

}

