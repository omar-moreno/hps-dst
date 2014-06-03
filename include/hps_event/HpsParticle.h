/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @date:       March 29, 2013
 *
 */

#ifndef __HPS_PARTICLE_H__
#define __HPS_PARTICLE_H__

//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRef.h>

//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <EcalCluster.h>

class HpsParticle : public TObject { 

	public:

		HpsParticle(); 
		HpsParticle(const HpsParticle &);
		~HpsParticle();
		HpsParticle &operator=(const HpsParticle &);

		void Clear(Option_t *option="");

		void addTrack(SvtTrack* svt_track);
		void addCluster(EcalCluster* ecal_cluster);

        void setCharge(const int charge){ this->charge = charge; };
        void setMomentum(const double*); 
		void setVertexPosition(const double*);
		
		TRefArray* getTracks() const; 
		TRefArray* getClusters() const;
        int getCharge() const { return charge; };
        std::vector<double> getMomentum() const;  
        std::vector<double> getVertexPosition() const; 

		ClassDef(HpsParticle, 1);

	private:

		TRefArray* svt_tracks;
		TRefArray* ecal_clusters;
		
		int n_daughters;
        int charge; 
		
        double px; 
        double py; 
        double pz; 
		double vtx_x;
		double vtx_y;
		double vtx_z;

};	// HpsParticle

#endif // _HPS_PARTICLE_H_
