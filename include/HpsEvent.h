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
        void setTriggerBitInfo(std::vector<int> trigger_bits){ this->trigger_bits = trigger_bits; };

        int getEventNumber()       const  { return event_number; };
        int getRunNumber()         const { return run_number; };
        int getNumberOfTracks()    const { return n_tracks; };
        int getNumberOfClusters()   const { return n_clusters; };
        int getNumberOfHits()       const  { return n_hits; };
        std::vector<int> getTriggerBitInfo() const { return trigger_bits; };

        SvtTrack*     getTrack(int track_n);
        EcalCluster*  getEcalCluster(int cluster_n);
        SvtHit*       getSvtHit(int hit_n);

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

        std::vector<int> trigger_bits;
};

#endif


