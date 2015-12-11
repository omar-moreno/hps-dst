/**
 *  @file   HpsEventBuilder.h
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   December 31, 2013
 *
 */

#ifndef __HPS_EVENT_BUILDER_H__
#define __HPS_EVENT_BUILDER_H__

//-----------//
//--- DST ---//
//-----------//
#include <EventBuilder.h>
#include <SvtDataWriter.h>
#include <EcalDataWriter.h>
#include <MCParticleDataWriter.h>
#include <HpsParticleDataWriter.h>
#include <TriggerData.h>

//------------//
//--- LCIO ---//
//------------//
#include <EVENT/LCGenericObject.h>
#include <IMPL/LCCollectionVec.h>

class HpsEventBuilder : public EventBuilder {

    public: 

        /** Constructor */
        HpsEventBuilder(); 

        /** Destructor */
        ~HpsEventBuilder(); 

        /**
         *  Write collections encapsulated by an LCEvent object to an 
         *  {@link HpsEvent} object.
         *
         *  @param lc_event LCEvent to process.
         *  @param hps_event The {@link HpsEvent} to write the LCEvent data to.
         */
        void makeHpsEvent(EVENT::LCEvent* lc_event, HpsEvent* hps_event); 

        /**
         * Write event specific variables to an {@link HpsEvent}.
         * 
         * @param lc_event  LCSim event from which the event information is
         *                  retrieved.
         * @param hps_event {@link HpsEvent} that the event information will
         *                  written to. 
         */
        void writeEventData(EVENT::LCEvent* lc_event, HpsEvent* hps_event); 

        /**
         *  Enable/disable the creation of an HpsEvent using recon files 
         *  containing ECal data only.
         *
         *  @param ecal_only - true to write ECal data only, false otherwise
         */    
        void writeEcalOnly(const bool ecal_only) { this->ecal_only = ecal_only; }; 

    private:

        SvtDataWriter* svt_writer; 
        EcalDataWriter* ecal_writer; 
        MCParticleDataWriter* mc_particle_writer;
        HpsParticleDataWriter* particle_writer; 
        TriggerData* hps_trigger_data;

        IMPL::LCCollectionVec* trigger_data;
        EVENT::LCGenericObject* trigger_datum;  

        bool ecal_only;

}; // HpsEventBuilder

#endif // __HPS_EVENT_BUILDER_H__
