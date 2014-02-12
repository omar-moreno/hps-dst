/**
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *
 */

//-- DST --//
#include <GblDataWriter.h>

//-- C++ --//
#include <cstdlib>
#include <iostream>

//-- LCIO --//
#include <UTIL/LCRelationNavigator.h>

using namespace std;

GblDataWriter::GblDataWriter() : m_debug(false) 
{}

GblDataWriter::GblDataWriter(bool debug) : m_debug(debug) 
{}

GblDataWriter::~GblDataWriter() 
{}


void GblDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {
  
  // write information needed for GBL to the event  

  string track_col_name = "MatchedTracks";
  IMPL::LCCollectionVec* tracks = 0;

  // Get the collection of tracks from the event. If the event doesn't
  // have the specified collection, skip the rest and just return
  try{
    tracks = (IMPL::LCCollectionVec*) event->getCollection(track_col_name);
  } catch(EVENT::DataNotAvailableException exception){
    cout << "Collection " << track_col_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }

  // Get the relation to the GBL track data object
  // if it's not there, skip test rest and return

  string rel_gbltrk_name = "TrackToGBLTrack";
  IMPL::LCCollectionVec* trkToGblTrk = NULL;  
  try {
    trkToGblTrk = (IMPL::LCCollectionVec*) event->getCollection(rel_gbltrk_name);
  } catch(EVENT::DataNotAvailableException exception) {
    cout << "Collection " << rel_gbltrk_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }

  // Get the relation to the GBL hit data object
  // if it's not there, skip test rest and return

  string rel_toGblStrip_name = "GBLTrackToStripData";
  IMPL::LCCollectionVec* gblTrkToGblStrip = NULL;  
  try {
    gblTrkToGblStrip = (IMPL::LCCollectionVec*) event->getCollection(rel_toGblStrip_name);
  } catch(EVENT::DataNotAvailableException exception) {
    cout << "Collection " << rel_toGblStrip_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }
  
  
  // Loop over the lcsim tracks and find the GBL track and strip data info

  UTIL::LCRelationNavigator* rel_gbltrk_nav = new UTIL::LCRelationNavigator(trkToGblTrk);
  
  for(unsigned int itrack = 0; itrack != tracks->getNumberOfElements(); ++itrack) {
    const EVENT::LCObjectVec gblTracks = rel_gbltrk_nav->getRelatedToObjects(tracks->getElementAt(itrack));
    EVENT::LCObjectVec::size_type n_gblTracks =  gblTracks.size();
    
    for(unsigned int igbl = 0 ; igbl != n_gblTracks; ++ igbl) {
      
      EVENT::LCGenericObject* gblTrackGeneric = (EVENT::LCGenericObject*) gblTracks.at(igbl);
      
      cout << "processing GBLTrackData igbl " << igbl << endl;
      
      // Add a track to the HpsEvent
      GblTrackData* gbl_track_data = hps_event->addGblTrackData();
      
      // Fill the track parameters
      gbl_track_data->setTrackParameters(gblTrackGeneric->getDoubleVal(0),gblTrackGeneric->getDoubleVal(1),gblTrackGeneric->getDoubleVal(2),gblTrackGeneric->getDoubleVal(3),gblTrackGeneric->getDoubleVal(4));
      
      
      
      // find the GBL hits
      
      UTIL::LCRelationNavigator* rel_gblStrip_nav = new UTIL::LCRelationNavigator(gblTrkToGblStrip);
      const EVENT::LCObjectVec gblStrips = rel_gblStrip_nav->getRelatedToObjects(gblTrackGeneric);
      const EVENT::LCObjectVec::size_type n_gblStrips = gblStrips.size();
      
      cout << "found " << n_gblStrips << " GBL strips for this GBL track data object" << endl;
      
      for(EVENT::LCObjectVec::size_type istrip = 0; istrip < n_gblStrips; ++ istrip) {
        
        cout << "processing GBLStrip " << istrip << endl;
        
        EVENT::LCGenericObject* gblStripGeneric = (EVENT::LCGenericObject*) gblStrips.at(istrip);

        GblStripData* gbl_strip_data = hps_event->addGblStripData();
        gbl_track_data->addStrip(gbl_strip_data);
        
        // Check that the data structure is the correct length
        if( gblStripGeneric->getNInt() == 1 ) {
          gbl_strip_data->SetId(gblStripGeneric->getIntVal(0));
        } 
        else {
          cout << "ERROR! The data structure has the wrong format:\n";
          cout << gblStripGeneric->getNInt() 
               << " ints. => check the DST maker" << endl;
          exit(1);
        }
        
        // Check that the data structure is the correct length
        if( gblStripGeneric->getNDouble() == 21 ) {
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
        }
        else {
          cout << "ERROR! The data structure has the wrong format:\n";
          cout << gblStripGeneric->getNDouble() 
               << " doubles. => check the DST maker" << endl;
          exit(1);
        }

        
        
      }

      cout << gbl_track_data->toString() << endl;

      


      
    } // gbl tracks
  } // seed tracks
  
  
  delete rel_gbltrk_nav;
  
  
  
}


