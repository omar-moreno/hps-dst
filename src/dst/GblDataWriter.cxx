/**
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *
 */

#include <GblDataWriter.h>
#include <iostream>

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
  
  
  string gbl_track_col_name = "GBLTrackData";
  IMPL::LCCollectionVec* tracks = 0;
  // Get the collection of GBL tracks from the event. If the event doesn't
  // have the specified collection, skip the rest and just return
  try{
    tracks = (IMPL::LCCollectionVec*) event->getCollection(gbl_track_col_name);
  } catch(EVENT::DataNotAvailableException exception){
    cout << "Collection " << gbl_track_col_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }
  
  EVENT::LCGenericObject* track = NULL;

  // Loop over the tracks and fill the HpsEvent
  for(int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n){
    
    // Get a track from the LCIO collection
    track = (EVENT::LCGenericObject*) tracks->getElementAt(track_n);

    cout << "processing GBLTrackData " << track_n << endl;
	
    // Add a track to the HpsEvent
    GblTrackData* gbl_track_data = hps_event->addGblTrackData();
    
    cout << " n doubles " << track->getNDouble() << endl;
    // Fill the track parameters
    gbl_track_data->setTrackParameters(track->getDoubleVal(0),track->getDoubleVal(1),track->getDoubleVal(2),track->getDoubleVal(3),track->getDoubleVal(4));
                                               
    cout << gbl_track_data->toString() << endl;

// public static final int PERKAPPA =0;
// 		public static final int PERTHETA = 1;
// 		public static final int PERPHI = 2;
// 		public static final int PERD0 = 3;
// 		public static final int PERZ0 = 4;

    // hps_track->setTrackParameters(track->getD0(), 
    //                               track->getPhi(), 
    //                               track->getOmega(), 
	// 								  track->getTanLambda(), 
    //                               track->getZ0());
  }
  

  

}
