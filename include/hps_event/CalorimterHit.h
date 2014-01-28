/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@version:	v 1.0
 *	@date:		January 28, 2014
 *
 */

#ifndef _CALORIMETER_HIT_H_
#define _CALORIMETER_HIT_H_


class CalorimeterHit { 

	public: 

		virtual ~CalorimeterHit(){}; 

		virtual std::vector<double> getPosition() = 0;
		virtual double getEnergy() = 0; 
		
		virtual void setPosition(const std::vector<double>) = 0; 
		virtual void setEnergy(const double) = 0; 	

};

#endif // _CALORIMETER_HIT_H_
