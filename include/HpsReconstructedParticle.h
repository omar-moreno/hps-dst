/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       March 29, 2013
 */

#ifndef _HPS_RECONSTRUCTED_PARTICLE_H_
#define _HPS_RECONSTRUCTED_PARTICLE_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>

//---//
#include <SvtTrack.h>
#include <EcalCluster.h>

class HpsReconstructedParticle : public TObject { 

	public: 
		HpsReconstructedParticle(); 
		HpsReconstructedParticle(const HpsReconstructedParticle &recoParticleObj);
		virtual ~HpsReconstructedParticle();
		HpsReconstructedParticle &operator=(const HpsReconstructedParticle &recoParticleObj);

		void Clear(Option_t *option="");
		void addTrack(SvtTrack*); 
		void addCluster(EcalCluster*); 	
		
		ClassDef(HpsReconstructedParticle, 1);

	private:
		TRefArray *tracks; 
		TRefArray *ecal_clusters; 

};	// HpsReconstructedParticle

#endif // _HPS_RECONSTRUCTED_PARTICLE_H_
