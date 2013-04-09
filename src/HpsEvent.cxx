/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */
#include <HpsEvent.h>

ClassImp(HpsEvent)
	
HpsEvent::HpsEvent()
    :   TObject(), 
        tracks(new TClonesArray("SvtTrack", 1000)),
        svt_hits(new TClonesArray("SvtHit", 1000)),
        ecal_clusters(new TClonesArray("EcalCluster", 1000)),
        event_number(0), run_number(0), n_tracks(0), n_hits(0), n_clusters(0)
{}

HpsEvent::HpsEvent(const HpsEvent &hpsEventObj)
    :   TObject(hpsEventObj),
        tracks(new TClonesArray("SvtTrack", 1000)),
        svt_hits(new TClonesArray("SvtHit", 1000)),
        ecal_clusters(new TClonesArray("EcalCluster", 1000))
{
    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_hits       = hpsEventObj.n_hits; 
    this->n_clusters   = hpsEventObj.n_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;


    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
}


HpsEvent::~HpsEvent()
{
	Clear();
    delete tracks; 
    delete ecal_clusters; 
    delete svt_hits; 
}

HpsEvent &HpsEvent::operator=(const HpsEvent &hpsEventObj)
{
    // Check for self-assignment
    if(this == &hpsEventObj) return *this;
    
    TObject::operator=(hpsEventObj);
    Clear(); 
    delete tracks; 
    delete ecal_clusters; 
    delete svt_hits; 
    this->trigger_bits.clear();

    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_hits       = hpsEventObj.n_hits; 
    this->n_clusters   = hpsEventObj.n_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;

    tracks = new TClonesArray("SvtTrack", 1000);
    ecal_clusters = new TClonesArray("EcalCluster", 1000); 
    svt_hits = new TClonesArray("SvtHit", 1000);

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;

    return *this;     
}

void HpsEvent::Clear(Option_t * /*option*/)
{
    TObject::Clear(); 
    tracks->Clear("C");
    svt_hits->Clear("C");
    ecal_clusters->Clear("C");
    n_clusters = 0;  
    n_tracks = 0;  
    n_hits = 0; 
}


SvtTrack* HpsEvent::addTrack()
{
	return (SvtTrack*) tracks->ConstructedAt(n_tracks++);
}

SvtHit* HpsEvent::addSvtHit()
{
    return (SvtHit*) svt_hits->ConstructedAt(n_hits++); 
}

EcalCluster* HpsEvent::addEcalCluster()
{
    return (EcalCluster*) ecal_clusters->ConstructedAt(n_clusters++);  
}

SvtTrack* HpsEvent::getTrack(int track_n)
{
	return (SvtTrack*) tracks->At(track_n);
}

EcalCluster* HpsEvent::getEcalCluster(int cluster_n)
{
	return (EcalCluster*) ecal_clusters->At(cluster_n);
}

SvtHit* HpsEvent::getSvtHit(int hit_n)
{
	return (SvtHit*) svt_hits->At(hit_n);
}
