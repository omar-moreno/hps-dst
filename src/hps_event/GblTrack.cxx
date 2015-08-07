/**
 * @file GblTrack.cxx
 * @brief Class used to encapsulate GBL track information.
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *          SLAC
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date February 3, 2014
 */

#include <GblTrack.h>

ClassImp(GblTrack)

GblTrack::GblTrack() 
    : TObject(),
      seed_track(NULL), 
      cov(5, 5),
      chi2(-1.) {
}

GblTrack::~GblTrack() {
}

void GblTrack::setSeedTrackParameters(double kappa, double theta, double phi, double d0, double z0) {
   seed_kappa = kappa;
   seed_theta = theta;
   seed_phi = phi;
   seed_d0 = d0;
   seed_z0 = z0;
}

void GblTrack::setTrackParameters(double C, double th, double phi0, double dca, double z) {
   kappa = C;
   theta = th;
   phi = phi0;
   d0 = dca;
   z0 = z;
}

void GblTrack::setCov(const TMatrixD& mat) {
  cov = mat;
}

void GblTrack::setChi2(double c) {
chi2 = c;
}

void GblTrack::setNdf(double ndof) {
   ndf = ndof;
}

void GblTrack::setMomentumVector(double x, double y, double z) {
   px = x;
   py = y;
   pz = z;
}

void GblTrack::print() {
     std::cout << "GblTrack: " << std::endl;
     std::cout << "seed        (kappa,theta,phi,d0,z0): " << getKappa() 
               << "," << getTheta() 
               << "," << getPhi() 
               << "," << getD0() 
               << "," << getZ0() << std::endl;
     std::cout << "seed params (kappa,theta,phi,d0,z0): " << getSeedKappa() << "," 
               << getSeedTheta() << "," 
               << getSeedPhi() << "," 
               << getSeedD0() << "," 
               << getSeedZ0() << std::endl;
}
