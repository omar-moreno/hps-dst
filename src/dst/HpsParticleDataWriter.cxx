/**
 *
 * @file HpsParticleDataWriter.cxx
 * @brief Data writer used to convert LCIO ReconstructedParticle objects
 *        to {@link HpsParticle} objects and add them to an event.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date March 31, 2014
 *
 */

#include <HpsParticleDataWriter.h>

HpsParticleDataWriter::HpsParticleDataWriter() 
    : fs_particles_collection_name("FinalStateParticles"),
      uc_v0_candidates_collection_name("UnconstrainedV0Candidates"),
      uc_moller_candidates_collection_name("UnconstrainedMollerCandidates"),
      bsc_v0_candidates_collection_name("BeamspotConstrainedV0Candidates"),
      bsc_moller_candidates_collection_name("BeamspotConstrainedMollerCandidates"),
      tc_v0_candidates_collection_name("TargetConstrainedV0Candidates"),
      tc_moller_candidates_collection_name("TargetConstrainedMollerCandidates"),
      particle(NULL), 
      hps_particle(NULL) {

    

    // Create a mapping between a HpsParticle::ParticleType and the LCIO 
    // collection name
    // FIXME: This should really be used the LCIO ReconstructedParticle type. 
    //        Doing it this way is really ugly! 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::FINAL_STATE_PARTICLE, fs_particles_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::UC_V0_CANDIDATE, uc_v0_candidates_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::UC_MOLLER_CANDIDATE, uc_moller_candidates_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::BSC_V0_CANDIDATE, bsc_v0_candidates_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::BSC_MOLLER_CANDIDATE, bsc_moller_candidates_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::TC_V0_CANDIDATE, tc_v0_candidates_collection_name)); 
    particle_collections.insert(
            std::pair<HpsParticle::ParticleType, std::string>(HpsParticle::TC_MOLLER_CANDIDATE, tc_moller_candidates_collection_name)); 
}

HpsParticleDataWriter::~HpsParticleDataWriter() {
}

void HpsParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {

    // Iterate through all of the collections of LCIO ReconstructedParticle objects and
    // add them to the event (HpsEvent object).  If the collection isn't present in the
    // event, throw an exception. 
    for (auto const &particle_collection : particle_collections) { 
        
        // Get the collection from the event
        EVENT::LCCollection* particles = (EVENT::LCCollection*) event->getCollection(particle_collection.second);
       
        // Write the particle data to the event
        writeParticleData(particle_collection.first, particles, hps_event); 
    }
}

void HpsParticleDataWriter::writeParticleData(HpsParticle::ParticleType collection_type, 
        EVENT::LCCollection* particles, HpsEvent* hps_event) {
    
    // Loop through all of the particles in the event
    for (int particle_n = 0; particle_n < particles->getNumberOfElements(); ++particle_n) {

        // Get a particle from the LCEvent
        particle = (IMPL::ReconstructedParticleImpl*) particles->getElementAt(particle_n); 

        // Get a particle from the HpsEvent
        hps_particle = hps_event->addParticle(collection_type);

        // Set the charge of the HpsParticle    
        hps_particle->setCharge(particle->getCharge());

        // Set the HpsParticle type
        hps_particle->setType(particle->getType());  
        
        // Set the energy of the HpsParticle
        hps_particle->setEnergy(particle->getEnergy());

        // Set the momentum of the HpsParticle
        hps_particle->setMomentum(particle->getMomentum()); 

        // Set the mass of the HpsParticle
        hps_particle->setMass(particle->getMass());

        // If a particle has SVT tracks, add all track info to the HpsParticle 
        if (particle->getTracks().size() != 0) {
        
            // Loop through all of the tracks associated with the particle 
            for (uint p_track_n = 0; p_track_n < particle->getTracks().size(); ++p_track_n) {
                
                // Loop through all of the tracks in the HpsEvent and find the one
                // that matches the track associated with the particle
                for (int track_n = 0; track_n < hps_event->getNumberOfTracks(); ++track_n) {

                    // Use the track chi^2 to find the match
                    // TODO: Verify that the chi^2 is enough to find the match
                    if (particle->getTracks()[p_track_n]->getChi2() == hps_event->getTrack(track_n)->getChi2()) {
                        hps_particle->addTrack(hps_event->getTrack(track_n));
                        
                        // If the particle is a final state particle, add a
                        // reference from the corresponding track to the particle
                        if (collection_type == HpsParticle::FINAL_STATE_PARTICLE) { 
                            hps_event->getTrack(track_n)->setParticle(hps_particle); 
                        }
                        break;
                    }
                }
            }
        }

        // If a particle has Ecal clusters, add the cluster to the HpsParticle  
        if (particle->getClusters().size() != 0) {

            // Loop through all of the clusters associated with the particle
            for (uint p_cluster_n = 0; p_cluster_n < particle->getClusters().size(); ++p_cluster_n) {
                // Loop through all of the clusters in the HpsEvent and find the one 
                // that matches the cluster associated with the particle.
                for (int cluster_n = 0; cluster_n < hps_event->getNumberOfEcalClusters(); ++cluster_n) {

                    // Use the cluster energy to find the match
                    // TODO: Verify that the cluster enegy is enough to find a match
                    if (particle->getClusters()[p_cluster_n]->getEnergy() == hps_event->getEcalCluster(cluster_n)->getEnergy()) {
                        hps_particle->addCluster(hps_event->getEcalCluster(cluster_n)); 
                        break;
                    }
                }       
            }
        }

        // Only add vertex information if the particle is not a final state particle
        if (collection_type == HpsParticle::FINAL_STATE_PARTICLE) {
            
            // Set the PDG ID of the particle
            hps_particle->setPDG(particle->getParticleIDUsed()->getPDG());    
            continue;
        } 

        // Set the vertex position of the particle
        hps_particle->setVertexPosition(((IMPL::VertexImpl*) particle->getStartVertex())->getPosition()); 

        // Set the vertex chi2
        hps_particle->setVertexFitChi2(((IMPL::VertexImpl*) particle->getStartVertex())->getChi2()); 

        // If the particle has daughter particles, add the daughters to the HpsParticle
        if (particle->getParticles().size() != 0) {
    
            // Loop through all of the daughter particles associated with the particle
            for (uint pd_particle_n = 0; pd_particle_n < particle->getParticles().size(); ++pd_particle_n) {
                
                // Loop through all of the final state particles in the HpsEvent and
                // find the one that matches the daughters associated with the particles
                for (int d_particle_n = 0; d_particle_n < hps_event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE); ++d_particle_n) {
                    
                    //
                    if (particle->getParticles()[pd_particle_n]->getEnergy() == hps_event->getParticle(HpsParticle::FINAL_STATE_PARTICLE, d_particle_n)->getEnergy()) {
                        hps_particle->addParticle(hps_event->getParticle(HpsParticle::FINAL_STATE_PARTICLE, d_particle_n));
                        break;  
                    }
                }
            }   
        }
    }   
}
