/**
 *
 * @author: 	phansson@slac.stanford.edu
 * @section purpose: GBL track information
 * @version:    v1.0
 * @date:       February 3, 2014
 */

#ifndef _GBL_TRACK_H_
#define _GBL_TRACK_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>

class GblTrack : public TObject {

 private:
  double m_kappa;
  double m_theta;
  double m_phi;
  double m_d0;
  double m_z0;
  double m_seed_kappa;
  double m_seed_theta;
  double m_seed_phi;
  double m_seed_d0;
  double m_seed_z0;
  double m_chi2;
  double m_ndf;

  
 public:
  GblTrack();
  virtual ~GblTrack();
  void SetSeedTrackParameters(double kappa, double theta, double phi, double d0, double z0) {
    m_seed_kappa = kappa;
    m_seed_theta = theta;
    m_seed_phi = phi;
    m_seed_d0 = d0;
    m_seed_z0 = z0;
  }
  void SetTrackParameters(double kappa, double theta, double phi, double d0, double z0) {
    m_kappa = kappa;
    m_theta = theta;
    m_phi = phi;
    m_d0 = d0;
    m_z0 = z0;
  }
  void SetChi2(double chi2) {
    m_chi2 = chi2;
  }
  void SetNdf(double ndf) {
    m_ndf = ndf;
  }
  double getKappa() const { return m_kappa;}
  double getTheta() const { return m_theta;}
  double getPhi() const { return m_phi;}
  double getD0() const { return m_d0;}
  double getZ0() const { return m_z0;}
  double getChi2() const { return m_chi2;}
  double getNdf() const { return m_ndf;}



        ClassDef(GblTrack,1) //Track class for use with GBL
}; // GblTrack

#endif // _GBL_TRACK_H_
