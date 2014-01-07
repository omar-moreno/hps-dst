/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 2, 2013
 *	@version: 1.0
 *
 */

#include <SvtDataWriter.h>

SvtDataWriter::SvtDataWriter() 
	: tracks_collection_name("MatchedTracks"),
	  hits_collection_name("")
{}

SvtDataWriter::~SvtDataWriter()
{}

void SvtDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	// Get the collection of tracks from the event	
	tracks = (IMPL::LCCollectionVec*) event->getCollection(tracks_collection_name);

	// Loop over the tracks and fill the HpsEvent
	for(int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n){

		// Get a track from the LCIO collection
		track = (IMPL::TrackImpl*) tracks->getElementAt(track_n);
	
		// Add a track to the HpsEvent
		hps_track = hps_event->addTrack(); 
		
		// Fill the track parameters
		hps_track->setTrackParameters(track->getD0(), 
									  track->getPhi(), 
									  track->getOmega(), 
									  track->getTanLambda(), 
									  track->getZ0());

		// Set the track momentum
		std::vector<double> momentum = TrackUtils::getMomentumVector(track); 
		hps_track->setMomentum(momentum[0], momentum[1], momentum[2]); 
			
		// Set the track fit chi^2
		hps_track->setTrackChi2(track->getChi2());

		// Set the track charge
		hps_track->setCharge(TrackUtils::getCharge(track)); 

		// Get the hits associated with the track
		EVENT::TrackerHitVec tracker_hits = track->getTrackerHits();

		//
		for(int hit_n = 0; hit_n < (int) tracker_hits.size(); ++hit_n){
			
			svt_hit = hps_event->addSvtHit();
			hps_track->addHit(svt_hit); 
				

		}		
	}
}
