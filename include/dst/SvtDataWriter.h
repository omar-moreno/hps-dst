/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 2, 2013
 *	@version: 1.0
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

//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>


//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/TrackImpl.h>

class SvtDataWriter : public DataWriter { 

	public:
		
		SvtDataWriter(); 
		~SvtDataWriter(); 

		// 
		void writeData(EVENT::LCEvent*, HpsEvent*);

		// 
		void setTracksCollectionName(std::string tracks_collection_name){
			this->tracks_collection_name = tracks_collection_name; }; 

		//
		void setHitCollectionName(std::string hits_collection_name){
			this->hits_collection_name = hits_collection_name; };

		// 
		void setBField(double b_field){
			this->b_field = b_field;
		}

	private:
	
		std::string tracks_collection_name;
		std::string hits_collection_name; 	

		IMPL::LCCollectionVec* tracks;
		IMPL::TrackImpl* track;
		
		SvtTrack* hps_track;
		SvtHit* svt_hit; 		
	
		double b_field;
};

#endif // __SVT_DATA_WRITER_H__
