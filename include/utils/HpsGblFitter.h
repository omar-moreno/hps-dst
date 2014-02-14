/**
 *	@section purpose: 
 *		Creates an HPS Data Summary Tape
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: December 20, 2013
 *	@version: 1.0
 *
 */
#ifndef __HPS_GBL_FITTER_H__
#define __HPS_GBL_FITTER_H__

//--- C++ ---//

//--- HPS GBL ---//
class GblTrackData;
class TRandom;




class HpsGblFitter {

 public:
  HpsGblFitter(double bz);
  ~HpsGblFitter();
  void Fit(const GblTrackData* track);  
  void Clear();
  void SetDebug(bool debug);
  bool GetDebug();
 private:
  double m_Bz;
  double m_bfac;// for Bz in Tesla, momentum in GeV and Radius in mm
  TRandom *m_r;
  bool m_debug;
  
};

#endif
