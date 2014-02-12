/**
 *	@author:	Omar Moreno <omoreno1@ucsc.edu>
 *	@section institution
 *				Santa Cruz Institute for Particle Physics
 *				University of California, Santa Cruz
 *	@version:	v 1.0
 *	@date:		February 11, 2014
 *
 */

//--- HPS Event ---//
//-----------------//
#include <HpsMCParticle.h>

ClassImp(HpsMCParticle)

HpsMCParticle::HpsMCParticle()
	: TObject(), pdg(0), charge(0), generator_status(0),
	  energy(0), mass(0), px(0), py(0), pz(0), epx(0), epy(0), epz(0)
{}

HpsMCParticle::HpsMCParticle(const HpsMCParticle &mcParticleObj)
	:TObject(), pdg(mcParticleObj.pdg), charge(mcParticleObj.charge),
	 generator_status(mcParticleObj.generator_status),
	 energy(mcParticleObj.energy), mass(mcParticleObj.mass),
	 px(mcParticleObj.px), py(mcParticleObj.py), pz(mcParticleObj.pz),
	epx(mcParticleObj.epx), epy(mcParticleObj.epy), epz(mcParticleObj.epz)
{
}

HpsMCParticle::~HpsMCParticle()
{
	Clear();
}

HpsMCParticle &HpsMCParticle::operator=(const HpsMCParticle &mcParticleObj)
{
	// Check for self-assignment
	if(this == &mcParticleObj) return *this;

	TObject::operator=(mcParticleObj);
	Clear();

	this->pdg = mcParticleObj.pdg;
	this->charge = mcParticleObj.charge;
	this->generator_status = mcParticleObj.generator_status;
	this->energy = mcParticleObj.energy;
	this->mass = mcParticleObj.mass;
	this->px = mcParticleObj.px;
	this->py = mcParticleObj.py;
	this->pz = mcParticleObj.pz;
	this->epx = mcParticleObj.epx;
	this->epy = mcParticleObj.epy;
	this->epz = mcParticleObj.epz;

	return *this;
}

void HpsMCParticle::Clear(Option_t* /*option*/)
{
	TObject::Clear();
}

void HpsMCParticle::setMomentum(const double* momentum)
{
	px = momentum[0];
	py = momentum[1];
	pz = momentum[2];
}

void HpsMCParticle::setEndpoint(const double* end_point)
{
	epx = end_point[0];
	epy = end_point[1];
	epz = end_point[2];
}

std::vector<double> HpsMCParticle::getMomentum() const
{
	std::vector<double> momentum(3, 0);
	momentum[0] = px;
	momentum[1] = py;
	momentum[2] = pz;
	return momentum;
 }

std::vector<double> HpsMCParticle::getEndpoint() const
{
	std::vector<double> end_point(3, 0);
	end_point[0] = epx;
	end_point[1] = epy;
	end_point[2] = epz;
	return end_point;
}
