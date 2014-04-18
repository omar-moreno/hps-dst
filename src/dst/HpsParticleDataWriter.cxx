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

}

HpsParticleDataWriter::~HpsParticleDataWriter()
{
	delete particles;
	delete particle;
	delete hps_particle;
}

void HpsParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
    // Get the collection of particles from the event. If the event doesn't 
    // have particles, return;
    try { 
    
		particles = (IMPL::LCCollectionVec*) event->getCollection(fs_particles_collection_name);
		
		for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){
		
			particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

			hps_particle = hps_event->addFSParticle();

			if(particle->getClusters().size() > 0){
				for(int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n){
					if(particle->getClusters()[0]->getEnergy() == hps_event->getEcalCluster(cluster_n)->getClusterEnergy()){
						hps_particle->setEcalCluster(hps_event->getEcalCluster(cluster_n)); 
					}
				}
			}

			if(particle->getTracks().size() > 0){
				
				for(int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n){
					if(particle->getTracks()[0]->getChi2() == hps_event->getTrack(track_n)->getChi2()){
						hps_particle->setTrack(hps_event->geTrack(track_n));
					}
				}
			}	
		}
		
	} catch(DataNotAvailableException std::exception) { 
			
	}
	
	try { 
	
		particles = (IMPL::LCCollectionVec*) event->getCollection(uc_vtx_particles_collection_name);
		
		for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){
		
			particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

			hps_particle = hps_event->addVtxParticle(HpsEvent::UC_VTX_PARTICLES_INDEX);	
		}
	
	} catch(DataNotAvailableException std::exception) { 
			
	}

	try {	
		particles = (IMPL::LCCollectionVec*) event->getCollection(bsc_vtx_particles_collection_name);
		
		for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){
		
			particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

			hps_particle = hps_event->addVtxParticle(HpsEvent::BSC_VTX_PARTICLES_INDEX);	
		}
		
	} catch(DataNotAvailableException std::exception) { 
	
	}

	try {
		particles = (IMPL::LCCollectionVec*) event->getCollection(tc_vtx_particles_collection_name);
		
		for(int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n){
		
			particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

			hps_particle = hps_event->addVtxParticle(HpsEvent::TC_VTX_PARTICLES_INDEX);	
		}

	} catch(DataNotAvailableException std::exception) { 
	
    }
}
