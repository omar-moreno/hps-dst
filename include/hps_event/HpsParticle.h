/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @date:       March 29, 2013
 *
 */

#ifndef _HPS_PARTICLE_H_
#define _HPS_PARTICLE_H_

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

		void setTrack(SvtTrack*);
		void setCluster(EcalCluster*);	
		void setVertexPosition(const double*);

		SvtTrack* getTrack() const;
		EcalCluster* getEcalCluster() const;
        std::vector<double> getVertexPosition() const; 

		ClassDef(HpsParticle, 1);

	private:

		TRef svt_track;
		TRef ecal_cluster;

		double vtx_x;
		double vtx_y;
		double vtx_z;

};	// HpsParticle

#endif // _HPS_PARTICLE_H_
