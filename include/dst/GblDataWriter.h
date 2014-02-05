#ifndef __GBL_DATA_WRITER_H__
#define __GBL_DATA_WRITER_H__

//--- C++ ---//
//-----------//

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>

//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>


class GblDataWriter : public DataWriter {

 public:
  
  GblDataWriter();
  GblDataWriter(bool debug);
  ~GblDataWriter();
  
  void writeData(EVENT::LCEvent*, HpsEvent*);
  
 private:
  bool m_debug;
  GblTrackData* gbl_track_data;

};


#endif // __GBL_DATA_WRITER_H__
