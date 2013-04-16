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
	:	TObject(),	tracks(new TRefArray()), ecal_clusters(new TRefArray()),
	 	vtx_x(0), vtx_y(0), vtx_z(0)
{}

HpsReconstructedParticle::HpsReconstructedParticle(
		const HpsReconstructedParticle &reconParticleObj)
	: TObject(), tracks(new TRefArray()), ecal_clusters(new TRefArray()),
	  vtx_x(reconParticleObj.vtx_x), vtx_y(reconParticleObj.vtx_y),
	  vtx_z(reconParticleObj.vtx_z)
{
	*tracks = *reconParticleObj.tracks;
	*ecal_clusters = *reconParticleObj.ecal_clusters;
}

HpsReconstructedParticle::~HpsReconstructedParticle()
{
	Clear(); 
	delete tracks; 
	delete ecal_clusters; 
}

HpsReconstructedParticle &HpsReconstructedParticle::operator=(
		const HpsReconstructedParticle &reconParticleObj)
{
	// Check for self-assignment
	if(this == &reconParticleObj) return *this;

	TObject::operator=(reconParticleObj);
	Clear(); 
	delete tracks; 
	delete ecal_clusters; 

	this->vtx_x = reconParticleObj.vtx_x;
	this->vtx_y = reconParticleObj.vtx_y;
	this->vtx_z = reconParticleObj.vtx_z;

	tracks = new TRefArray(); 
	*tracks = *reconParticleObj.tracks;
	ecal_clusters = new TRefArray(); 
	*ecal_clusters = *reconParticleObj.ecal_clusters;

	return *this; 
}

void HpsReconstructedParticle::Clear(Option_t* /* option */)
{
	TObject::Clear(); 
	tracks->Delete(); 
	ecal_clusters->Delete(); 
}

void HpsReconstructedParticle::addTrack(SvtTrack* track)
{
	tracks->Add(track); 
}

void HpsReconstructedParticle::addCluster(EcalCluster* ecal_cluster)
{
	ecal_clusters->Add(ecal_cluster);
}

void HpsReconstructedParticle::setVertexPosition(double *vtx_pos)
{
	vtx_x = vtx_pos[0];
	vtx_y = vtx_pos[1];
	vtx_z = vtx_pos[2];
}

