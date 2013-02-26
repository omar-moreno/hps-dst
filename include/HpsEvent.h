
#ifndef _HPS_EVENT_H_
#define	_HPS_EVENT_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include "TObject.h"
#include "TClonesArray.h"

//--- LCIO ---//
#include "EVENT/LCEvent.h"
#include "IMPL/TrackImpl.h"

using namespace std; 

class HpsEvent : public TObject { 
	
	private:
		TClonesArray *tracks; //->	
		
		int event_number;
		int run_number;
		int n_tracks;
		int n_clusters;

	public:
		HpsEvent();	// Default Ctor
		~HpsEvent();
		IMPL::TrackImpl* AddTrack();

		//--- methods ---//
		void setEventNumber(int events){ event_number = events; };
		void setRunNumber(int run){ run_number = run; };
		void setNumberOfTracks(int tracks){ n_tracks = tracks; };
		void setNumberOfClusters(int clusters){ n_clusters = clusters; };	

	ClassDef(HpsEvent, 1);	
};

#endif


