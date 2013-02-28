
#include <HpsEvent.h>

ClassImp(HpsEvent)
	
HpsEvent::HpsEvent()
{
	tracks = new TClonesArray("Track", 1000);
    svt_hits = new TClonesArray("SvtHit", 1000); 
    ecal_clusters = new TClonesArray("EcalCluster", 1000); 
    n_tracks = 0; 
    n_hits = 0; 
}

HpsEvent::~HpsEvent()
{
	Clear();
}

void HpsEvent::Clear(Option_t * /*option*/)
{
    cout << "Clearing Event" << endl;
    TObject::Clear(); 
    tracks->Clear("C");
    svt_hits->Clear("C");
    ecal_clusters->Clear("C");
    n_clusters = 0;  
    n_tracks = 0;  
    n_hits = 0; 
}


Track* HpsEvent::addTrack()
{
    cout << "Adding a track" << endl;   
	return (Track*) tracks->ConstructedAt(n_tracks++);
}

SvtHit* HpsEvent::addSvtHit()
{
    cout << "Adding a hit" << endl;
    return (SvtHit*) svt_hits->ConstructedAt(n_hits++); 
}

EcalCluster* HpsEvent::addEcalCluster()
{
    cout << "Adding Ecal cluster" << endl;
    return (EcalCluster*) ecal_clusters->ConstructedAt(n_clusters++);  
}
