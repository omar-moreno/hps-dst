/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@version:	v 1.0
 *	@date:		January 28, 2014
 *
 */

#include <EcalHit.h>

ClassImp(EcalHit)

EcalHit::EcalHit() : TObject(), energy(0), hit_x(0), hit_y(0), hit_z(0)
{}

EcalHit::~EcalHit()
{
	Clear(); 
}

void EcalHit::Clear(Option_t* /* options */)
{
	TObject::Clear(); 
}

void EcalHit::setPosition(const std::vector<double> position)
{
	hit_x = position[0];
	hit_y = position[1]; 
	hit_z = position[2]; 
}

std::vector<double> EcalHit::getPosition()
{
	std::vector<double> position(3, 0); 
	position[0] = hit_x; 
	position[1] = hit_y; 
	position[2] = hit_z; 
	
	return position; 	
}

