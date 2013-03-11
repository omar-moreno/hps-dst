/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#include <SvtTrack.h>

ClassImp(SvtTrack)

SvtTrack::SvtTrack()
	: 	TObject(), svt_hits(new TRefArray()),
	  	n_hits(0), px(0), py(0), pz(0), d0(0), phi(0), omega(0),
	  	tan_lambda(0), z0(0)

{}

SvtTrack::SvtTrack(const SvtTrack &svtTrackObj)
	:	TObject(), svt_hits(new TRefArray()),
		px(svtTrackObj.px), py(svtTrackObj.py), pz(svtTrackObj.pz), d0(svtTrackObj.d0),
		phi(svtTrackObj.phi), omega(svtTrackObj.omega),
		tan_lambda(svtTrackObj.tan_lambda), z0(svtTrackObj.z0)
{
	*svt_hits = *svtTrackObj.svt_hits;
}

SvtTrack::~SvtTrack()
{
	Clear();
	delete svt_hits;
}

SvtTrack &SvtTrack::operator=(const SvtTrack &svtTrackObj)
{
	// Check for self-assignment
	if(this == &svtTrackObj) return *this;

	TObject::operator=(svtTrackObj);
	Clear();
	delete svt_hits;

	this->px = svtTrackObj.px;
	this->py = svtTrackObj.py;
	this->pz = svtTrackObj.pz;
	this->d0 = svtTrackObj.d0;
	this->phi = svtTrackObj.phi;
	this->omega = svtTrackObj.omega;
	this->tan_lambda = svtTrackObj.tan_lambda;
	this->z0 = svtTrackObj.z0;

	svt_hits = new TRefArray();
	*svt_hits = *svtTrackObj.svt_hits;

	return *this;
}

void SvtTrack::Clear(Option_t* /* option */)
{
    TObject::Clear(); 
    svt_hits->Delete(); 
    n_hits = 0; 
}

void SvtTrack::setTrackParameters(double d0, double phi, double omega,
		double tan_lambda, double z0)
{
    this->d0 		 = d0;
    this->phi 		 = phi;
    this->omega 	 = omega;
    this->tan_lambda = tan_lambda;
    this->z0 		 = z0;
}

void SvtTrack::setMomentum(double px, double py, double pz)
{
	this->px = px;
	this->py = py;
	this->pz = pz;
}


void SvtTrack::addHit(SvtHit* hit)
{
    ++n_hits; 
    svt_hits->Add(hit); 
}

