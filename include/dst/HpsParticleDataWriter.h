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

}; // HpsParticleWriter

#endif // __HPS_PARTICLE_WRITER_H__
