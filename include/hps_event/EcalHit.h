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
		
		void setEnergy(const double energy){ this->energy = energy; }; 
		void setPosition(const double*);
		void setCrystalIndices(int, int);

		double getEnergy() const { return energy; };
 		std::vector<double> getPosition() const;
		int getXCrystalIndex() const { return index_x; };
		int getYCrystalIndex() const { return index_y; };

		ClassDef(EcalHit, 1);

	private: 

		int index_x;
		int index_y;
		int x;
		int y;
		int z;

		double energy;

}; // EcalHit

#endif // _ECAL_HIT_H_
