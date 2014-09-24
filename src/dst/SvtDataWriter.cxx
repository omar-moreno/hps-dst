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
	: tracks(NULL), track(NULL), tracker_hit(NULL), quality_data(NULL),
	  svt_track(NULL), svt_hit(NULL),
	  tracks_collection_name("MatchedTracks"),
      trackquality_collection_name("TrackQualityData"),
      trackquality_rel_collection_name("TrackQualityDataRelations"),
	  l1_isolation(-9999), l2_isolation(-9999)
{}

SvtDataWriter::~SvtDataWriter()
{}

void SvtDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	
	// Get the collection of tracks from the event	
	tracks = (IMPL::LCCollectionVec*) event->getCollection(tracks_collection_name);


	// Get the collection of LCRelations between track quality variables 
	// (TrackQualityData) and the corresponding track.  If the collection
	// does not exist, continue and fill the track quality variables with defaults.
	// TODO: Once the final recon output is set, the DST maker should fail 
	//		 if the collection isn't found.
	try {
		quality_data = (IMPL::LCCollectionVec*) event->getCollection(trackquality_rel_collection_name);
	} catch(EVENT::DataNotAvailableException e) {
		std::cout << "Collection " << trackquality_rel_collection_name << " was not found."
				  << "Hit isolation variables will be filled with defaults." << std::endl;
	}

    UTIL::LCRelationNavigator* quality_data_nav = NULL;
	if(quality_data != NULL){
		quality_data_nav = new UTIL::LCRelationNavigator(quality_data);
	}

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

        // If the track quality data relations are present in the event,  
		// get the track quality data associated with this track.  Otherwise, 
		// just fill the track quality variables with defaults.
		if(quality_data_nav != NULL){
			EVENT::LCObjectVec quality_data_list = quality_data_nav->getRelatedFromObjects(track);
			if (quality_data_list.size()!=1) {
				std::cout << "SvtDataWriter: ERROR! The data structure has the wrong format:\n";
				std::cout << quality_data_list.size() << " TrackQualityData linked from a single track. => check the DST maker" << std::endl;
				exit(1);
			}
			IMPL::LCGenericObjectImpl* quality_datum = (IMPL::LCGenericObjectImpl*) quality_data_list.at(0);
			if (quality_datum->getNDouble()!=2) {
				std::cout << "SvtDataWriter: ERROR! The data structure has the wrong format:\n";
				std::cout << quality_datum->getNDouble() << " doubles in this TrackQualityData. => check the DST maker" << std::endl;
				exit(1);
			}
			
			l1_isolation = quality_datum->getDoubleVal(0); 
			l2_isolation = quality_datum->getDoubleVal(1); 	
		
		}
        svt_track->setL1Isolation(l1_isolation);
        svt_track->setL2Isolation(l2_isolation);

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
    delete quality_data_nav;
}
