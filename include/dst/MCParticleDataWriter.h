/**
 *  @file MCParticleDataWriter.h
 *  @brief Class used to write MC particles to the DST.
 *	@author Omar Moreno, SLAC National Accelerator Laboratory
 */

#ifndef _MC_PARTICLE_DATA_WRITER_H_
#define _MC_PARTICLE_DATA_WRITER_H_

//----------------//
//   C++ StdLib   //
//----------------//
#include <math.h>

//---------//
//   DST   //
//---------//
#include "DataWriter.h"

//----------//
//   LCIO   //
//----------//
#include <EVENT/LCIO.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/MCParticleImpl.h>
#include <Exceptions.h>

//---------------//
//   HPS Event   //
//---------------//
#include "MCParticle.h"

class MCParticleDataWriter : public DataWriter {

	public:

        /** Constructor */
		MCParticleDataWriter();

        /** Destructor */
		~MCParticleDataWriter();

		//
		void writeData(EVENT::LCEvent*, HpsEvent*);

		//
		void setMCParticleCollectionName(std::string mc_particles_collection_name){
			this->mc_particles_collection_name = mc_particles_collection_name;
		}

	private:

        /** */
		std::string mc_particles_collection_name{"MCParticle"};

        /** */
		IMPL::LCCollectionVec* mc_particles{nullptr};
		
        /** */
        IMPL::MCParticleImpl* mc_particle{nullptr};

}; // MCParticleDataWriter

#endif // _MC_PARTICLE_DATA_WRITER_H_
