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
	: tracks(NULL),
      track(NULL),
      tracker_hit(NULL),
	  svt_track(NULL),
      svt_hit(NULL),
	  tracks_collection_name("MatchedTracks"),
      track_data_collection_name("TrackData"), 
      track_data_rel_collection_name("TrackDataRelations") { 
}

SvtDataWriter::~SvtDataWriter() {
}

void SvtDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {
	
	// Get the collection of tracks from the event	
    tracks = (IMPL::LCCollectionVec*) event->getCollection(tracks_collection_name);

	// Get the collection of LCRelations between track data variables 
	// (TrackData) and the corresponding track.
    track_data = (IMPL::LCCollectionVec*) event->getCollection(track_data_rel_collection_name);
    
    UTIL::LCRelationNavigator* track_data_nav = new UTIL::LCRelationNavigator(track_data);

	// Loop over the tracks and fill the HpsEvent
	for (int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n) {

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

        // Get te track data associated with this track.
        EVENT::LCObjectVec track_data_list = track_data_nav->getRelatedFromObjects(track);

        // If the track data list should only contain a single object.  If not, throw an exception
        if (track_data_list.size() != 1) { 
            throw std::runtime_error("[ SvtDataWriter ]: The data structure has the wrong format.");
        }

        // Get the track data generic object
        IMPL::LCGenericObjectImpl* track_datum = (IMPL::LCGenericObjectImpl*) track_data_list.at(0);
       
        // Check that the data structure is correct.  If it's not, throw a runtime exception.   
        if (track_datum->getNDouble() != 12 || track_datum->getNFloat() != 1 || track_datum->getNInt() != 1) {
            throw std::runtime_error("[ SvtDataWriter ]: The collection " + track_data_collection_name 
                    + " has the wrong structure.");
        }

        // Set all isolation values for the track
        for (int iso_index = 0; iso_index < track_datum->getNDouble(); ++iso_index) { 
            svt_track->setIsolation(iso_index, track_datum->getDoubleVal(iso_index));
        }

        // Set the track time
        svt_track->setTrackTime(track_datum->getFloatVal(0));

        // Set the volume (top/bottom) in which the track resides
        svt_track->setTrackVolume(track_datum->getIntVal(0));

		// Get the hits associated with the track
		EVENT::TrackerHitVec tracker_hits = track->getTrackerHits();

		//  Loop through all of the hits and add references to those that
        //  correspond to this track
		for (int hit_n = 0; hit_n < (int) tracker_hits.size(); ++hit_n) {
			
			tracker_hit = (IMPL::TrackerHitImpl*) tracker_hits[hit_n];

			svt_hit = hps_event->addSvtHit();

			svt_hit->setLayer(TrackUtils::getLayer(tracker_hit));
			svt_hit->setPosition(tracker_hit->getPosition());
			svt_hit->setCovarianceMatrix(tracker_hit->getCovMatrix());
			svt_hit->setTime(tracker_hit->getTime());

			svt_track->addHit(svt_hit);
        } 
        		
    }
    delete track_data_nav;
}
