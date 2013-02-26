
#include "HpsEvent.h"

ClassImp(HpsEvent)
	
HpsEvent::HpsEvent()
{
	tracks = new TClonesArray("IMPL::TrackImpl", 1000);
}

HpsEvent::~HpsEvent()
{
	Clear();
}

IMPL::TrackImpl* HpsEvent::AddTrack()
{	
	IMPL::TrackImpl *track = (IMPL::TrackImpl*) tracks->ConstructedAt(n_tracks);
	++n_tracks;
	return track;
}

