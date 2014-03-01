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

GblTrackWriter::GblTrackWriter() : m_debug(false), m_b_field(-99999999.9) {
}

GblTrackWriter::~GblTrackWriter() {
}

void GblTrackWriter::setDebug(bool debug) {
  m_debug = debug;
}

void GblTrackWriter::writeData(HpsEvent* hps_event) {


  // Do the GBL refit of the existing track
  
  // Check if relevant information is available in the event
  
  if( hps_event->getNumberOfGblTracksData() == 0 ) {
    if(m_debug) {
      cout << "GblTrackWriter: There are no GBL Track Data in this event. " 
           << "Skip doing GBL refit" << endl;
    }
    return;
  }

  if(m_debug) {
    cout <<  "GblTrackWriter: " << hps_event->getNumberOfGblTracksData() 
         << " GBL track data objects in this event" << endl;
  }

  if( hps_event->getNumberOfGblStripData() == 0 ) {
    cout << "GblTrackWriter:  There are no GBL strip data in this event. " 
         << "Skip doing GBL refit" << endl;
    return;
  }

  if(m_debug) {
    cout <<  "GblTrackWriter: " << hps_event->getNumberOfGblStripData() 
         << " GBL strip data objects in this event" << endl;
  }
  


  // create the GBL track fitter
  // TODO: move this to a member variable?
  // TODO: get magnetic field from event
  
  if( m_b_field < -999999.) {
    cout << "GblTrackWriter: ERROR b_field is not set correctly, B-field = " << m_b_field << endl;
    exit(-1);
  }

  HpsGblFitter gblFitter(m_b_field);
  gblFitter.SetDebug(m_debug);
  
  
  // Loop over the GBL tracks

  if(m_debug) {
    cout << "GblTrackWriter: " <<  hps_event->getNumberOfGblTracksData() << " GBLTrackData" << endl;
  }


  for( int i_gbl_track = 0; i_gbl_track !=  hps_event->getNumberOfGblTracksData(); ++i_gbl_track ) {
    

    // clear the fitter
    gblFitter.Clear();
    
    // find the GBL data object
    const GblTrackData* gblTrackData = hps_event->getGblTrackData(i_gbl_track);
    
    if( gblTrackData == NULL) {
      cout << "GblTrackWriter: ERROR: the GBL track data object is null!?" << endl;
      exit(1);
    }

    if(m_debug) {
      cout << "GblTrackWriter: gbl track " << i_gbl_track << endl;
    }

    // do the GBL refit    
    HpsGblFitter::HpsGblFitStatus fit_status = gblFitter.Fit(gblTrackData);
    
    if( fit_status == HpsGblFitter::OK) {

      if(m_debug) {
        cout << "TrackWriter: fit ok, add a gbl track to the event" << endl;
      }

      // add a track to the event
      GblTrack* gblTrack = hps_event->addGblTrack();
      gblFitter.SetTrackProperties(gblTrack, gblTrackData);

    } else {

      if(m_debug) {
        cout << "TrackWriter: fit NOT ok, no gbl track added" << endl;
      }
      
    }
    
  } //tracks

}

void GblTrackWriter::setBField(double b_field) {
  m_b_field = b_field;
}


