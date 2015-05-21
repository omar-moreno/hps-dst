/**
 * @file: HpsParticle.h
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section Institution \n
 * 			Santa Cruz Institute for Particle Physics
 * 			University of California, Santa Cruz
 * @date: March 29, 2013
 */

#ifndef __HPS_PARTICLE_H__
#define __HPS_PARTICLE_H__

//------------//
//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRef.h>
#include <TRefArray.h>


// Forward declare HPS event classes
class SvtTrack;
class EcalCluster;

class HpsParticle : public TObject { 

	public:

        /**
         * Default Constructor
         */
		HpsParticle(); 

        /**
         * Copy Constructor
         *
         * @param particle_obj : HpsParticle that will be cloned
         */
		HpsParticle(const HpsParticle &particle_obj);
		
        /**
         * Destructor
         */
        ~HpsParticle();

        /**
         *
         */
		HpsParticle &operator=(const HpsParticle &);

        /**
         *
         */
		void Clear(Option_t *option="");

        /**
         * Add a reference to an SVT track associated with this particle.
         *
         * @param svt_track : SVT track associated with this particle
         */
		void addTrack(SvtTrack* svt_track);
        
        /**
         * Add a reference to an Ecal cluster associated with this particle.
         *
         * @param ecal_cluster : Ecal cluster associated with this particle
         */
		void addCluster(EcalCluster* ecal_cluster);
        
        /**
         * Add a daughter particle composing this particle.
         *
         * @param particle : Daughter particle composing this particle
         */
		void addParticle(HpsParticle* particle); 
        
        /**
         * Set the charge of the particle.
         *
         * @param charge : Particle charge
         */
        void setCharge(const int charge) { this->charge = charge; };
        
        /**
         * Set the particle ID.
         *
         * @param pdg : The particle ID
         */
	    void setPDG(const int pdg) { this->pdg = pdg; }; 
        
        /**
         * Set the energy of the particle in GeV.
         *
         * @param energy : The particle energy
         */
        void setEnergy(const double energy) { this->energy = energy; };
        
        /**
         * Set the invariant mass of the particle in GeV.
         *
         * @param mass : The invariant mass of the particle
         */
		void setMass(const double mass) { this->mass = mass; }; 
       
        /**
         * Set the momentum of the particle in GeV.
         *
         * @param momentum : The momentum of the particle
         */
        void setMomentum(const double* momentum); 

        /**
         * Set the vertex position of the particle.
         *
         * @param vtx_pos : The vertex position of the particle
         */
		void setVertexPosition(const float* vtx_pos);

        /**
         * Set the chi^2 of the vertex fit.
         *
         * @param vtx_fit_chi2 : The chi^2 of the vertex fit
         */
        void setVertexFitChi2(const double vtx_fit_chi2) { this->vtx_fit_chi2 = vtx_fit_chi2; };
        
        /**
         * Get the SVT tracks associated with this particle.
         *
         * @return An array of references to the tracks associated with this
         *         particle
         */
		TRefArray* getTracks() const; 
        
        /**
         * Get the Ecal clusters associated with this particle.
         *
         * @return An array of references to the Ecal clusters associated
         *         with this particle
         */
		TRefArray* getClusters() const;
        
        /**
         * Get the daughter particles composing this particle.
         *
         * @return An array of references to the daughter particles associated
         *         with this particle
         */
		TRefArray* getParticles() const; 
        
        /**
         * Get the charge of the particle.
         *
         * @return The particle charge
         */
        int getCharge() const { return charge; };
        
        /**
         * Get the particle ID.
         *
         * @return The particle ID
         */
        int getPDG() const { return pdg; }; 
        
        /**
         * Get the energy of the particle in GeV.
         *
         * @return The particle energy in GeV
         */
		double getEnergy() const { return energy; }; 
        
        /**
         * Get the invariant mass of the particle in GeV.
         *
         * @return The invariant mass of the particle in GeV
         */
		double Mass() const { return mass; }; 
        
        /**
         * Get the momentum of the particle in GeV.
         *
         * @return The momentum of the particle
         */
        std::vector<double> getMomentum() const;  
        
        /**
         * Get the vertex position of the particle.
         *
         * @return The vertex position of the particle
         */
        std::vector<double> getVertexPosition() const;

        /**
         * Get the chi^2 of the vertex fit.
         *
         * @return The chi^2 of the vertex fit
         */
        double getVertexFitChi2() const { return vtx_fit_chi2; }; 

		ClassDef(HpsParticle, 1);

	private:

		TRefArray* svt_tracks;
		TRefArray* ecal_clusters;
		TRefArray* particles; 
		
		int n_daughters;
        int charge; 
	    int pdg; 

        double px; 
        double py; 
        double pz;
		double vtx_x;
		double vtx_y;
		double vtx_z;
        double vtx_fit_chi2;
		double energy; 
		double mass; 

};	// HpsParticle

#endif // _HPS_PARTICLE_H_
