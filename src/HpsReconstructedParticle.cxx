/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       March 29, 2013
 */

#include <HpsReconstructedParticle.h>

ClassImp(HpsReconstructedParticle)

HpsReconstructedParticle::HpsReconstructedParticle()
	:	TObject(), vtx_x(0), vtx_y(0), vtx_z(0)
{}

HpsReconstructedParticle::HpsReconstructedParticle(
		const HpsReconstructedParticle &reconParticleObj)
	: TObject(), vtx_x(reconParticleObj.vtx_x),
	  vtx_y(reconParticleObj.vtx_y), vtx_z(reconParticleObj.vtx_z),
	  track(reconParticleObj.track),
	  ecal_cluster(reconParticleObj.ecal_cluster)
{}

HpsReconstructedParticle::~HpsReconstructedParticle()
{
	Clear(); 
}

HpsReconstructedParticle &HpsReconstructedParticle::operator=(
		const HpsReconstructedParticle &reconParticleObj)
{
	// Check for self-assignment
	if(this == &reconParticleObj) return *this;

	TObject::operator=(reconParticleObj);
	Clear(); 

	this->vtx_x = reconParticleObj.vtx_x;
	this->vtx_y = reconParticleObj.vtx_y;
	this->vtx_z = reconParticleObj.vtx_z;
	this->track = reconParticleObj.track;
	this->ecal_cluster = reconParticleObj.ecal_cluster;

	return *this; 
}

void HpsReconstructedParticle::Clear(Option_t* /* option */)
{
	TObject::Clear(); 
}

void HpsReconstructedParticle::setTrack(SvtTrack* track)
{
	this->track = track;
}

void HpsReconstructedParticle::setEcalCluster(EcalCluster* ecal_cluster)
{
	this->ecal_cluster = ecal_cluster;
}

void HpsReconstructedParticle::setVertexPosition(double *vtx_pos)
{
	vtx_x = vtx_pos[0];
	vtx_y = vtx_pos[1];
	vtx_z = vtx_pos[2];
}

SvtTrack* HpsReconstructedParticle::getTrack()
{
	return (SvtTrack*) track.GetObject();
}

EcalCluster* HpsReconstructedParticle::getEcalCluster()
{
	return (EcalCluster*) ecal_cluster.GetObject();
}

