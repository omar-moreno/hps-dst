/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 2, 2013
 *
 */

#include <SvtDataWriter.h>

SvtDataWriter::SvtDataWriter() 
	: tracks_collection_name("MatchedTracks")
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
		svt_track = hps_event->addTrack(); 
		
		// Fill the track parameters
		svt_track->setTrackParameters(track->getD0(), 
									  track->getPhi(), 
									  track->getOmega(), 
									  track->getTanLambda(), 
									  track->getZ0());
			
		// Set the track fit chi^2
		svt_track->setChi2(track->getChi2());

		// Get the hits associated with the track
		EVENT::TrackerHitVec tracker_hits = track->getTrackerHits();

		//
		for(int hit_n = 0; hit_n < (int) tracker_hits.size(); ++hit_n){
			
			tracker_hit = (IMPL::TrackerHitImpl*) tracker_hits[hit_n];

			svt_hit = hps_event->addSvtHit();

			svt_hit->setLayer(TrackUtils::getLayer(tracker_hit));
			svt_hit->setPosition(tracker_hit->getPosition());
			svt_hit->setCovarianceMatrix(tracker_hit->getCovMatrix());
			svt_hit->setTime(tracker_hit->getTime());

			svt_track->addHit(svt_hit); 
		}		
	}
}
