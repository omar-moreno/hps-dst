/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       March 29, 2013
 */

#include <HpsParticle.h>

ClassImp(HpsParticle)

HpsParticle::HpsParticle()
	:	TObject(), vtx_x(0), vtx_y(0), vtx_z(0)
{}

HpsParticle::HpsParticle(const HpsParticle &reconParticleObj)
	: TObject(), vtx_x(reconParticleObj.vtx_x),
	  vtx_y(reconParticleObj.vtx_y), vtx_z(reconParticleObj.vtx_z),
	  svt_track(reconParticleObj.svt_track),
	  ecal_cluster(reconParticleObj.ecal_cluster)
{}

HpsParticle::~HpsParticle()
{
	Clear(); 
}

HpsParticle &HpsParticle::operator=(const HpsParticle &reconParticleObj)
{
	// Check for self-assignment
	if(this == &reconParticleObj) return *this;

	TObject::operator=(reconParticleObj);
	Clear(); 

	this->vtx_x = reconParticleObj.vtx_x;
	this->vtx_y = reconParticleObj.vtx_y;
	this->vtx_z = reconParticleObj.vtx_z;
	this->svt_track = reconParticleObj.svt_track;
	this->ecal_cluster = reconParticleObj.ecal_cluster;

	return *this; 
}

void HpsParticle::Clear(Option_t* /* option */)
{
	TObject::Clear(); 
}

void HpsParticle::setTrack(SvtTrack* svt_track)
{
	this->svt_track = svt_track;
}

void HpsParticle::setCluster(EcalCluster* ecal_cluster)
{
	this->ecal_cluster = ecal_cluster;
}

void HpsParticle::setVertexPosition(const double *vtx_pos)
{
	vtx_x = vtx_pos[0];
	vtx_y = vtx_pos[1];
	vtx_z = vtx_pos[2];
}

SvtTrack* HpsParticle::getTrack() const
{
	return (SvtTrack*) svt_track.GetObject();
}

EcalCluster* HpsParticle::getEcalCluster() const
{
	return (EcalCluster*) ecal_cluster.GetObject();
}


std::vector<double> HpsParticle::getVertexPosition() const
{
    std::vector<double> vertex(3,0); 
    vertex[0] = vtx_x; 
    vertex[1] = vtx_y; 
    vertex[2] = vtx_z; 
    return vertex; 
}

