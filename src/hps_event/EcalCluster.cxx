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
	  hit_time(0), seed_energy(0), seed_x(0), seed_y(0)
{}

EcalCluster::EcalCluster(const EcalCluster &ecalClusterObj)
	: TObject(), n_ecal_hits(ecalClusterObj.n_ecal_hits),
	  x(ecalClusterObj.x), y(ecalClusterObj.y), z(ecalClusterObj.z),
	  energy(ecalClusterObj.energy), hit_time(ecalClusterObj.hit_time),
	  seed_energy(ecalClusterObj.seed_energy),
	  seed_x(ecalClusterObj.seed_x),
	  seed_y(ecalClusterObj.seed_y)
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
	this->seed_energy = ecalClusterObj.seed_energy;
	this->seed_x = ecalClusterObj.seed_x;
	this->seed_y = ecalClusterObj.seed_y;

	return *this;
}

void EcalCluster::Clear(Option_t* /*option*/)
{
    TObject::Clear();
}

void EcalCluster::setClusterPosition(double* position)
{
    this->x = position[0];
    this->y = position[1];
    this->z = position[2];
}

void EcalCluster::setSeedPosition(double* position)
{
	this->seed_x = position[0];
	this->seed_y = position[1];
	this->seed_z = position[2];
}

