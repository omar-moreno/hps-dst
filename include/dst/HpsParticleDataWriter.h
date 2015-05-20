/**
 *  @file: HpsParticleDataWriter.h
 *  @author: Omar Moreno <omoreno1@ucsc.edu>
 *  @section Institution \n
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 *  @date:      March 31, 2014
 *
 */

#ifndef __HPS_PARTICLE_WRITER_H__
#define __HPS_PARTICLE_WRITER_H__

//-----------//
//--- C++ ---//
//-----------//
#include <string>
#include <map>

//------------//
//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <IMPL/VertexImpl.h>
#include <Exceptions.h>

//-----------//
//--- DST ---//
//-----------//
#include <DataWriter.h>

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <HpsParticle.h>
#include <HpsEvent.h>

class HpsParticleDataWriter : public DataWriter { 

    public: 

        /**
         * Default constructor
         */
        HpsParticleDataWriter();

        /**
         * Destructor
         */ 
        ~HpsParticleDataWriter(); 

        /**
         * Make HpsParticles out of ReconstructedParticles and write them to
         * an HpsEvent.
         *
         * @param lc_event : LCSim event from which the ReconstructedParticle 
         *                   collection is retrieved.
         * @param hps_event : HpsEvent to which the HpsParticles will be 
         *                    written to. 
         */
        void writeData(EVENT::LCEvent* lc_event, HpsEvent* hps_event);

    private:

        /**
         * Make HpsParticles using a specific collection of 
         * ReconstructedParticles and write them to an HpsEvent.
         *
         * @param collection_type : The type of particles that will be written
         *                          to the HpsEvent.
         * @param particles: The collection of ReconstructedParticles
         * @param hps_event : HpsEvent to which the HpsParticles will be 
         *                    written to. 
         */
        void writeParticleData(HpsEvent::collection_t collection_type, IMPL::LCCollectionVec* particles, HpsEvent* hps_event); 

        std::string fs_particles_collection_name;
        std::string uc_vtx_particles_collection_name; 
        std::string bsc_vtx_particles_collection_name; 
        std::string tc_vtx_particles_collection_name; 

        IMPL::LCCollectionVec* particles;
        IMPL::ReconstructedParticleImpl* particle; 

        HpsParticle* hps_particle; 

        std::map<HpsEvent::collection_t, std::string> particle_collections;

}; // HpsParticleWriter

#endif // __HPS_PARTICLE_WRITER_H__
