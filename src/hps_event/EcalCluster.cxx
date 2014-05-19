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
	: TObject(), ecal_hits(new TRefArray()),
	  n_ecal_hits(0), x(0), y(0), z(0), energy(0),
	  hit_time(0), m2(0), m3(0)
{}

EcalCluster::EcalCluster(const EcalCluster &ecalClusterObj)
	: TObject(), ecal_hits(new TRefArray()), 
	  seed_hit(ecalClusterObj.seed_hit), n_ecal_hits(ecalClusterObj.n_ecal_hits),
	  x(ecalClusterObj.x), y(ecalClusterObj.y), z(ecalClusterObj.z),
	  energy(ecalClusterObj.energy), hit_time(ecalClusterObj.hit_time), 
	  m2(ecalClusterObj.m2), m3(ecalClusterObj.m3)
{
	*ecal_hits = *ecalClusterObj.ecal_hits; 
}

EcalCluster::~EcalCluster()
{
    Clear();
	delete ecal_hits; 	
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
	this->m2 = ecalClusterObj.m2; 
	this->m3 = ecalClusterObj.m3; 

	ecal_hits = new TRefArray(); 
	*ecal_hits = *ecalClusterObj.ecal_hits;

	seed_hit = ecalClusterObj.seed_hit; 	

	return *this;
}

void EcalCluster::Clear(Option_t* /*option*/)
{
    TObject::Clear();
    ecal_hits->Delete();
    n_ecal_hits = 0;
}

void EcalCluster::setPosition(const float* position)
{
    x = position[0];
    y = position[1];
    z = position[2];
}

void EcalCluster::addHit(EcalHit* hit)
{
	++n_ecal_hits;

	if(seed_hit.GetObject() == NULL
			|| (((EcalHit*) seed_hit.GetObject())->getEnergy() < hit->getEnergy())){
		seed_hit = hit;
	}

	ecal_hits->Add(hit);
}

std::vector<double> EcalCluster::getPosition() const
{
	std::vector<double> position(3, 0); 
	position[0] = x; 
	position[1] = y; 
	position[2] = z; 
	
	return position; 	
}

EcalHit* EcalCluster::getSeed() const
{
	return (EcalHit*) seed_hit.GetObject();
}

TRefArray* EcalCluster::getEcalHits() const
{
	return ecal_hits;
}

