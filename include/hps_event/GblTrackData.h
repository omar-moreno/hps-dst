/**
 * @section purpose: Stores track information needed by GBL
 * @author: 	Per Hansson Adrian <phansson@slac.stanford.edu>
 * @version:    v1.0
 * @date:       February 3, 2014
 */

#ifndef _GBL_TRACK_DATA_H_
#define _GBL_TRACK_DATA_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>
#include <TMatrixD.h>

//---//
#include <TRefArray.h>

#include <GblStripData.h>
class GblStripData;

class GblTrackData : public TObject {
  
 private:
  TRefArray* m_gbl_strip_hits;
  int n_gbl_strip_hits;
  double m_kappa;
  double m_theta;
  double m_phi;
  double m_d0;
  double m_z0;
  TMatrixD m_prjPerToCl;
  
 public:
  GblTrackData();
  virtual ~GblTrackData();
  void Clear(Option_t *option="");
  void addStrip(GblStripData* strip);
  
  void setTrackParameters(double kappa, double theta, double phi, double d0, double z0) {
    m_kappa = kappa;
    m_theta = theta;
    m_phi = phi;
    m_d0 = d0;
    m_z0 = z0;
  }
  
  void setPrjPerToCl(const unsigned int& row, const unsigned int& col, const double& val) {
    m_prjPerToCl[row][col] = val;
  }
  
  int getNStrips() const { return n_gbl_strip_hits;}
  GblStripData* getStrip(const int& i) const {
    //TObject* obj = m_gbl_strip_hits->At(i);
    //GblStripData* strip = (GblStripData*)obj;
    //return strip;
    return static_cast<GblStripData*>(m_gbl_strip_hits->At(i));
  }
  double getKappa() const { return m_kappa;}
  double getTheta() const { return m_theta;}
  double getPhi() const { return m_phi;}
  double getD0() const { return m_d0;}
  double getZ0() const { return m_z0;}
  TMatrixD getPrjPerToCl() const { return m_prjPerToCl; }
  std::string toString() const;
  
  ClassDef(GblTrackData,1) //Track information needed by GBL
    
}; // GblTrackData
    
    

#endif // _GBL_TRACK_DATA_H_
