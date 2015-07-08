/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @date:       February 19, 2013
 */

#ifndef _HPS_EVENT_H_
#define	_HPS_EVENT_H_

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

    public:

		HpsEvent();
        HpsEvent(const HpsEvent &hpsEventoObj);
        virtual ~HpsEvent();    
        HpsEvent &operator=(const HpsEvent &hpsEventObj);

		enum collection_t { 
			FINAL_STATE_PARTICLES = 0,
			UC_VTX_PARTICLES	  = 1, 
			BSC_VTX_PARTICLES	  = 2, 
			TC_VTX_PARTICLES	  = 3
		};

        void Clear(Option_t *option="");
        SvtTrack* 		addTrack();
        SvtHit* 		addSvtHit();
        EcalCluster* 	addEcalCluster();
        EcalHit* 		addEcalHit();
        HpsParticle*    addParticle(collection_t); 
        HpsMCParticle*  addHpsMCParticle();
        GblTrack*   	addGblTrack();
        GblTrackData* 	addGblTrackData();
        GblStripData* 	addGblStripData();

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


        int getNumberOfTracks()         const  { return n_tracks; };
        int getNumberOfEcalClusters()   const  { return n_ecal_clusters; };
		int getNumberOfParticles(collection_t)	const; 
        int getNumberOfGblTracks()      const  { return n_gbl_tracks; };
        int getNumberOfGblTracksData()  const  { return n_gbl_tracks_data; };
        int getNumberOfGblStripData()   const  { return n_gbl_strips_data; };

        SvtTrack*      getTrack(int);
        SvtHit*        getSvtHit(int);
        EcalCluster*   getEcalCluster(int);
        EcalHit* 	   getEcalHit(int);
        HpsMCParticle* getMCParticle(int);
        HpsParticle*   getParticle(collection_t, int); 
        GblTrack*      getGblTrack(int);
        GblTrackData*  getGblTrackData(int);
        GblStripData*  getGblStripData(int);

        ClassDef(HpsEvent, 1);	

    private:

        TClonesArray* tracks;              //->
        TClonesArray* svt_hits;            //->
        TClonesArray* ecal_clusters;       //->
        TClonesArray* ecal_hits; 		   //->
        TClonesArray* fs_particles;        //->
        TClonesArray* uc_vtx_particles;    //->
        TClonesArray* bsc_vtx_particles;   //->
        TClonesArray* tc_vtx_particles;    //->
        TClonesArray* mc_particles; 	   //->
        TClonesArray* gbl_tracks;          //->
        TClonesArray* gbl_tracks_data;     //->
        TClonesArray* gbl_strips_data;     //->

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
        int n_uc_vtx_particles;
        int n_bsc_vtx_particles;
        int n_tc_vtx_particles;
        int n_mc_particles;
        int n_gbl_tracks;
        int n_gbl_tracks_data;
        int n_gbl_strips_data;
        
};

#endif


