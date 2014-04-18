/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date:		March 31, 2014
 *
 */

#ifndef __HPS_PARTICLE_WRITER_H__
#define __HPS_PARTICLE_WRITER_H__

//--- C++ ---//
//-----------//
#include <string>

//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/ReconstructedParticleImpl.h>

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- HPS Event ---//
//-----------------//
#include <HpsParticle.h>

class HpsParticleDataWriter : public DataWriter { 

    public: 

        HpsParticleDataWriter(); 
        ~HpsParticleDataWriter(); 

        //
        void writeData(EVENT::LCEvent*, HpsEvent*);

	private:

		std::string fs_particles_collection_name;
		std::string uc_vtx_particles_collection_name; 
		std::string bsc_vtx_particles_collection_name; 
		std::string tc_vtx_particles_collection_name; 

		IMPL::LCCollectionVec* particles;
		IMPL::ReconstructedParticleImpl* particle; 

		HpsParticle* hps_particle; 

}; // HpsParticleWriter

#endif // __HPS_PARTICLE_WRITER_H__
