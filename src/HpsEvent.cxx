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
        muon_clusters(new TClonesArray("MuonCluster", 1000)),
        fs_recon_particles(new TClonesArray("HpsReconstructedParticle", 1000)),
        vtx_recon_particles(new TClonesArray("HpsReconstructedParticle", 1000)),
        event_number(0), run_number(0), n_tracks(0), n_hits(0),
        n_ecal_clusters(0), n_muon_clusters(0)
{}

HpsEvent::HpsEvent(const HpsEvent &hpsEventObj)
    :   TObject(hpsEventObj),
        tracks(new TClonesArray("SvtTrack", 1000)),
        svt_hits(new TClonesArray("SvtHit", 1000)),
        ecal_clusters(new TClonesArray("EcalCluster", 1000)),
        muon_clusters(new TClonesArray("MuonCluster", 1000)),
        fs_recon_particles(new TClonesArray("HpsReconstructedParticle", 1000)),
        vtx_recon_particles(new TClonesArray("HpsReconstructedParticle", 1000))
{
    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_hits       = hpsEventObj.n_hits; 
    this->n_ecal_clusters  = hpsEventObj.n_ecal_clusters;
    this->n_muon_clusters  = hpsEventObj.n_muon_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;


    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *muon_clusters = *hpsEventObj.muon_clusters;
    *fs_recon_particles = *hpsEventObj.fs_recon_particles;
    *vtx_recon_particles = *hpsEventObj.vtx_recon_particles;
}


HpsEvent::~HpsEvent()
{
	Clear();
    delete tracks; 
    delete ecal_clusters; 
    delete svt_hits; 
    delete muon_clusters;
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
    delete fs_recon_particles;
    delete vtx_recon_particles;
    this->trigger_bits.clear();

    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_hits       = hpsEventObj.n_hits; 
    this->n_ecal_clusters   = hpsEventObj.n_ecal_clusters;
    this->n_muon_clusters   = hpsEventObj.n_muon_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;

    tracks = new TClonesArray("SvtTrack", 1000);
    ecal_clusters = new TClonesArray("EcalCluster", 1000); 
    svt_hits = new TClonesArray("SvtHit", 1000);
    fs_recon_particles = new TClonesArray("HpsReconstructedParticle", 1000);
    vtx_recon_particles = new TClonesArray("HpsReconstructedParticle", 1000);

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *muon_clusters = *hpsEventObj.muon_clusters;
    *fs_recon_particles = *hpsEventObj.fs_recon_particles;
    *vtx_recon_particles = *hpsEventObj.vtx_recon_particles;

    return *this;     
}

void HpsEvent::Clear(Option_t * /*option*/)
{
    TObject::Clear(); 
    tracks->Clear("C");
    svt_hits->Clear("C");
    ecal_clusters->Clear("C");
    muon_clusters->Clear("C");
    n_ecal_clusters = 0;
    n_muon_clusters = 0;
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
    return (EcalCluster*) ecal_clusters->ConstructedAt(n_ecal_clusters++);
}

MuonCluster* HpsEvent::addMuonCluster()
{
	return (MuonCluster*) muon_clusters->ConstructedAt(n_muon_clusters++);
}

HpsReconstructedParticle* HpsEvent::addReconParticle(int type)
{
	//
	assert(type == fs_type || type == vtx_type);

	if(type == fs_type){
		return (HpsReconstructedParticle*) fs_recon_particles->ConstructedAt(n_fs_recon_particles++);
	} else {
		return (HpsReconstructedParticle*) vtx_recon_particles->ConstructedAt(n_vtx_recon_particles++);
	}
}
SvtTrack* HpsEvent::getTrack(int track_n)
{
	return (SvtTrack*) tracks->At(track_n);
}

EcalCluster* HpsEvent::getEcalCluster(int ecal_cluster_n)
{
	return (EcalCluster*) ecal_clusters->At(ecal_cluster_n);
}

SvtHit* HpsEvent::getSvtHit(int hit_n)
{
	return (SvtHit*) svt_hits->At(hit_n);
}

MuonCluster* HpsEvent::getMuonCluster(int muon_cluster_n)
{
	return (MuonCluster*) muon_clusters->At(muon_cluster_n);
}

const int HpsEvent::fs_type  = 1;
const int HpsEvent::vtx_type = 2;
