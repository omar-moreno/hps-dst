/**
 *	@section purpose: write GBL input data to DST
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@date: Feb. 12, 2014
 *	@version: 1.0
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *
 */
#ifndef __GBL_DATA_WRITER_H__
#define __GBL_DATA_WRITER_H__

//--- C++ ---//
//-----------//

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- HPS Event ---//
//-----------------//

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

};


#endif // __GBL_DATA_WRITER_H__
