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
					 index_x(0), index_y(0),
					 x(0), y(0), z(0), energy(0)
{}

EcalHit::~EcalHit()
{
	Clear(); 
}

void EcalHit::Clear(Option_t* /* options */)
{
	TObject::Clear(); 
}

void EcalHit::setPosition(const double* position)
{
	x = position[0];
	y = position[1];
	z = position[2];
}

void EcalHit::setCrystalIndices(int index_x, int index_y)
{
	this->index_x = index_x;
	this->index_y = index_y;
}

std::vector<double> EcalHit::getPosition() const
{
	std::vector<double> position(3,0);
	position[0] = x;
	position[1] = y;
	position[2] = z;
	return position;
}
