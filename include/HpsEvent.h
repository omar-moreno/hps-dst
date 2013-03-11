/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#ifndef _HPS_EVENT_H_
#define	_HPS_EVENT_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

//---//
#include <SvtTrack.h>
#include <SvtHit.h>
#include <EcalCluster.h> 

class HpsEvent : public TObject { 

    public:
        HpsEvent();
        HpsEvent(const HpsEvent &hpsEventoObj);
        virtual ~HpsEvent();    
        HpsEvent &operator=(const HpsEvent &hpsEventObj);

        void Clear(Option_t *option="");
        SvtTrack* addTrack();
        SvtHit* addSvtHit();
        EcalCluster* addEcalCluster(); 

        void setEventNumber(int event_number){ this->event_number = event_number; };
        void setRunNumber(int run_number){ this->run_number = run_number; };
        void setNumberOfTracks(int n_tracks){ this->n_tracks = n_tracks; };
        void setNumberOfClusters(int n_clusters){ this->n_clusters = n_clusters; };	

        int getEventNumber()        const { return event_number; };
        int getRunNumber()          const { return run_number; }; 
        int getNumberOfTracks()     const { return n_tracks; };
        int getNumberOfClusters()   const { return n_clusters; };
        int getNumberOfHits()       const { return n_hits; };

        ClassDef(HpsEvent, 1);	

    private:
        TClonesArray *tracks;        //->	
        TClonesArray *svt_hits;      //-> 
        TClonesArray *ecal_clusters; //->  

        int event_number;
        int run_number;
        int n_tracks;
        int n_hits; 
        int n_clusters;
};

#endif


