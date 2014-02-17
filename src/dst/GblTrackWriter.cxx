/**
 *	@section purpose: interface to GBL track refit 
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@date: Feb. 12, 2014
 *	@version: 1.0
 *
 */

//--- DST ---//
#include <GblTrackWriter.h>

//--- C++ ---//
#include <iostream>
#include <cstdlib>

//--- HPS EVENT ---//
#include <HpsEvent.h>
#include <GblTrackData.h>

//--- UTILS ---//
#include <HpsGblFitter.h>


using namespace std;

GblTrackWriter::GblTrackWriter() {
}

GblTrackWriter::~GblTrackWriter() {
}

void GblTrackWriter::writeData(HpsEvent* hps_event) {

  bool debug = true;

  // Do the GBL refit of the existing track
  
  // Check if relevant information is available in the event
  
  if( hps_event->getNumberOfGblTracksData() == 0 ) {
    cout << "There is no GBL Track Data in this event. " 
         << "Skip doing GBL refit" << endl;
    return;
  }

  if(debug) {
    cout <<  hps_event->getNumberOfGblTracksData() 
         << " GBL track data objects in this event" << endl;
  }

  if( hps_event->getNumberOfGblStripData() == 0 ) {
    cout << "There is no GBL strip data in this event. " 
         << "Skip doing GBL refit" << endl;
    return;
  }

  if(debug) {
    cout <<  hps_event->getNumberOfGblStripData() 
         << " GBL strip data objects in this event" << endl;
  }
  

  if(debug) {
    cout << "GblTrackWriter: Run " << hps_event->getRunNumber() << " event " << hps_event->getEventNumber() << endl;
  }

  // create the GBL track fitter
  // TODO: move this to a member variable?
  // TODO: get magnetic field from event
  
  HpsGblFitter gblFitter(-0.491);
  gblFitter.SetDebug(debug);
  
  
  // Loop over the GBL tracks

  if(debug) {
    cout << "GblTrackWriter: " <<  hps_event->getNumberOfGblTracksData() << " GBLTrackData" << endl;
  }


  for( int i_gbl_track = 0; i_gbl_track !=  hps_event->getNumberOfGblTracksData(); ++i_gbl_track ) {
    

    // clear the fitter
    gblFitter.Clear();
    
    // find the GBL data object
    const GblTrackData* gblTrackData = hps_event->getGblTrackData(i_gbl_track);
    
    if( gblTrackData == NULL) {
      cout << "ERROR: the GBL track data object is null!?" << endl;
      exit(1);
    }

    if(debug) {
      cout << "GblTrackWriter: track " << i_gbl_track << endl;
    }

    // do the GBL refit    
    HpsGblFitter::HpsGblFitStatus fit_status = gblFitter.Fit(gblTrackData);
    
    if( fit_status == HpsGblFitter::OK) {
      // add a 
      GblTrack* gblTrack = hps_event->addGblTrack();
      gblFitter.SetTrackProperties(gblTrack, gblTrackData);
    }
    
  } //tracks

}

