
/*
 *  write_hps_tree.cxx
 *      author: Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *
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
    tree->Branch("d0",				&d0				);
    tree->Branch("phi",				&phi			);
    tree->Branch("omega",			&omega			);
    tree->Branch("tan_lambda",		&tan_lambda		);
    tree->Branch("z0",				&z0				);
    tree->Branch("px",				&px				);	
    tree->Branch("py",				&py				);	
    tree->Branch("pz",				&pz				);	
    tree->Branch("trk_chi2",		&trk_chi2		);
    tree->Branch("trk_ndf",			&trk_ndf		);
    tree->Branch("cluster_e",       &cluster_e      );
    tree->Branch("cluster_x",       &cluster_x      );
    tree->Branch("cluster_y",       &cluster_y      );
    tree->Branch("cluster_z",       &cluster_z      );
    tree->Branch("cluster_nhits",   &cluster_nhits  );

    // Create the LCIO reader
    IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader();
    lc_reader->open(lcio_file_name.c_str());

    IMPL::TrackImpl* track;
    IMPL::LCCollectionVec* tracks;
    EVENT::LCEvent* event;
    IMPL::ClusterImpl* cluster;
    IMPL::LCCollectionVec* clusters;

    while( (event = lc_reader->readNextEvent()) != 0 ){

        if(dump_event){
            UTIL::LCTOOLS::dumpEvent(event);
            exit(0);
        }

        event_number = (int) event->getEventNumber();
        run_number	 = (int) event->getRunNumber();	

        // Clear the vectors of their previously stored data
        d0.clear();
        phi.clear();
        omega.clear();
        tan_lambda.clear();
        z0.clear();
        px.clear();
        py.clear();
        pz.clear();
        trk_chi2.clear();
        trk_ndf.clear();
        cluster_e.clear(); 
        cluster_x.clear(); 
        cluster_y.clear(); 
        cluster_z.clear(); 
        cluster_nhits.clear(); 

        // Get the collection of tracks from the event	
        tracks = (IMPL::LCCollectionVec*) event->getCollection("MatchedTracks");

        n_tracks = (int) tracks->getNumberOfElements();

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
