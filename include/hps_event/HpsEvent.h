/**
 *
 * @file HpsEvent.h
 * @brief Event class used to encapsulate event information and physics 
 *        collections.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date February 19, 2013
 *
 */

#ifndef _HPS_EVENT_H_
#define _HPS_EVENT_H_

//-----------//
//--- C++ ---//
//-----------//
#include <iostream>
#include <assert.h>

//-----------//
//-- ROOT ---//
//-----------//
#include <TObject.h>
#include <TClonesArray.h>

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>
#include <EcalCluster.h> 
#include <EcalHit.h>
#include <HpsParticle.h>
#include <HpsMCParticle.h>
#include <GblTrackData.h>
#include <GblStripData.h>
#include <GblTrack.h>

class HpsEvent : public TObject { 

    // TODO: Add documentation

    public:

        /** Constructor */
        HpsEvent();

        /** 
         * Copy Constructor
         *
         * @param hpsEventObj An HpsEvent object 
         */
        HpsEvent(const HpsEvent &hpsEventObj);

        /** Destructor */
        ~HpsEvent();    
        
        /**
         * Copy assignment operator 
         *
         * @param hpsEventObj An HpsEvent object
         */ 
        HpsEvent &operator=(const HpsEvent &hpsEventObj);

        /** */
        void Clear(Option_t *option="");
        
        /** */
        SvtTrack*       addTrack();
        
        /** */
        SvtHit*         addSvtHit();
        
        /** */
        EcalCluster*    addEcalCluster();
        
        /** */
        EcalHit*        addEcalHit();
        
        /**
         * Create an {@link HpsParticle} object with the given 
         * {@link HpsParticle::ParticleType} and add it to the
         * event.
         *
         * @param type The type of particle that is being requested e.g. 
         *             HpsParticle::FINAL_STATE_PARTICLE.
         * @return A pointer to the newly created HpsParticle object. 
         */
        HpsParticle*    addParticle(HpsParticle::ParticleType type); 
        
        /** */
        HpsMCParticle*  addHpsMCParticle();
        
        /** */
        GblTrack*       addGblTrack();
        
        /** */
        GblTrackData*   addGblTrackData();
        
        /** */
        GblStripData*   addGblStripData();

        /**
         *
         */
        void setEventNumber(int event_number){ this->event_number = event_number; };
        
        /**
         *
         */
        void setRunNumber(int run_number){ this->run_number = run_number; };
       
        /**
         *
         */
        void setTriggerTimeStamp(const int trigger_time_stamp) { this->trigger_time_stamp = trigger_time_stamp; }; 
        /**
         *
         */
        void setSingle0Trigger(const int single0_trigger) { this->single0_trigger = single0_trigger; };

        /**
         *
         */
        void setSingle1Trigger(const int single1_trigger) { this->single1_trigger = single1_trigger; };

        /**
         *
         */
        void setPair0Trigger(const int pair0_trigger) { this->pair0_trigger = pair0_trigger; };
        
        /**
         *
         */
        void setPair1Trigger(const int pair1_trigger) { this->pair1_trigger = pair1_trigger; };

        /**
         *
         */
        void setPulserTrigger(const int pulser_trigger) { this->pulser_trigger = pulser_trigger; };
       
        /**
         *
         */ 
        int getEventNumber() const  { return event_number; };
        
        /**
         *
         */ 
        int getRunNumber() const  { return run_number; };

        /**
         *
         */
        bool isSingle0Trigger() const { return (bool) single0_trigger; };

        /**
         *
         */
        bool isSingle1Trigger() const { return (bool) single1_trigger; };

        /**
         *
         */
        bool isPair0Trigger() const { return (bool) pair0_trigger; };
        
        /**
         *
         */
        bool isPair1Trigger() const { return (bool) pair1_trigger; };

        /**
         *
         */
        bool isPulserTrigger() const { return (bool) pulser_trigger; };


        /** */
        int getNumberOfTracks()         const  { return n_tracks; };
        
        /** */
        int getNumberOfSvtHits()        const  { return n_svt_hits; };
        
        /** */
        int getNumberOfEcalClusters()   const  { return n_ecal_clusters; };
        
        /**
         * Get the number of particles ({@link HpsParticle} objects) of the 
         * given {@link HpsParticle::ParticleType} in the event.
         *
         * @param type The type of particle that is being requested e.g. 
         *             HpsParticle::FINAL_STATE_PARTICLE.
         * @return The number of particles of the given type in the event.
         */
        int getNumberOfParticles(HpsParticle::ParticleType type) const; 
        int getNumberOfGblTracks()      const  { return n_gbl_tracks; };
        int getNumberOfGblTracksData()  const  { return n_gbl_tracks_data; };
        int getNumberOfGblStripData()   const  { return n_gbl_strips_data; };

        /** */
        SvtTrack*      getTrack(int);
        
        /** */
        SvtHit*        getSvtHit(int);
        
        /** */
        EcalCluster*   getEcalCluster(int);
        
        /** */
        EcalHit*       getEcalHit(int);
        
        /** */
        HpsMCParticle* getMCParticle(int);
        
        /**
         * Get the particle object ({@link HpsParticle}) of the given type and
         * at the given position in the container from the event.  See the class
         * {@link HpsParticle} for the type of particles that are available.
         *
         * @param type The type of particle that is being requested e.g. 
         *             HpsParticle::FINAL_STATE_PARTICLE.
         * @param particle_index The position of the particle in the container
         * @return An HpsParticle object of the given type and at the given 
         *         position in the container
         * @throws std::runtime_error if the given type is invalid
         *
         */
        HpsParticle*   getParticle(HpsParticle::ParticleType type, int particle_index); 
        
        /** */
        GblTrack*      getGblTrack(int);
        
        /** */
        GblTrackData*  getGblTrackData(int);
        
        /** */
        GblStripData*  getGblStripData(int);

        ClassDef(HpsEvent, 1);  

    private:

        /** Collection of SVT tracks */ 
        TClonesArray* tracks;                //->
        /** Collection of SVT 3D hits */
        TClonesArray* svt_hits;              //->
        /** Collection of Ecal clusters */
        TClonesArray* ecal_clusters;         //->
        /** Collection of Ecal hits */
        TClonesArray* ecal_hits;             //->
        /** Collection of final state particles */
        TClonesArray* fs_particles;          //->
        /** Collection of unconstrained v0 candidates */
        TClonesArray* uc_v0_candidates;      //->
        /** Collection of unconstrained Moller candidates */
        TClonesArray* uc_moller_candidates;  //->
        /** Collection of beam spot constrained v0 candidates */
        TClonesArray* bsc_v0_candidates;     //->
        /** Collection of beam spot constrained Moller candidates */
        TClonesArray* bsc_moller_candidates; //->
        /** Collection of target constrained v0 candidates */
        TClonesArray* tc_v0_candidates;      //->
        /** Collection of target constrained Moller candidates */
        TClonesArray* tc_moller_candidates;  //->
        /** Collection of Monte Carlo particles */
        TClonesArray* mc_particles;          //->
        /** Collection of GBL tracks */
        TClonesArray* gbl_tracks;            //->
        /** Collection of GBLTrackData Generic Objects */
        TClonesArray* gbl_tracks_data;       //->
        /** Collection of GBLStripClusterData Generic Objects */
        TClonesArray* gbl_strips_data;       //->

        // Event information
        int event_number;
        int run_number;
        int trigger_time_stamp;
        int single0_trigger;
        int single1_trigger;
        int pair0_trigger;
        int pair1_trigger;
        int pulser_trigger;

        int n_tracks;
        int n_svt_hits;
        int n_ecal_clusters;
        int n_ecal_hits;
        int n_fs_particles;
        int n_uc_v0_candidates;
        int n_uc_moller_candidates;
        int n_bsc_v0_candidates;
        int n_bsc_moller_candidates;
        int n_tc_v0_candidates;
        int n_tc_moller_candidates; 
        int n_mc_particles;
        int n_gbl_tracks;
        int n_gbl_tracks_data;
        int n_gbl_strips_data;
        
};

#endif


