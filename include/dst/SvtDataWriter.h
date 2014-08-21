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

//--- C++ ---//
//-----------//
#include <limits>
#include <cmath>
#include <stdexcept>

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- Utils ---//
//-------------//
#include <TrackUtils.h>

//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <UTIL/LCRelationNavigator.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/TrackerHitImpl.h>

//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>

class SvtDataWriter : public DataWriter { 

	public:
		
		SvtDataWriter(); 
		~SvtDataWriter(); 

		// 
		void writeData(EVENT::LCEvent*, HpsEvent*);

		// 
		void setTracksCollectionName(std::string tracks_collection_name){
			this->tracks_collection_name = tracks_collection_name;
		};

	private:
	
		std::string tracks_collection_name;
		std::string trackquality_collection_name;
		std::string trackquality_rel_collection_name;

		IMPL::LCCollectionVec* tracks;
		IMPL::TrackImpl* track;
		IMPL::TrackerHitImpl* tracker_hit;
		
		SvtTrack* svt_track;
		SvtHit* svt_hit; 		

}; // SvtDataWriter

#endif // __SVT_DATA_WRITER_H__
