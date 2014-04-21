/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: December 31, 2013
 *
 */

#ifndef __HPS_EVENT_BUILDER_H__
#define __HPS_EVENT_BUILDER_H__

//--- DST ---//
//-----------//
#include <EventBuilder.h>
#include <SvtDataWriter.h>
#include <EcalDataWriter.h>
#include <MCParticleDataWriter.h>
#include <GblDataWriter.h>
#include <GblTrackWriter.h>
#include <HpsParticleDataWriter.h>

class HpsEventBuilder : public EventBuilder {

	public: 
		HpsEventBuilder(); 
		~HpsEventBuilder(); 

		//
		void makeHpsEvent(EVENT::LCEvent*, HpsEvent*); 

		// 
		void setBField(const double);
		void setGblFlag(const bool);
		
		bool getGblFlag() const;

	private:

		SvtDataWriter* svt_writer; 
		EcalDataWriter* ecal_writer; 
		MCParticleDataWriter* mc_particle_writer;
		HpsParticleDataWriter* particle_writer; 
		GblDataWriter* gbl_data_writer;
		GblTrackWriter* gbl_track_writer;

		bool m_run_gbl;
};

#endif // __HPS_EVENT_BUILDER_H__
