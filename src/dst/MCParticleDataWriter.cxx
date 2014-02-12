/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@version:	v 1.0
 *	@date:		February 11, 2014
 *
 */


#include <MCParticleDataWriter.h>

MCParticleDataWriter::MCParticleDataWriter()
	: mc_particles_collection_name("MCParticle"),
	  mc_particles(NULL), mc_particle(NULL), hps_mc_particle(NULL)
{}


MCParticleDataWriter::~MCParticleDataWriter()
{}

void MCParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{

	// Get the collection of MC particles from the event. If the event doesn't have
	// MC particles, return.
	try {
		mc_particles = (IMPL::LCCollectionVec*) event->getCollection(mc_particles_collection_name);
	} catch(EVENT::DataNotAvailableException e){
		return;
	}

	// Loop over the MC particles and fill the HpsEvent
	for(int mc_particle_n = 0; mc_particle_n < mc_particles->getNumberOfElements(); ++mc_particle_n){

		// Get an MC particle from the LCIO collection
		mc_particle = (IMPL::MCParticleImpl*) mc_particles->getElementAt(mc_particle_n);

		// Add an MC particle to the HpsEvent
		hps_mc_particle = hps_event->addHpsMCParticle();

		// Set the PDG ID
		hps_mc_particle->setPDG(mc_particle->getPDG());

		// Set the charge
		hps_mc_particle->setCharge(mc_particle->getCharge());

		// Set the generator status
		hps_mc_particle->setGeneratorStatus(mc_particle->getGeneratorStatus());

		// Set the energy
		hps_mc_particle->setEnergy(mc_particle->getEnergy());

		// Set the mass
		hps_mc_particle->setMass(mc_particle->getMass());

		// Set the momentum
		hps_mc_particle->setMomentum(mc_particle->getMomentum());

		// Set the end point of the MC particle
		hps_mc_particle->setEndpoint(mc_particle->getEndpoint());

	}
}
