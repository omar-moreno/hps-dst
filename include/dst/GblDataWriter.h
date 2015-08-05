/**
 *	@section purpose: write GBL input data to DST
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	         SLAC
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *	         Santa Cruz Institute for Particle Physics
 *	         University of California, Santa Cruz
 *	@date: Feb. 12, 2014
 *
 */

#ifndef __GBL_DATA_WRITER_H__
#define __GBL_DATA_WRITER_H__

//--- C++ ---//
//-----------//
#include <cstdlib>
#include <cmath>
#include <iostream>

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <UTIL/LCRelationNavigator.h>
#include <IMPL/TrackImpl.h>

//--- HPS Event ---//
//-----------------//
#include <GblTrackData.h>
#include <GblStripData.h>

class GblDataWriter : public DataWriter {

    public:

        GblDataWriter();
        ~GblDataWriter();

        void setDebug(bool debug);
        void writeData(EVENT::LCEvent*, HpsEvent*);

    private:

        std::string track_col_name;
        std::string trk_to_gbltrk_rel_col_name;
        std::string gbltrk_to_gblstrip_rel_col_name;

        bool m_debug;
		
		IMPL::LCRelationImpl* trk_to_gbltrk_relation;
		IMPL::LCGenericObjectImpl* gbl_track_data;
        IMPL::LCGenericObjectImpl* gbl_strip;  
		IMPL::TrackImpl* track; 
	
		GblTrackData* hps_gbl_track_data;
        GblStripData* hps_gbl_strip; 

}; // GblDataWriter

#endif // __GBL_DATA_WRITER_H__
