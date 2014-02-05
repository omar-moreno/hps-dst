/**
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *
 */

#include <GblDataWriter.h>
#include <iostream>
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
  // then use the event in the GBL API
  // this will allow GBL to run standalone on DST
  

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
  
  string rel_name = "TrackToGBLTrack";
  IMPL::LCCollectionVec* trkToGblTrk = NULL;  
  try {
    trkToGblTrk = (IMPL::LCCollectionVec*) event->getCollection(rel_name);
  } catch(EVENT::DataNotAvailableException exception) {
    cout << "Collection " << rel_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }
  
  UTIL::LCRelationNavigator* rel_nav = new UTIL::LCRelationNavigator(trkToGblTrk);
  
  cout << "relation from " << rel_nav->getFromType() << " to " << rel_nav->getToType() << " found" << endl;
  
  for(unsigned int itrack = 0; itrack != tracks->getNumberOfElements(); ++itrack) {
    const EVENT::LCObjectVec gbltracks = rel_nav->getRelatedToObjects(tracks->getElementAt(itrack));
    EVENT::LCObjectVec::size_type n_gbltracks =  gbltracks.size();
    
    for(unsigned int igbl = 0 ; igbl != n_gbltracks; ++ igbl) {
      
      EVENT::LCGenericObject* gbltrack = (EVENT::LCGenericObject*) gbltracks.at(igbl);
      
      cout << "processing GBLTrackData igbl " << igbl << endl;
      
      
      // Add a track to the HpsEvent
      GblTrackData* gbl_track_data = hps_event->addGblTrackData();
      
      cout << " n doubles " << gbltrack->getNDouble() << endl;
      // Fill the track parameters
      gbl_track_data->setTrackParameters(gbltrack->getDoubleVal(0),gbltrack->getDoubleVal(1),gbltrack->getDoubleVal(2),gbltrack->getDoubleVal(3),gbltrack->getDoubleVal(4));
      
      cout << gbl_track_data->toString() << endl;
      
      
    } // gbl tracks
  } // seed tracks
  
  
  delete rel_nav;
  
  
  
}


