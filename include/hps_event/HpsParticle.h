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
         *
         */
		void addTrack(SvtTrack* svt_track);
        
        /**
         *
         */
		void addCluster(EcalCluster* ecal_cluster);
        
        /**
         *
         */
		void addParticle(HpsParticle* particle); 
        
        /**
         *
         */
        void setCharge(const int charge){ this->charge = charge; };
        
        /**
         *
         */
	    void setPDG(const int pdg){ this->pdg = pdg; }; 
        
        /**
         *
         */
        void setEnergy(const double energy){ this->energy = energy; };
        
        /**
         *
         */
		void setMass(const double mass){ this->mass = mass; }; 
        
        /**
         *
         */
		void setVertexPosition(const float*);
        
        /**
         *
         */
        void setMomentum(const double*); 
        
        /**
         *
         */
		TRefArray* getTracks() const; 
        
        /**
         *
         */
		TRefArray* getClusters() const;
        
        /**
         *
         */
		TRefArray* getParticles() const; 
        
        /**
         *
         */
        int getCharge() const { return charge; };
        
        /**
         *
         */
        int getPDG() const { return pdg; }; 
        
        /**
         *
         */
		double getEnergy() const { return energy; }; 
        
        /**
         *
         */
		double Mass() const { return mass; }; 
        
        /**
         *
         */
        std::vector<double> getMomentum() const;  
        
        /**
         *
         */
        std::vector<double> getVertexPosition() const; 

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
		double energy; 
		double mass; 

};	// HpsParticle

#endif // _HPS_PARTICLE_H_
