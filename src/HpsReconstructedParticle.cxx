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
	:	TObject(),	tracks(new TRefArray()), ecal_clusters(new TRefArray())
{}

HpsReconstructedParticle::HpsReconstructedParticle(
		const HpsReconstructedParticle &recoParticleObj) 
	: TObject(), tracks(new TRefArray()), ecal_clusters(new TRefArray()) 
{
	*tracks = *recoParticleObj.tracks; 
	*ecal_clusters = *recoParticleObj.ecal_clusters;
}

HpsReconstructedParticle::~HpsReconstructedParticle()
{
	Clear(); 
	delete tracks; 
	delete ecal_clusters; 
}

HpsReconstructedParticle &HpsReconstructedParticle::operator=(
		const HpsReconstructedParticle &recoParticleObj)
{
	// Check for self-assignment
	if(this == &recoParticleObj) return *this; 

	TObject::operator=(recoParticleObj);
	Clear(); 
	delete tracks; 
	delete ecal_clusters; 

	tracks = new TRefArray(); 
	*tracks = *recoParticleObj.tracks; 
	ecal_clusters = new TRefArray(); 
	*ecal_clusters = *recoParticleObj.ecal_clusters; 

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


