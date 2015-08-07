/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 2, 2013
 *
 */

#ifndef __SVT_DATA_WRITER_H__
#define __SVT_DATA_WRITER_H__

//-----------//
//--- C++ ---//
//-----------//
#include <stdexcept>

//-----------//
//--- DST ---//
//-----------//
#include <DataWriter.h>

//-------------//
//--- Utils ---//
//-------------//
#include <TrackUtils.h>

//------------//
//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <UTIL/LCRelationNavigator.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/TrackerHitImpl.h>

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>

class SvtDataWriter : public DataWriter { 

	public:

        /**
         * Constructor
         */        
		SvtDataWriter();

        /**
         * Default constructor
         */ 
		~SvtDataWriter(); 

        /**
         * Make SvtTracks and SvtHits out of LCIO Tracks and TrackerHits
         * and write them to an HpsEvent.
         *
         * @param lc_event : LCSim event from which the Tracks and 
         *                   TrackerHit collections are retrieved.
         * @param hps_event : HpsEvent to which the SvtTracks and SvtHits 
         *                    will be written to 
         */        
		void writeData(EVENT::LCEvent*, HpsEvent*);

	private:

		IMPL::LCCollectionVec* tracks;
		IMPL::TrackImpl* track;
		IMPL::TrackerHitImpl* tracker_hit;
        IMPL::LCCollectionVec* track_data;
		
		SvtTrack* svt_track;
		SvtHit* svt_hit; 		

		std::string tracks_collection_name;
        std::string track_data_collection_name;
        std::string track_data_rel_collection_name;

}; // SvtDataWriter

#endif // __SVT_DATA_WRITER_H__
