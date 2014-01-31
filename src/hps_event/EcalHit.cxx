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

EcalHit::EcalHit() : TObject(),
					 energy(0), hit_x(0), hit_y(0), hit_z(0),
					 index_x(0), index_y(0)
{}

EcalHit::~EcalHit()
{
	Clear(); 
}

void EcalHit::Clear(Option_t* /* options */)
{
	TObject::Clear(); 
}

void EcalHit::setPosition(const int hit_x, const int hit_y, const int hit_z)
{
	this->hit_x = hit_x;
	this->hit_y = hit_y;
	this->hit_z = hit_z;
}

void EcalHit::setCrystalIndices(int index_x, int index_y)
{
	this->index_x = index_x;
	this->index_y = index_y;
}

std::vector<double> EcalHit::getPosition() const 
{
	std::vector<double> position(3, 0); 
	position[0] = hit_x; 
	position[1] = hit_y; 
	position[2] = hit_z; 
	
	return position; 	
}
