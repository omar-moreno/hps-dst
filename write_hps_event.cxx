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

//---//
#include <HpsEvent.h>
#include <Track.h>
#include <SvtHit.h>

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

    HpsEvent *hps_event = new HpsEvent(); 
    Track *hps_track = NULL;  
    SvtHit *svt_hit = NULL; 

    // Create a ROOT tree 
    TTree *tree = new TTree("HPS_Event", "HPS event tree"); 
    tree->Branch("Event", "HpsEvent", &hps_event, 32000, 3); 

    // Create the LCIO reader
    IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader();
    lc_reader->open(lcio_file_name.c_str());

    IMPL::TrackImpl* track;
    IMPL::LCCollectionVec* tracks;
    EVENT::LCEvent* event;
    IMPL::ClusterImpl* cluster;
    IMPL::LCCollectionVec* clusters;
    EVENT::TrackerHitVec hits;
    while( (event = lc_reader->readNextEvent()) != 0 ){

        hps_event->Clear(); 

        if(dump_event){
            UTIL::LCTOOLS::dumpEvent(event);
            exit(0);
        }

        cout << "Processing event: " << event->getEventNumber() << endl;

        hps_event->setEventNumber(event->getEventNumber()); 
        hps_event->setRunNumber(event->getRunNumber());

        // Get the collection of tracks from the event	
        tracks = (IMPL::LCCollectionVec*) event->getCollection("MatchedTracks");

        cout << "Event contains " << tracks->getNumberOfElements() << " track" << endl;
        // Loop over the tracks and fill the event
        for(int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n){ 

            // Get a track from the LCIO collection
            track = (IMPL::TrackImpl*) tracks->getElementAt(track_n);

            // Add a track to HPS event 
            hps_track = hps_event->addTrack();

            // Fill the track parameters
            hps_track->setTrackParameters(track->getD0(), track->getPhi(), 
                    track->getOmega(), track->getTanLambda(), 
                    track->getZ0());

            // Calculate the track momentum
            pt = abs((1/track->getOmega())*b_field*param);
            hps_track->setMomentum(pt*cos(track->getPhi()), 
                    pt*sin(track->getPhi()), pt*track->getTanLambda()); 

            // Get the hits associated with the track 
            hits = track->getTrackerHits();

            cout << "Track contains " << hits.size() << " hits" << endl;
            for(int hit_n = 0; hit_n < hits.size(); ++hit_n){

                
                   svt_hit = hps_event->addSvtHit(); 
                   hps_track->addHit(svt_hit); 
                   svt_hit->setLayer(getHitLayer(hits[hit_n]->getPosition())); 
                   svt_hit->setHitPosition(hits[hit_n]->getPosition());
                   svt_hit->setHitPositionError(sqrt(hits[hit_n]->getCovMatrix()[0]), 
                   sqrt(hits[hit_n]->getCovMatrix()[2]), 
                   sqrt(hits[hit_n]->getCovMatrix()[5]));      
                   
            }
        } 

        cout << "Done processing event" << endl;
        tree->Fill();
        cout << "Tree has been filled" << endl; 
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
