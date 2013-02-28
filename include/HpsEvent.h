
#ifndef _HPS_EVENT_H_
#define	_HPS_EVENT_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>
#include <TObjArray.h>

//---//
#include <Track.h>
#include <SvtHit.h>
#include <EcalCluster.h> 

using namespace std; 

class HpsEvent : public TObject { 

    public:
        HpsEvent();	            
        virtual ~HpsEvent();    
        
        void Clear(Option_t *option="");

        Track* addTrack();
        SvtHit* addSvtHit();
        EcalCluster* addEcalCluster(); 

        void setEventNumber(int events){ event_number = events; };
        void setRunNumber(int run){ run_number = run; };
        void setNumberOfTracks(int tracks){ n_tracks = tracks; };
        void setNumberOfClusters(int clusters){ n_clusters = clusters; };	

        int getNumberOfTracks(){ return n_tracks; };
        int getNumberOfClusters(){ return n_clusters; }; 

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


