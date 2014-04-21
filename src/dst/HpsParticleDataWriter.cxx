/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@date:		March 31, 2014
 *
 */

#include <HpsParticleDataWriter.h>

HpsParticleDataWriter::HpsParticleDataWriter() 
	: fs_particles_collection_name("FinalStateParticles"),
	  uc_vtx_particles_collection_name("AprimeUnconstrained"),
	  bsc_vtx_particles_collection_name("AprimeBeamspotConstrained"),
	  tc_vtx_particles_collection_name("AprimeTargetConstrained"),
	  particles(NULL), particle(NULL), hps_particle(NULL)
{
	particle_collections.insert(std::pair<int, std::string>(0, fs_particles_collection_name)); 
	particle_collections.insert(std::pair<int, std::string>(1, uc_vtx_particles_collection_name)); 
	particle_collections.insert(std::pair<int, std::string>(2, bsc_vtx_particles_collection_name)); 
	particle_collections.insert(std::pair<int, std::string>(3, tc_vtx_particles_collection_name)); 
}

HpsParticleDataWriter::~HpsParticleDataWriter()
{
	delete particles;
	delete particle;
	delete hps_particle;
}

void HpsParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	try { 

		// Iterate through all of the collections of particles.  If the collection 
		// is empty, skip it.
		std::map<int, std::string>::iterator particle_collection; 
		for(particle_collection = particle_collections.begin(); 
				particle_collection != particle_collections.end(); ++particle_collection){

			// Get the collection from the event
			particles = (IMPL::LCCollectionVec*) event->getCollection(particle_collection->second);
			if(particle_collection->first == 0 && particles->getNumberOfElements() == 0) return;
			else if(particles->getNumberOfElements() == 0) continue;

			// Write the particle data to the event
			writeData(particle_collection->first, particles, hps_event); 
		}

	} catch(EVENT::DataNotAvailableException e){
		// For now, don't do anything.  Once the HPS reconstruction is
		// verified to place a collection for every single event, then the
		// exception will be handled properly.
	}	
}

void HpsParticleDataWriter::writeData(int collection_type, IMPL::LCCollectionVec* particles, HpsEvent* hps_event)
{
	// Loop through all of the particles in the event
	for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){

		// Get a particle from the LCEvent
		particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

		// Get a particle from the HpsEvent
		if(collection_type == 0) hps_particle = hps_event->addFSParticle();
		else hps_particle = hps_event->addVtxParticle(collection_type); 	

		// Only write particles that have clusters associated with them.  If a 
		// particle only has a track and no cluster, there is an issue with 
		// the HPS reconstruction.
		if(particle->getClusters().size() == 0) continue;

		// Loop through all of the clusters in the HpsEvent and find the one 
		// that matches the cluster associated with the particle.
		for(int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n){

			// Use the cluster energy to find the match
			// TODO: Verify that the cluster enegy is enough to find a match
			if(particle->getClusters()[0]->getEnergy() == hps_event->getEcalCluster(cluster_n)->getEnergy()){
				hps_particle->setCluster(hps_event->getEcalCluster(cluster_n)); 
				break;
			}
		}

		// If there aren't any tracks associated with a particle, continue to the next particle
		if(particle->getTracks().size() == 0) continue;

		// Loop through all of the tracks in the HpsEvent and find the one
		// that matches the track associated with the particle
		for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){

			// Use the track chi^2 to find the match
			// TODO: Verify that the chi^2 is enough to find the match
			if(particle->getTracks()[0]->getChi2() == hps_event->getTrack(track_n)->getChi2()){
				hps_particle->setTrack(hps_event->getTrack(track_n));
				break;
			}
		}
	}	
}
