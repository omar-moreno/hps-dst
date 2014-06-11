/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @date:       March 29, 2013
 */

#include <HpsParticle.h>

ClassImp(HpsParticle)

HpsParticle::HpsParticle()
	: TObject(), svt_tracks(new TRefArray()), ecal_clusters(new TRefArray()),
	  particles(new TRefArray()), n_daughters(0), charge(0), 
      px(0), py(0), pz(0), vtx_x(0), vtx_y(0), vtx_z(0), energy(0), mass(0)

{}

HpsParticle::HpsParticle(const HpsParticle &particleObj)
	: TObject(), svt_tracks(new TRefArray()), ecal_clusters(new TRefArray()),
	  particles(new TRefArray()),
	  n_daughters(particleObj.n_daughters), charge(particleObj.charge),
      px(particleObj.px), py(particleObj.py), pz(particleObj.pz), 
      vtx_x(particleObj.vtx_x), vtx_y(particleObj.vtx_y), vtx_z(particleObj.vtx_z),
	  energy(particleObj.energy), mass(particleObj.mass)
{
	*svt_tracks = *particleObj.svt_tracks;
	*ecal_clusters = *particleObj.ecal_clusters;	
	*particles = *particleObj.particles; 
}

HpsParticle::~HpsParticle()
{
	Clear();
    delete svt_tracks; 
	delete ecal_clusters; 
}

HpsParticle &HpsParticle::operator=(const HpsParticle &particleObj)
{
	// Check for self-assignment
	if(this == &particleObj) return *this;

	TObject::operator=(particleObj);
	Clear(); 

	this->n_daughters = particleObj.n_daughters;
    this->charge = particleObj.charge;

    this->px = particleObj.px; 
    this->py = particleObj.py; 
    this->pz = particleObj.pz; 
	this->vtx_x = particleObj.vtx_x;
	this->vtx_y = particleObj.vtx_y;
	this->vtx_z = particleObj.vtx_z;
	this->energy = particleObj.energy;
	this->mass = particleObj.mass; 

	svt_tracks = new TRefArray();
	*svt_tracks = *particleObj.svt_tracks;
	ecal_clusters = new TRefArray(); 
	*ecal_clusters = *particleObj.ecal_clusters;
	particles = new TRefArray(); 
	*particles = *particleObj.particles; 	

	return *this; 
}

void HpsParticle::Clear(Option_t* /* option */)
{
	TObject::Clear();
	svt_tracks->Delete();
	ecal_clusters->Delete(); 	
	n_daughters = 0; 	
}

void HpsParticle::addTrack(SvtTrack* svt_track)
{
	svt_tracks->Add(svt_track);
}

void HpsParticle::addCluster(EcalCluster* ecal_cluster)
{
	ecal_clusters->Add(ecal_cluster);
}

void HpsParticle::addParticle(HpsParticle* particle)
{
	++n_daughters;
	particles->Add(particle); 
}

void HpsParticle::setMomentum(const double* momentum)
{
    px = momentum[0];
    py = momentum[1];
    pz = momentum[2];
}

void HpsParticle::setVertexPosition(const float* vtx_pos)
{
	vtx_x = (double) vtx_pos[0];
	vtx_y = (double) vtx_pos[1];
	vtx_z = (double) vtx_pos[2];
}

TRefArray* HpsParticle::getTracks() const
{
	return svt_tracks;
}

TRefArray* HpsParticle::getClusters() const
{
	return ecal_clusters;
}

TRefArray* HpsParticle::getParticles() const 
{
	return particles; 
}

std::vector<double> HpsParticle::getMomentum() const
{
    std::vector<double> momentum(3,0);
    momentum[0] = px;
    momentum[1] = py;
    momentum[2] = pz;
    return momentum;
}

std::vector<double> HpsParticle::getVertexPosition() const
{
    std::vector<double> vertex(3,0); 
    vertex[0] = vtx_x; 
    vertex[1] = vtx_y; 
    vertex[2] = vtx_z; 
    return vertex; 
}

