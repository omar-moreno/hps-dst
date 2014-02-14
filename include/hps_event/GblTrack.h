/**
 *
 * @author: 	Per Hansson Adrian <phansson@slac.stanford.edu>
 * @section institution
 * 				SLAC
 * @version:    v 0.1
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

    public:
        GblTrack();
        virtual ~GblTrack();
        ClassDef(GblTrack,1) //Track class for use with GBL
}; // GblTrack

#endif // _GBL_TRACK_H_
