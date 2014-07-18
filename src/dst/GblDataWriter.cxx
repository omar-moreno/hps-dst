/**
 *	@section purpose: write GBL input data to DST
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@date: Feb. 12, 2014
 *	@version: 1.0
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *
 */

//-- DST --//
#include <GblDataWriter.h>
#include <SvtTrack.h>
#include <SvtHit.h>

//-- C++ --//
#include <cstdlib>
#include <cmath>
#include <iostream>

//-- LCIO --//
#include <UTIL/LCRelationNavigator.h>

//-- HPS event --//
#include <GblTrackData.h>
#include <GblStripData.h>


using namespace std;

// definition of generic collection sizes
// these will need to be matched to the input DST
static const unsigned int n_gblTrackGenericDoubleDST = 14;
static const unsigned int n_gblTrackGenericIntDST = 1;
static const unsigned int n_gblStripGenericDoubleDST = 22;
static const unsigned int n_gblStripGenericIntDST = 1;
static const unsigned int n_prjPerToCl = 9; // n matrix elements in projection matrix

GblDataWriter::GblDataWriter() : m_debug(false), 
                                 m_track_col_name("MatchedTracks"), 
                                 m_rel_gbltrk_name("TrackToGBLTrack"), 
                                 m_rel_toGblStrip_name("GBLTrackToStripData")
{}

GblDataWriter::GblDataWriter(bool debug) : m_debug(debug) 
{}

GblDataWriter::~GblDataWriter() 
{}

void GblDataWriter::setDebug(bool debug){
  m_debug = debug;
}

void GblDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {
    
  if(m_debug) {
    cout << "GblDataWriter: write data start " << endl;
  }

  // write information needed for GBL to the event  

  IMPL::LCCollectionVec* tracks = 0;

  // Get the collection of tracks from the event. If the event doesn't
  // have the specified collection, skip the rest and just return
  try{
    tracks = (IMPL::LCCollectionVec*) event->getCollection(m_track_col_name);
  } catch(EVENT::DataNotAvailableException e){
    if(m_debug) {
      cout << "Collection " << m_track_col_name << " was not found. "
           << "Skipping adding GBL data ..." << endl;
    }
    return;
  }

  // Get the relation to the GBL track data object
  // if it's not there, skip test rest and return

  IMPL::LCCollectionVec* trkToGblTrk = NULL;  
  try {
    trkToGblTrk = (IMPL::LCCollectionVec*) event->getCollection(m_rel_gbltrk_name);
  } catch(EVENT::DataNotAvailableException e) {
    if(m_debug) {
      cout << "Collection " << m_rel_gbltrk_name << " was not found. "
           << "Skipping adding GBL data ..." << endl;
    }
    return;
  }

  // Get the relation to the GBL hit data object
  // if it's not there, skip test rest and return

  IMPL::LCCollectionVec* gblTrkToGblStrip = NULL;  
  try {
    gblTrkToGblStrip = (IMPL::LCCollectionVec*) event->getCollection(m_rel_toGblStrip_name);
  } catch(EVENT::DataNotAvailableException e) {
    cout << "Collection " << m_rel_toGblStrip_name << " was not found. "
         << "Skipping adding GBL data ..." << endl;
    return;
  }
  
  
  // Loop over the lcsim tracks and find the GBL track and strip data info

  UTIL::LCRelationNavigator* rel_gbltrk_nav = new UTIL::LCRelationNavigator(trkToGblTrk);
  
  for(unsigned int itrack = 0; itrack != tracks->getNumberOfElements(); ++itrack) {
    const EVENT::LCObjectVec gblTracks = rel_gbltrk_nav->getRelatedToObjects(tracks->getElementAt(itrack));
    EVENT::LCObjectVec::size_type n_gblTracks =  gblTracks.size();

    if(m_debug) {
      cout << "Loop over " << n_gblTracks << " in this event" << endl;
    }
    
    for(unsigned int igbl = 0 ; igbl != n_gblTracks; ++ igbl) {
      
      EVENT::LCGenericObject* gblTrackGeneric = (EVENT::LCGenericObject*) gblTracks.at(igbl);
      
      if(m_debug) {
        cout << "processing GBLTrackData igbl " << igbl << endl;
      }
      
      // Add a track to the HpsEvent
      GblTrackData* gbl_track_data = hps_event->addGblTrackData();
      
      
      // Check that the data structure is the correct length
      if( gblTrackGeneric->getNInt() !=  n_gblTrackGenericIntDST ) {
        cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
        cout << gblTrackGeneric->getNInt() 
             << " ints. => check the DST maker" << endl;
        exit(1);
      }

      // Check that the data structure is the correct length
      if( gblTrackGeneric->getNDouble() != n_gblTrackGenericDoubleDST ) {
        cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
        cout << gblTrackGeneric->getNDouble() 
             << " doubles. => check the DST maker" << endl;
        exit(1);
      }



      // Fill the track parameters
      gbl_track_data->setTrackParameters(gblTrackGeneric->getDoubleVal(0),gblTrackGeneric->getDoubleVal(1),gblTrackGeneric->getDoubleVal(2),gblTrackGeneric->getDoubleVal(3),gblTrackGeneric->getDoubleVal(4));
      
      for(unsigned int idx = 0; idx < n_prjPerToCl; ++idx) {
        unsigned int row = static_cast<unsigned int>(floor(static_cast<double>(idx)/3.0));
        unsigned int col = idx % 3;        
        //cout << "prjmat " << idx << "," << row << "," << col << " -> " << gblTrackGeneric->getDoubleVal(5+idx) << endl;
        gbl_track_data->setPrjPerToCl(row, col, gblTrackGeneric->getDoubleVal(5+idx));
      }      
      
      
      // find the GBL hits
      
      UTIL::LCRelationNavigator* rel_gblStrip_nav = new UTIL::LCRelationNavigator(gblTrkToGblStrip);
      const EVENT::LCObjectVec gblStrips = rel_gblStrip_nav->getRelatedToObjects(gblTrackGeneric);
      const EVENT::LCObjectVec::size_type n_gblStrips = gblStrips.size();
      
      if(m_debug) {
        cout << "GblDataWriter: found " << n_gblStrips << " GBL strips for this GBL track data object" << endl;
      }
      
      for(EVENT::LCObjectVec::size_type istrip = 0; istrip < n_gblStrips; ++ istrip) {
        
        if(m_debug) {
          cout << "GblDataWriter: processing GBLStrip " << istrip << endl;
        }
        
        EVENT::LCGenericObject* gblStripGeneric = (EVENT::LCGenericObject*) gblStrips.at(istrip);

        GblStripData* gbl_strip_data = hps_event->addGblStripData();
        gbl_track_data->addStrip(gbl_strip_data);
        
        // Check that the data structure is the correct length
        if( gblStripGeneric->getNInt() ==  n_gblStripGenericIntDST ) {
          gbl_strip_data->SetId(gblStripGeneric->getIntVal(0));
        } 
        else {
          cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
          cout << gblStripGeneric->getNInt() 
               << " ints. => check the DST maker" << endl;
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
          cout << "GblDataWriter: ERROR! The data structure has the wrong format:\n";
          cout << gblStripGeneric->getNDouble() 
               << " doubles. => check the DST maker" << endl;
          exit(1);
        }

        
        
      }

      if(m_debug) {
        cout << "GblDataWriter: track data info \n" << gbl_track_data->toString() << endl;
      }
      
    } // gbl tracks
  } // seed tracks
  
  
  delete rel_gbltrk_nav;
  
  if(m_debug) {
    cout << "GblDataWriter: write data end " << endl;
  }
  
  
}


