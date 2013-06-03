/**
 * 	@section purpose:
 *		Writes a ROOT tree using the HpsEvent class.
 *
 *  @author: 	Omar Moreno <omoreno1@ucsc.edu>
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *  @date: February 19, 2013
 *  @version: v1.0

 */				

//--- C++ ---//
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <time.h>

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
#include <IMPL/LCGenericObjectImpl.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <UTIL/LCTOOLS.h>
#include <Exceptions.h>
#include <IMPL/LCRelationImpl.h>

//---//
#include <HpsEvent.h>
#include <SvtTrack.h>
#include <SvtHit.h>
#include <EcalCluster.h>
#include <EcalUtils.h>

using namespace std; 

void printUsage();
int getHitLayer(const double *position);

int main(int argc, char **argv)
{
	clock_t init_time = clock();

	string root_file_name;
    string lcio_file_name;
    float b_field = -0.491; 
    int option_char;
    // Parse any command line arguments.  If an invalid argument is passed,
    // print the usage
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
            case 'h':
                printUsage();
                return(EXIT_SUCCESS);
            default:
                printUsage();
                return(EXIT_FAILURE);
        }
    }

	const int fs_type  = 1;
	const int vtx_type = 2;

	// Collection Names
	const string trackCollectionName            = "MatchedTracks";
	const string trigDataCollectionName         = "TriggerBank";
	const string ecalClusterCollectionName      = "EcalClusters";
	const string finalStateReconParticleColName = "FinalStateParticles";
	const string vertexedReconParticleColName   = "VertexedReconParticles";
	const string ecal_hits_relation_col_name    = "EcalCalHitsRelation";

    double pt; 
    const double param = 2.99792458e-04; 

    // If an LCIO file is not specified, exit the program
    if(lcio_file_name.length() == 0){ 
        cout << "Please specify an LCIO file to process.\nUse the -h flag for usage" << endl;
        return(EXIT_FAILURE);
    } else if(root_file_name.length() == 0){
    	// If not specified, the default DST file name is set to "default.root"
        root_file_name = "default.root";
    }
    cout << "Output will be written to " << root_file_name << endl;

    // Create a ROOT file	
    TFile *root_file = new TFile(root_file_name.c_str(), "RECREATE");

    HpsEvent* hps_event = new HpsEvent();
    SvtTrack* hps_track = NULL;
    SvtHit* svt_hit = NULL;
    EcalCluster* hps_ecal_cluster = NULL;
    HpsReconstructedParticle* hps_recon_particle = NULL;

    // Create a ROOT tree along with the HPS Event branch which
    // will encapsulate all event information
    TTree *tree = new TTree("HPS_Event", "HPS event tree"); 
    tree->Branch("Event", "HpsEvent", &hps_event, 32000, 3); 

    // Create the LCIO file reader
    IO::LCReader *lc_reader = IOIMPL::LCFactory::getInstance()->createLCReader();
    lc_reader->open(lcio_file_name.c_str());

    IMPL::TrackImpl* track;
    IMPL::LCCollectionVec* tracks;
    IMPL::ClusterImpl* ecal_cluster;
    IMPL::LCCollectionVec* ecal_clusters;
    IMPL::LCCollectionVec* trigger_data;
    IMPL::LCGenericObjectImpl *trigger_datum;
    IMPL::LCCollectionVec* recon_particles;
    IMPL::ReconstructedParticleImpl* recon_particle;
    IMPL::LCRelationImpl* ecal_cal_hits_relation;
    IMPL::LCCollectionVec* ecal_cal_hits_relations;
    EVENT::LCEvent* event;
    EVENT::TrackerHitVec hits;
    EVENT::CalorimeterHitVec ecal_hits;
    vector<int> trigger_bits;
    // Loop over all events in the LCIO file
    while( (event = lc_reader->readNextEvent()) != 0 ){

    	// Clear the event of any previous data
        hps_event->Clear(); 
        trigger_bits.clear();

        // Print the event number every 500 events
        if(event->getEventNumber()%500 == 0){
        	cout << "Processing event: " << event->getEventNumber() << endl;
        }

        // Set the event number and run number
        hps_event->setEventNumber(event->getEventNumber());
        hps_event->setRunNumber(event->getRunNumber());

        // Get the trigger data from the event and fill the trigger bit
        // information
        trigger_data = (IMPL::LCCollectionVec*) event->getCollection(trigDataCollectionName);
        trigger_datum = (IMPL::LCGenericObjectImpl*) trigger_data->getElementAt(0);
        for(int trig_n = 0; trig_n < trigger_datum->getNInt(); ++trig_n){
        	trigger_bits.push_back(trigger_datum->getIntVal(trig_n));
        }
        hps_event->setTriggerBitInfo(trigger_bits);

        // Get the collection of tracks from the event	
        tracks = (IMPL::LCCollectionVec*) event->getCollection(trackCollectionName);

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

            // Set the track fit chi^2
            hps_track->setTrackChi2(track->getChi2());

            // Set the track charge
            if(track->getOmega() > 0) hps_track->setCharge(1);
            else hps_track->setCharge(-1);

            // Get the hits associated with the track 
            hits = track->getTrackerHits();

            for(int hit_n = 0; hit_n < (int) hits.size(); ++hit_n){
                
                   svt_hit = hps_event->addSvtHit(); 
                   hps_track->addHit(svt_hit); 
                   svt_hit->setLayer(getHitLayer(hits[hit_n]->getPosition())); 
                   svt_hit->setHitPosition(hits[hit_n]->getPosition());
                   svt_hit->setHitPositionError(sqrt(hits[hit_n]->getCovMatrix()[0]), 
                   sqrt(hits[hit_n]->getCovMatrix()[2]), 
                   sqrt(hits[hit_n]->getCovMatrix()[5]));      
                   
            }
        }

        // Get the collection of Ecal clusters from the event
        ecal_clusters = (IMPL::LCCollectionVec*) event->getCollection(ecalClusterCollectionName);
        ecal_cal_hits_relations = (IMPL::LCCollectionVec*) event->getCollection(ecal_hits_relation_col_name);

        // Loop over all clusters and fill the event
        for(int cluster_n = 0; cluster_n < ecal_clusters->getNumberOfElements(); ++cluster_n){

        	// Get an Ecal cluster from the LCIO collection
        	ecal_cluster = (IMPL::ClusterImpl*) ecal_clusters->getElementAt(cluster_n);

        	// Add a cluster to HPS event
        	hps_ecal_cluster = hps_event->addEcalCluster();

        	// Set the cluster position
        	hps_ecal_cluster->setClusterPosition((double*) ecal_cluster->getPosition());

        	// Set the cluster energy
        	hps_ecal_cluster->setClusterEnergy(ecal_cluster->getEnergy());

        	// Get the ecal hits used to create the cluster
        	ecal_hits = ecal_cluster->getCalorimeterHits();

        	// Set the number of crystals associated with the cluster
        	hps_ecal_cluster->setNumberOfEcalHits(ecal_hits.size());

        	double* moments = EcalUtils::getShowerMoments(ecal_cluster, ecal_cal_hits_relations);

        	//
        	hps_ecal_cluster->setM2(moments[1]);

        	//
        	hps_ecal_cluster->setM3(moments[2]);

        	// Loop through all of the Ecal hits in the event and find the
        	// crystal with the highest energy
        	double hit_energy = 0;
        	float hit_index = -1;
        	for(int ecal_hit_n = 0; ecal_hit_n < (int) ecal_hits.size(); ++ecal_hit_n){
        		if(hit_energy < ecal_hits[ecal_hit_n]->getEnergy())
        			hit_energy = ecal_hits[ecal_hit_n]->getEnergy();
        			hit_index = ecal_hit_n;
        	}

        	// Set the energy and position of the highest energy crystal in
        	// the cluster
        	hps_ecal_cluster->setHighestEnergyHit(hit_energy);
        	hps_ecal_cluster->setHighestEnergyHitPosition((double*) ecal_hits[hit_index]->getPosition());

        }

        // Get the collection of of final state ReconstructedParticles
        try{
        	recon_particles
        		= (IMPL::LCCollectionVec*) event->getCollection(finalStateReconParticleColName);
        } catch(EVENT::DataNotAvailableException &exception) {
 //       	cout << "Collection " << finalStateReconParticleColName << " was not found." << endl;
        	continue;
        }
        // Loop over all final state ReconstructedParticles and fill the event
        for(int recon_n = 0; recon_n < recon_particles->getNumberOfElements(); ++recon_n){

        	// Get a final state ReconstructedParticle from the LCIO collection
        	recon_particle = (IMPL::ReconstructedParticleImpl*) recon_particles->getElementAt(recon_n);

        	// Add a reconstructed particle to HPS event
        	hps_recon_particle = hps_event->addReconParticle(fs_type);

        	// Add a cluster to the collection of final state recon particles
        	if(recon_particle->getClusters().size() > 0){
        		for(int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n){

        			if(recon_particle->getClusters()[0]->getEnergy() == hps_event->getEcalCluster(cluster_n)->getClusterEnergy()){
        				hps_recon_particle->setEcalCluster(hps_event->getEcalCluster(cluster_n));
        			}
        		}
        	}

        	// Add a track to the collection of final state recon particles
        	if(recon_particle->getTracks().size() > 0){
        		for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){
        			if(recon_particle->getTracks()[0]->getChi2() == hps_event->getTrack(track_n)->getChi2()){
        				hps_recon_particle->setTrack(hps_event->getTrack(track_n));
        			}
        		}
        	}
        }

        tree->Fill();
        pt = 0; 
    }

    lc_reader->close();
    root_file->Write();
    root_file->Close();

    cout << "Finished writing ROOT Tree!" << endl;

    clock_t end_time = clock();
    cout << "Time elapsed: " << (end_time - init_time)*1000/CLOCKS_PER_SEC << " seconds" << endl;

    return(EXIT_SUCCESS);
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
