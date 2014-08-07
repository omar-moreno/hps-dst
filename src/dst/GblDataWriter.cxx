/**
 *	@section purpose: write GBL input data to DST
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@date: Feb. 12, 2014
 *
 */

#include <GblDataWriter.h>

// definition of generic collection sizes
// these will need to be matched to the input DST
static const unsigned int n_gblTrackGenericDoubleDST = 14;
static const unsigned int n_gblTrackGenericIntDST = 1;
static const unsigned int n_gblStripGenericDoubleDST = 22;
static const unsigned int n_gblStripGenericIntDST = 1;
static const unsigned int n_prjPerToCl = 9; // n matrix elements in projection matrix

GblDataWriter::GblDataWriter() 
	: m_track_col_name("MatchedTracks"), m_rel_gbltrk_name("TrackToGBLTrack"), 
	  m_rel_toGblStrip_name("GBLTrackToStripData"), m_debug(false),
	  tracks(NULL)
{}

GblDataWriter::~GblDataWriter() 
{}

void GblDataWriter::setDebug(bool debug){
	m_debug = debug;
}

void GblDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {

	if(m_debug) {
		std::cout << "GblDataWriter: write data start " << std::endl;
	}

	// Get the collection of tracks from the event. If the event doesn't
	// have the specified collection, skip the rest and just return
	try{
		tracks = (IMPL::LCCollectionVec*) event->getCollection(m_track_col_name);
	} catch(EVENT::DataNotAvailableException e){
		std::cout << "Collection " << m_track_col_name << " was not found. "
				  << "Skipping adding GBL data ..." << std::endl;
		return;
	}

	// Get the collection of LCRelations between a GblTrackData and the 
	// corresponding seed track. If the event doesn't have the specified 
	// collection, skip it
	try {
		trk_to_gbltrk_relations = (IMPL::LCCollectionVec*) event->getCollection(m_rel_gbltrk_name);
	} catch(EVENT::DataNotAvailableException e) {
		std::cout << "Collection " << m_rel_gbltrk_name << " was not found. "
				  << "Skipping adding GBL data ..." << std::endl;
		return;
	}

	// Get the collection of LCRelations between a GblStripData (GBL hit) 
	// object and the corresponding GblTrackData object.  If the event
	// doesn't have the specified collection, skip it
	try {
		gbltrk_to_gblstrip_relations = (IMPL::LCCollectionVec*) event->getCollection(m_rel_toGblStrip_name);
	} catch(EVENT::DataNotAvailableException e) {
		std::cout << "Collection " << m_rel_toGblStrip_name << " was not found. "
				  << "Skipping adding GBL data ..." << std::endl;
		return;
	}


	for(int rel_n = 0; rel_n < trk_to_gbltrk_relations->getNumberOfElements(); ++rel_n){
		
		trk_to_gbltrk_relation = (IMPL::LCRelationImpl*) trk_to_gbltrk_relations->getElementAt(rel_n);
		
		// Get the GblTrackData
		gbl_track_data = (IMPL::LCGenericObjectImpl*) trk_to_gbltrk_relation->getTo();

		// Get the track related to the GblTrackData
		track = (IMPL::TrackImpl*) trk_to_gbltrk_relation->getFrom(); 
		
		// Check that the data structure is the correct length
		if( gbl_track_data->getNInt() !=  n_gblTrackGenericIntDST ) {
			std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
			std::cout << gbl_track_data->getNInt() << " ints. => check the DST maker" << std::endl;
			exit(1);
		}

		// Check that the data structure is the correct length
		if( gbl_track_data->getNDouble() != n_gblTrackGenericDoubleDST ) {
			std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
			std::cout << gbl_track_data->getNDouble() << " doubles. => check the DST maker" << std::endl;
			exit(1);
		}
		
		// Add a GblTrackData object to the HpsEvent
		hps_gbl_track_data = hps_event->addGblTrackData();

		// Loop through all of the tracks in the HpsEvent and find the one
		// that matches the track associated with the GblTrackData
		for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){
			
			// Use the track fit chi^2 to find the match
			// TODO: Verify that the track fit chit^2 is enough to conclude
			//		 that the tracks match	
			if(track->getChi2() == hps_event->getTrack(track_n)->getChi2()){
				hps_gbl_track_data->setTrack(hps_event->getTrack(track_n));
				break;	
			}
		}


		// Fill the seed track parameters 
		// TODO: Other than theta, these are the same track parameters as the 
		//		 seed track.  Should a reference to the seed track be stored 
		//		 instead?
		/*hps_gbl_track_data->setTrackParameters(gblTrackGeneric->getDoubleVal(0),
										  gblTrackGeneric->getDoubleVal(1),
										  gblTrackGeneric->getDoubleVal(2),
										  gblTrackGeneric->getDoubleVal(3),
										  gblTrackGeneric->getDoubleVal(4));
		 */

		for(unsigned int idx = 0; idx < n_prjPerToCl; ++idx) {
			unsigned int row = static_cast<unsigned int>(floor(static_cast<double>(idx)/3.0));
			unsigned int col = idx % 3;        
			//std::cout << "prjmat " << idx << "," << row << "," << col << " -> " << gblTrackGeneric->getDoubleVal(5+idx) << std::endl;
			hps_gbl_track_data->setPrjPerToCl(row, col, gbl_track_data->getDoubleVal(5+idx));
		}   

		// Add all GblStrips to the GblTrackData object	
		UTIL::LCRelationNavigator* rel_gblStrip_nav = new UTIL::LCRelationNavigator(gbltrk_to_gblstrip_relations);
		
		// Get the list of GblStrips that are related to the GblTrackData object
		EVENT::LCObjectVec gbl_strips = rel_gblStrip_nav->getRelatedToObjects(gbl_track_data);
			
		if(m_debug) {
			std::cout << "GblDataWriter: found " << gbl_strips.size() 
					  << " GBL strips for this GBL track data object" << std::endl;
		}

		// Loop over all of the strips and add them to both the HpsEvent and GblTrackData
	}


	/*
	for(unsigned int itrack = 0; itrack != tracks->getNumberOfElements(); ++itrack) {

		for(unsigned int igbl = 0 ; igbl != n_gblTracks; ++ igbl) {

			// find the GBL hits

			UTIL::LCRelationNavigator* rel_gblStrip_nav = new UTIL::LCRelationNavigator(gbltrk_to_gblstrip_relations);
			const EVENT::LCObjectVec gblStrips = rel_gblStrip_nav->getRelatedToObjects(gblTrackGeneric);
			const EVENT::LCObjectVec::size_type n_gblStrips = gblStrips.size();

			for(EVENT::LCObjectVec::size_type istrip = 0; istrip < n_gblStrips; ++ istrip) {

				if(m_debug) {
					std::cout << "GblDataWriter: processing GBLStrip " << istrip << std::endl;
				}

				EVENT::LCGenericObject* gblStripGeneric = (EVENT::LCGenericObject*) gblStrips.at(istrip);

				GblStripData* gbl_strip_data = hps_event->addGblStripData();
				gbl_track_data->addStrip(gbl_strip_data);

				// Check that the data structure is the correct length
				if( gblStripGeneric->getNInt() ==  n_gblStripGenericIntDST ) {
					gbl_strip_data->SetId(gblStripGeneric->getIntVal(0));
				} 
				else {
					std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
					std::cout << gblStripGeneric->getNInt() 
						<< " ints. => check the DST maker" << std::endl;
					exit(1);
				}

				// Check that the data structure is the correct length
				if( gblStripGeneric->getNDouble() == n_gblStripGenericDoubleDST ) {
					gbl_strip_data->SetPath3D(gblStripGeneric->getDoubleVal(0));
					gbl_strip_data->SetPath(gblStripGeneric->getDoubleVal(1));
					gbl_strip_data->SetU(gblStripGeneric->getDoubleVal(2),gblStripGeneric->getDoubleVal(3),gblStripGeneric->getDoubleVal(4));
					gbl_strip_data->SetV(gblStripGeneric->getDoubleVal(5),gblStripGeneric->getDoubleVal(6),gblStripGeneric->getDoubleVal(7));
					gbl_strip_data->SetW(gblStripGeneric->getDoubleVal(8),gblStripGeneric->getDoubleVal(9),gblStripGeneric->getDoubleVal(10));


					gbl_strip_data->SetGlobalTrackDir(gblStripGeneric->getDoubleVal(11),gblStripGeneric->getDoubleVal(12),gblStripGeneric->getDoubleVal(13));
					gbl_strip_data->SetPhi(gblStripGeneric->getDoubleVal(14));
					gbl_strip_data->SetUmeas(gblStripGeneric->getDoubleVal(15));
					gbl_strip_data->SetTrackPos(gblStripGeneric->getDoubleVal(16),gblStripGeneric->getDoubleVal(17),gblStripGeneric->getDoubleVal(18));
					gbl_strip_data->SetUmeasErr(gblStripGeneric->getDoubleVal(19));
					gbl_strip_data->SetMSAngle(gblStripGeneric->getDoubleVal(20));
					gbl_strip_data->SetLambda(gblStripGeneric->getDoubleVal(21));
				}
				else {
					std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
					std::cout << gblStripGeneric->getNDouble() 
						<< " doubles. => check the DST maker" << std::endl;
					exit(1);
				}



			}

			if(m_debug) {
				std::cout << "GblDataWriter: track data info \n" << gbl_track_data->toString() << std::endl;
			}

		} // gbl tracks
	} // seed tracks


	delete rel_gbltrk_nav;

	if(m_debug) {
		std::cout << "GblDataWriter: write data end " << std::endl;
	}
	*/

}


