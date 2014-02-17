/**
 *	@section purpose: 
 *		GBL track refit
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@version: 1.0
 *
 */
#ifndef __HPS_GBL_FITTER_H__
#define __HPS_GBL_FITTER_H__

//--- ROOT ---//
#include "TMatrixD.h" //Cannot do forward declaration since it's typedef'ed in GBL
class TRandom;

//--- DST ---//
class GblTrackData;


class HpsGblFitter {

  enum HpsGblFitStatus {
    OK,INVALIDTRAJ,ERROR
  };

 public:
  HpsGblFitter(double bz);
  ~HpsGblFitter();
  HpsGblFitStatus Fit(const GblTrackData* track);  
  void Clear();
  void SetDebug(bool debug);
  bool GetDebug();
 private:
  TMatrixD gblSimpleJacobianLambdaPhi(double ds, double cosl, double bfac);
  double m_Bz;
  double m_bfac;// for Bz in Tesla, momentum in GeV and Radius in mm
  TRandom *m_r;
  bool m_debug;
  
};

#endif
