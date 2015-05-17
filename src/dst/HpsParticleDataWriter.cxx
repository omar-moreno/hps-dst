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
	  uc_vtx_particles_collection_name("UnconstrainedV0Candidates"),
	  bsc_vtx_particles_collection_name("BeamspotConstrainedV0Candidates"),
	  tc_vtx_particles_collection_name("TargetConstrainedV0Candidates"),
	  particles(NULL), particle(NULL), hps_particle(NULL)
{
	particle_collections.insert(std::pair<HpsEvent::collection_t, std::string>(HpsEvent::FINAL_STATE_PARTICLES, fs_particles_collection_name)); 
	particle_collections.insert(std::pair<HpsEvent::collection_t, std::string>(HpsEvent::UC_VTX_PARTICLES, uc_vtx_particles_collection_name)); 
	particle_collections.insert(std::pair<HpsEvent::collection_t, std::string>(HpsEvent::BSC_VTX_PARTICLES, bsc_vtx_particles_collection_name)); 
	particle_collections.insert(std::pair<HpsEvent::collection_t, std::string>(HpsEvent::TC_VTX_PARTICLES, tc_vtx_particles_collection_name)); 
}

HpsParticleDataWriter::~HpsParticleDataWriter()
{}

void HpsParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	try { 

		// Iterate through all of the collections of particles.  If the collection 
		// is empty, skip it.
		std::map<HpsEvent::collection_t, std::string>::iterator particle_collection; 
		for(particle_collection = particle_collections.begin(); 
				particle_collection != particle_collections.end(); ++particle_collection){

			// Get the collection from the event
			particles = (IMPL::LCCollectionVec*) event->getCollection(particle_collection->second);
			if(particle_collection->first == HpsEvent::FINAL_STATE_PARTICLES && particles->getNumberOfElements() == 0) return;
			else if(particles->getNumberOfElements() == 0) continue;

			// Write the particle data to the event
			writeParticleData(particle_collection->first, particles, hps_event); 
		}

	} catch(EVENT::DataNotAvailableException e){
		// For now, don't do anything.  Once the HPS reconstruction is
		// verified to place a collection for every single event, then the
		// exception will be handled properly.
	}	
}

void HpsParticleDataWriter::writeParticleData(HpsEvent::collection_t collection_type, IMPL::LCCollectionVec* particles, HpsEvent* hps_event)
{
	// Loop through all of the particles in the event
	for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){

		// Get a particle from the LCEvent
		particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

		// Get a particle from the HpsEvent
        hps_particle = hps_event->addParticle(collection_type);
       /* 
		if(collection_type == HpsEvent::FINAL_STATE_PARTICLES) hps_particle = hps_event->addFSParticle();
		else {
			hps_particle = hps_event->addVtxParticle(collection_type); 
		}*/

		// Set the charge of the HpsParticle	
		hps_particle->setCharge(particle->getCharge()); 
		
		// Set the energy of the HpsParticle
		hps_particle->setEnergy(particle->getEnergy());

		// Set the momentum of the HpsParticle
		hps_particle->setMomentum(particle->getMomentum()); 

		// Set the mass of the HpsParticle
		hps_particle->setMass(particle->getMass());

		// If a particle has SVT tracks, add all track info to the HpsParticle 
		if(particle->getTracks().size() != 0){
		
			// Loop through all of the tracks associated with the particle 
			for(uint p_track_n = 0; p_track_n < particle->getTracks().size(); ++p_track_n){
				
				// Loop through all of the tracks in the HpsEvent and find the one
				// that matches the track associated with the particle
				for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){

					// Use the track chi^2 to find the match
					// TODO: Verify that the chi^2 is enough to find the match
					if(particle->getTracks()[p_track_n]->getChi2() == hps_event->getTrack(track_n)->getChi2()){
						hps_particle->addTrack(hps_event->getTrack(track_n));
                        
                        // If the particle is a final state particle, add a
                        // reference from the corresponding track to the particle
                        if (collection_type == HpsEvent::FINAL_STATE_PARTICLES) { 
                            hps_event->getTrack(track_n)->setParticle(hps_particle); 
                        }
						break;
					}
				}
			}
		}

		// If a particle has Ecal clusters, add the cluster to the HpsParticle	
		if(particle->getClusters().size() != 0){

			// Loop through all of the clusters associated with the particle
			for(uint p_cluster_n = 0; p_cluster_n < particle->getClusters().size(); ++p_cluster_n){
				// Loop through all of the clusters in the HpsEvent and find the one 
				// that matches the cluster associated with the particle.
				for(int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n){

					// Use the cluster energy to find the match
					// TODO: Verify that the cluster enegy is enough to find a match
					if(particle->getClusters()[p_cluster_n]->getEnergy() == hps_event->getEcalCluster(cluster_n)->getEnergy()){
						hps_particle->addCluster(hps_event->getEcalCluster(cluster_n)); 
						break;
					}
				}		
			}
		}

		// Only add vertex information if the particle is not a final state particle
		if(collection_type == HpsEvent::FINAL_STATE_PARTICLES){
            
            // Set the PDG ID of the particle
            hps_particle->setPDG(particle->getParticleIDUsed()->getPDG());    
            continue;
        } 

		// Set the vertex position of the particle
		hps_particle->setVertexPosition(((IMPL::VertexImpl*) particle->getStartVertex())->getPosition()); 

		// If the particle has daughter particles, add the daughters to the HpsParticle
		if(particle->getParticles().size() != 0){
	
			//std::cout << "D particles: " << particle->getParticles().size() << std::endl;

			// Loop through all of the daughter particles associated with the particle
			for(uint pd_particle_n = 0; pd_particle_n < particle->getParticles().size(); ++pd_particle_n){
				
				// Loop through all of the final state particles in the HpsEvent and
				// find the one that matches the daughters associated with the particles
				for(int d_particle_n = 0; d_particle_n < hps_event->getNumberOfParticles(HpsEvent::FINAL_STATE_PARTICLES); ++d_particle_n){
					
					//
					if(particle->getParticles()[pd_particle_n]->getEnergy() == hps_event->getParticle(HpsEvent::FINAL_STATE_PARTICLES, d_particle_n)->getEnergy()){
						hps_particle->addParticle(hps_event->getParticle(HpsEvent::FINAL_STATE_PARTICLES, d_particle_n));
						break;	
					}
				}
			}	
		}
	}	
}
