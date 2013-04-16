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
	: TObject(), n_ecal_hits(0), x(0), y(0), z(0), energy(0),
	  hit_time(0), high_energy(0), x_high_energy(0), y_high_energy(0)
{}

EcalCluster::EcalCluster(const EcalCluster &ecalClusterObj)
	: TObject(), n_ecal_hits(ecalClusterObj.n_ecal_hits),
	  x(ecalClusterObj.x), y(ecalClusterObj.y), z(ecalClusterObj.z),
	  energy(ecalClusterObj.energy), hit_time(ecalClusterObj.hit_time),
	  high_energy(ecalClusterObj.high_energy),
	  x_high_energy(ecalClusterObj.x_high_energy),
	  y_high_energy(ecalClusterObj.y_high_energy)
{}

EcalCluster::~EcalCluster()
{
    Clear(); 
}

EcalCluster &EcalCluster::operator=(const EcalCluster &ecalClusterObj)
{
	// Check for self-assignment
	if(this == &ecalClusterObj) return *this;

	TObject::operator=(ecalClusterObj);
	Clear();

	this->n_ecal_hits = ecalClusterObj.n_ecal_hits;
	this->x = ecalClusterObj.x;
	this->y = ecalClusterObj.y;
	this->z = ecalClusterObj.z;
	this->energy = ecalClusterObj.energy;
	this->hit_time = ecalClusterObj.hit_time;
	this->high_energy = ecalClusterObj.high_energy;
	this->x_high_energy = ecalClusterObj.x_high_energy;
	this->y_high_energy = ecalClusterObj.y_high_energy;

	return *this;
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

void EcalCluster::setHighestEnergyHitPosition(double x_high_energy, double y_high_energy)
{
	this->x_high_energy = x_high_energy;
	this->y_high_energy = y_high_energy;
}

