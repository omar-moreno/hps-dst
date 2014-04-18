/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date:		March 31, 2014
 *
 */

#include <HpsParticleDataWriter.h>

HpsParticleDataWriter::HpsParticleDataWriter(){}

HpsParticleDataWriter::~HpsParticleDataWriter(){}

void HpsParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
    // Get the collection of particles from the event. If the event doesn't 
    // have particles, return;
    try { 
    } catch { 
       


    }
}
