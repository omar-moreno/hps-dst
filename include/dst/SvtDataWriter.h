/**
 * 
 * @file SvtDataWriter.h
 * @brief Data writer used to convert LCIO Tracks and TrackerHits
 *        to SvtTracks and SvtHits and add them to the HPS event.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *           Santa Cruz Institute for Particle Physics
 *           University of California, Santa Cruz
 *  @date January 2, 2013
 *
 */

#ifndef __SVT_DATA_WRITER_H__
#define __SVT_DATA_WRITER_H__

//-----------//
//--- C++ ---//
//-----------//
#include <stdexcept>
#include <vector>
#include <map>

//-----------//
//--- DST ---//
//-----------//
#include <DataWriter.h>
#include <DstUtils.h>

//-------------//
//--- Utils ---//
//-------------//
#include <TrackUtils.h>

//------------//
//--- LCIO ---//
//------------//
#include <EVENT/LCIO.h>
#include <EVENT/Track.h>
#include <EVENT/LCCollection.h>
#include <IMPL/LCGenericObjectImpl.h>
#include <UTIL/LCRelationNavigator.h>

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>

class SvtDataWriter : public DataWriter { 

    public:

        /**
         * Constructor
         */        
        SvtDataWriter();

        /**
         * Default constructor
         */ 
        ~SvtDataWriter(); 

        /**
         * Make SvtTracks and SvtHits out of LCIO Tracks and TrackerHits
         * and write them to an HpsEvent.
         *
         * @param lc_event  LCSim event from which the Tracks and 
         *                  TrackerHit collections are retrieved.
         * @param hps_event  HpsEvent to which the SvtTracks and SvtHits 
         *                   will be written to 
         */        
        void writeData(EVENT::LCEvent*, HpsEvent*);

    private:

        /** 
         * The name of the collection of 'partial tracks' i.e. tracks which are
         * a subset of other tracks in the event.
         */
        static const std::string PARTIAL_TRACKS_COL_NAME;  

        /** The name of the collection of TrackData GenericObjects */ 
        static const std::string TRACK_DATA_COL_NAME;

        /** 
         * The name of the collection containing Track to TrackData 
         * LCRelations
         */
        static const std::string TRACK_DATA_REL_COL_NAME;

}; // SvtDataWriter

#endif // __SVT_DATA_WRITER_H__
