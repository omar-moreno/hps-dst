
#include <SvtHit.h>

ClassImp(SvtHit)

SvtHit::SvtHit(){}

SvtHit::~SvtHit()
{
    Clear(); 
}

void SvtHit::Clear(Option_t* /* options */)
{
    TObject::Clear();  
}

void SvtHit::setLayer(int layer)
{
    hit_layer = layer; 
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
