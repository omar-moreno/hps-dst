/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#include <SvtHit.h>

ClassImp(SvtHit)

SvtHit::SvtHit()
	: TObject(), hit_layer(0), x(0), y(0), z(0), x_err(0), y_err(0), z_err(0)
{}

SvtHit::~SvtHit()
{
    Clear(); 
}

void SvtHit::Clear(Option_t* /* options */)
{
    TObject::Clear();  
}

void SvtHit::setHitPosition(const double* position)
{
    x = position[0]; 
    y = position[1];
    z = position[2]; 
}

void SvtHit::setHitPositionError(double hit_x_err, double hit_y_err, double hit_z_err)
{
    x_err = hit_x_err; 
    y_err = hit_y_err; 
    z_err = hit_z_err; 
}
