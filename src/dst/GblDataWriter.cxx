/**
 *	@section
 *		purpose: write GBL input data to DST
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	         SLAC
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *	         Santa Cruz Institute for Particle Physics
 *	         University of California, Santa Cruz
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
	  m_rel_toGblStrip_name("GBLTrackToStripData"), m_debug(true),
	  tracks(NULL), trk_to_gbltrk_relations(NULL), gbltrk_to_gblstrip_relations(NULL),
      trk_to_gbltrk_relation(NULL), gbl_track_data(NULL), gbl_strip(NULL),
      track(NULL), hps_gbl_track_data(NULL), hps_gbl_strip(NULL)
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

		// TODO: Assure that the track parameters calculated directly from the SvtTrack
		//		 are within some epsilon of those retrieved from the GblTrackData
		//		 generic object.  Throw an exception if they aren't. 
		/*if((hps_gbl_track_data->getKappa() - gbl_track_data->getDoubleVal(0)) != 0){
		   (hps_gbl_track_data->getTheta() - gbl_track_data->getDoubleVal(1)) ||
		   (hps_gbl_track_data->getPhi()   != gbl_track_data->getDoubleVal(2)) ||
		   (hps_gbl_track_data->getD0()	  != gbl_track_data->getDoubleVal(3)) ||
		   (hps_gbl_track_data->getZ0()	  != gbl_track_data->getDoubleVal(4)){
	
			std::cout << "Track data does not match!" << std::endl;
			std::cout << "hps: " << hps_gbl_track_data->getKappa() - gbl_track_data->getDoubleVal(0) << std::endl;	
		}*/	

		for(unsigned int idx = 0; idx < n_prjPerToCl; ++idx) {
			unsigned int row = static_cast<unsigned int>(floor(static_cast<double>(idx)/3.0));
			unsigned int col = idx % 3;        
			//std::cout << "prjmat " << idx << "," << row << "," << col << " -> " << gblTrackGeneric->getDoubleVal(5+idx) << std::endl;
			hps_gbl_track_data->setPrjPerToCl(row, col, gbl_track_data->getDoubleVal(5+idx));
		}   

		// Add all GblStrips to the GblTrackData object	
		UTIL::LCRelationNavigator* rel_gbl_strip_nav = new UTIL::LCRelationNavigator(gbltrk_to_gblstrip_relations);
		
		// Get the list of GblStrips that are related to the GblTrackData object
		EVENT::LCObjectVec gbl_strips = rel_gbl_strip_nav->getRelatedToObjects(gbl_track_data);
			
		if(m_debug) {
			std::cout << "GblDataWriter: found " << gbl_strips.size() 
					  << " GBL strips for this GBL track data object" << std::endl;
		}

        for(int gbl_strip_n = 0; gbl_strip_n < gbl_strips.size(); ++gbl_strip_n){
            
			if(m_debug) {
			    std::cout << "GblDataWriter: processing GBLStrip " << gbl_strip_n << std::endl;
			}

            gbl_strip = (IMPL::LCGenericObjectImpl*) gbl_strips.at(gbl_strip_n);

            hps_gbl_strip = hps_event->addGblStripData();

            hps_gbl_track_data->addStrip(hps_gbl_strip); 
				
            // Check that the data structure is the correct length
			if( gbl_strip->getNInt() ==  n_gblStripGenericIntDST ) {
				hps_gbl_strip->SetId(gbl_strip->getIntVal(0));
			} 
			else {
				std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
				std::cout << gbl_strip->getNInt() << " ints. => check the DST maker" << std::endl;
				exit(1);
			}
				
            // Check that the data structure is the correct length
			if( gbl_strip->getNDouble() == n_gblStripGenericDoubleDST ) {

				hps_gbl_strip->SetPath3D(gbl_strip->getDoubleVal(0));
				hps_gbl_strip->SetPath(gbl_strip->getDoubleVal(1));
				hps_gbl_strip->SetU(gbl_strip->getDoubleVal(2),
                                    gbl_strip->getDoubleVal(3),
                                    gbl_strip->getDoubleVal(4));
				hps_gbl_strip->SetV(gbl_strip->getDoubleVal(5),
                                    gbl_strip->getDoubleVal(6),
                                    gbl_strip->getDoubleVal(7));
				hps_gbl_strip->SetW(gbl_strip->getDoubleVal(8),
                                    gbl_strip->getDoubleVal(9),
                                    gbl_strip->getDoubleVal(10));
				hps_gbl_strip->SetGlobalTrackDir(gbl_strip->getDoubleVal(11),
                                                 gbl_strip->getDoubleVal(12),
                                                 gbl_strip->getDoubleVal(13));
				hps_gbl_strip->SetPhi(gbl_strip->getDoubleVal(14));
				hps_gbl_strip->SetUmeas(gbl_strip->getDoubleVal(15));
				hps_gbl_strip->SetTrackPos(gbl_strip->getDoubleVal(16),
                                           gbl_strip->getDoubleVal(17),
                                           gbl_strip->getDoubleVal(18));
				hps_gbl_strip->SetUmeasErr(gbl_strip->getDoubleVal(19));
				hps_gbl_strip->SetMSAngle(gbl_strip->getDoubleVal(20));
				hps_gbl_strip->SetLambda(gbl_strip->getDoubleVal(21));
			}
			else {
				std::cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
				std::cout << gbl_strip->getNDouble() << " doubles. => check the DST maker" << std::endl;
				exit(1);
	    	}
        }  // GBLStripData

		if(m_debug){
	
			std::cout << "GblDataWriter: Track parameters from LCIO GblTrackData collection: " << std::endl;
			std::cout << "kappa: " << gbl_track_data->getDoubleVal(0) << "\n"
					  << "theta: " << gbl_track_data->getDoubleVal(1) << "\n"
					  << "phi:   " << gbl_track_data->getDoubleVal(2) << "\n"
					  << "d0:    " << gbl_track_data->getDoubleVal(3) << "\n"
					  << "z0:    " << gbl_track_data->getDoubleVal(4) << std::endl;

			std::cout << "GblDataWriter: Track parameters calculated by DST GblTrackData\n" 
				      << hps_gbl_track_data->toString() << std::endl;
		
		}

        delete rel_gbl_strip_nav;

	} // GBLTrackData

	if(m_debug) {
		std::cout << "GblDataWriter: write data end " << std::endl;
	}
}


