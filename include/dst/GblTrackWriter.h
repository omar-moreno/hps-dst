/**
 *	@section purpose: interface to GBL track refit 
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@date: Feb. 12, 2014
 *	@version: 1.0
 *
 */
#ifndef __GBL_TRACK_WRITER_H__
#define __GBL_TRACK_WRITER_H__

//--- DST ---/

//-- HPS Event --//
class HpsEvent;

class GblTrackWriter {

 public:
  GblTrackWriter();
  ~GblTrackWriter();
  
  void writeData(HpsEvent* hps_event);
  void setDebug(bool debug);
  void setBField(double b_field);
  
 private:
  bool m_debug;
  double m_b_field;
};



#endif
