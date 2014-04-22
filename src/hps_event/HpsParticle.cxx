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
	:	TObject(), vtx_x(0), vtx_y(0), vtx_z(0), n_daughters(0),
		svt_tracks(new TRefArray()), ecal_clusters(new TRefArray())
{}

HpsParticle::HpsParticle(const HpsParticle &particleObj)
	: TObject(), vtx_x(particleObj.vtx_x),
	  vtx_y(particleObj.vtx_y), vtx_z(particleObj.vtx_z),
	  n_daughters(particleObj.n_daughters),
	  svt_tracks(new TRefArray()), ecal_clusters(new TRefArray())
{
	*svt_tracks = *particleObj.svt_tracks;
	*ecal_clusters = *particleObj.ecal_clusters;	
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

	this->vtx_x = particleObj.vtx_x;
	this->vtx_y = particleObj.vtx_y;
	this->vtx_z = particleObj.vtx_z;
	
	svt_tracks = new TRefArray();
	*svt_tracks = *particleObj.svt_tracks;
	ecal_clusters = new TRefArray(); 
	*ecal_clusters = *particleObj.ecal_clusters;	

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
	n_daughters++;
	ecal_clusters->Add(ecal_cluster);
}

void HpsParticle::setVertexPosition(const double *vtx_pos)
{
	vtx_x = vtx_pos[0];
	vtx_y = vtx_pos[1];
	vtx_z = vtx_pos[2];
}

TRefArray* HpsParticle::getTracks() const
{
	return svt_tracks;
}

TRefArray* HpsParticle::getClusters() const
{
	return ecal_clusters;
}

std::vector<double> HpsParticle::getVertexPosition() const
{
    std::vector<double> vertex(3,0); 
    vertex[0] = vtx_x; 
    vertex[1] = vtx_y; 
    vertex[2] = vtx_z; 
    return vertex; 
}

