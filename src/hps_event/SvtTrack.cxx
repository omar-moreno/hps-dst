/**
 * @file: SvtTrack.cxx
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section Institution \n
 *          Santa Cruz Institute for Particle Physics \n
 *          University of California, Santa Cruz
 * @date: February 19, 2013
 */

#include <SvtTrack.h>

ClassImp(SvtTrack)

SvtTrack::SvtTrack()
    : TObject(), 
      svt_hits(new TRefArray()),
      n_hits(0),
      d0(0),
      phi(0),
      omega(0),
      tan_lambda(0),
      z0(0),
      chi_squared(0),
      track_time(0),
      l1_isolation(0),
      l2_isolation(0) {
}

SvtTrack::SvtTrack(const SvtTrack &svtTrackObj)
    : TObject(),
      svt_hits(new TRefArray()),
      d0(svtTrackObj.d0),
      phi(svtTrackObj.phi),
      omega(svtTrackObj.omega),
      tan_lambda(svtTrackObj.tan_lambda),
      z0(svtTrackObj.z0),
      chi_squared(svtTrackObj.chi_squared),
      track_time(svtTrackObj.track_time),
      l1_isolation(svtTrackObj.l1_isolation),
      l2_isolation(svtTrackObj.l2_isolation) {
    
    *svt_hits = *svtTrackObj.svt_hits;
}


SvtTrack &SvtTrack::operator=(const SvtTrack &svtTrackObj) {
    
    // Check for self-assignment
    if(this == &svtTrackObj) return *this;

    TObject::operator=(svtTrackObj);
    Clear();
    delete svt_hits;

    this->d0 = svtTrackObj.d0;
    this->phi = svtTrackObj.phi;
    this->omega = svtTrackObj.omega;
    this->tan_lambda = svtTrackObj.tan_lambda;
    this->z0 = svtTrackObj.z0;
    this->chi_squared = svtTrackObj.chi_squared;
    this->track_time = svtTrackObj.track_time;
    this->l1_isolation = svtTrackObj.l1_isolation;
    this->l2_isolation = svtTrackObj.l2_isolation;

    svt_hits = new TRefArray();
    *svt_hits = *svtTrackObj.svt_hits;

    return *this;
}

SvtTrack::~SvtTrack() {
    Clear();
    delete svt_hits;
}

void SvtTrack::Clear(Option_t* /* option */) {
    TObject::Clear(); 
    svt_hits->Delete(); 
    n_hits = 0; 
}

void SvtTrack::setTrackParameters(double d0, double phi, double omega,
                                  double tan_lambda, double z0) {
    this->d0         = d0;
    this->phi        = phi;
    this->omega      = omega;
    this->tan_lambda = tan_lambda;
    this->z0         = z0;
}

void SvtTrack::addHit(SvtHit* hit) {
    ++n_hits; 
    svt_hits->Add(hit); 
}

TRefArray* SvtTrack::getSvtHits() const {
    return svt_hits;
}
