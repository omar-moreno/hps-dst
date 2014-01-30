/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@version:	v 1.0
 *	@date:		January 28, 2014
 *
 */

#ifndef _ECAL_HIT_H_
#define _ECAL_HIT_H_

//--- ROOT ---//
//------------//
#include <TObject.h>

//--- HPS Event ---//
//-----------------//
#include <CalorimeterHit.h>

class EcalHit : public CalorimeterHit, public TObject { 

	public: 
		EcalHit(); 
		~EcalHit();
		
		void Clear(Option_t* option=""); 
		
		void setPosition(const std::vector<double>); 
		void setEnergy(const double energy){ this->energy = energy; }; 

		std::vector<double> getPosition() const; 
		double getEnergy() const { return energy; };  

		ClassDef(EcalHit, 1); 

	private: 

		double energy;

		double hit_x; 
		double hit_y; 
		double hit_z; 
};

#endif
