/**
 * 
 * @file GblTrack.h
 * @brief Class used to describe an HPS SVT GBL track.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date December 11, 2015
 * 
 */

#ifndef __GBL_TRACK_H__
#define __GBL_TRACK_H__

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>

class GblTrack : public SvtTrack { 

    /** Constructor */
    GblTrack();

    ClassDef(GblTrack, 1) 
};

#endif // __GBL_TRACK_H__
