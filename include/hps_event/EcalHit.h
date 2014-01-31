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

class EcalHit : public TObject, public CalorimeterHit {

	public:

		EcalHit(); 
		~EcalHit();
		
		void Clear(Option_t* option=""); 
		
		void setPosition(const int, const int, const int);
		void setEnergy(const double energy){ this->energy = energy; }; 
		void setCrystalIndices(int, int);

		std::vector<double> getPosition() const; 
		double getEnergy() const { return energy; };
		int getXCrystalIndex() const { return index_x; };
		int getYCrystalIndex() const { return index_y; };

		ClassDef(EcalHit, 1);

	private: 

		double energy;

		int index_x;
		int index_y;

		double hit_x; 
		double hit_y; 
		double hit_z; 
};

#endif
