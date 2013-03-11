/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#include<EcalCluster.h>

ClassImp(EcalCluster)

EcalCluster::EcalCluster()
	: TObject(), n_ecal_hits(0), x(0), y(0), z(0), energy(0)
{}

EcalCluster::~EcalCluster()
{
    Clear(); 
}

void EcalCluster::Clear(Option_t* /*option*/)
{
    TObject::Clear();
}

void EcalCluster::setClusterPosition(double *position)
{
    x = position[0]; 
    y = position[1]; 
    z = position[2];
}

