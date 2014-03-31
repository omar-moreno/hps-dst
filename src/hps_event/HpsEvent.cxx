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
        ecal_hits(new TClonesArray("EcalHit", 1000)),
        muon_clusters(new TClonesArray("MuonCluster", 1000)),
        fs_recon_particles(new TClonesArray("HpsParticle", 1000)),
        vtx_recon_particles(new TClonesArray("HpsParticle", 1000)),
        mc_particles(new TClonesArray("HpsMCParticle", 1000)),
        gbl_tracks(new TClonesArray("GblTrack", 1000)),
        gbl_tracks_data(new TClonesArray("GblTrackData", 1000)),
        gbl_strips_data(new TClonesArray("GblStripData", 1000)),
        event_number(0), run_number(0), n_tracks(0), n_svt_hits(0),
        n_ecal_clusters(0), n_ecal_hits(0), n_muon_clusters(0),
        n_fs_recon_particles(0), n_vtx_recon_particles(0),
        n_mc_particles(0), n_gbl_tracks_data(0), n_gbl_strips_data(0)
{}

HpsEvent::HpsEvent(const HpsEvent &hpsEventObj)
    :   TObject(hpsEventObj),
        tracks(new TClonesArray("SvtTrack", 1000)),
        svt_hits(new TClonesArray("SvtHit", 1000)),
        ecal_clusters(new TClonesArray("EcalCluster", 1000)),
        ecal_hits(new TClonesArray("EcalHit", 1000)),
        muon_clusters(new TClonesArray("MuonCluster", 1000)),
        fs_recon_particles(new TClonesArray("HpsParticle", 1000)),
        vtx_recon_particles(new TClonesArray("HpsParticle", 1000)),
        mc_particles(new TClonesArray("HpsMCParticle", 1000)),
        gbl_tracks(new TClonesArray("GblTrack", 1000)),
        gbl_tracks_data(new TClonesArray("GblTrackData", 1000)),
        gbl_strips_data(new TClonesArray("GblStripData", 1000))
{
    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_svt_hits       = hpsEventObj.n_svt_hits;
    this->n_ecal_clusters  = hpsEventObj.n_ecal_clusters;
    this->n_ecal_hits  = hpsEventObj.n_ecal_hits;
    this->n_muon_clusters  = hpsEventObj.n_muon_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;
    this->n_fs_recon_particles = hpsEventObj.n_fs_recon_particles;
    this->n_vtx_recon_particles = hpsEventObj.n_vtx_recon_particles;
    this->n_mc_particles = hpsEventObj.n_mc_particles;
    this->n_gbl_tracks = hpsEventObj.n_gbl_tracks;
    this->n_gbl_tracks_data = hpsEventObj.n_gbl_tracks_data;
    this->n_gbl_strips_data = hpsEventObj.n_gbl_strips_data;

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *ecal_hits = *hpsEventObj.ecal_hits;
    *muon_clusters = *hpsEventObj.muon_clusters;
    *fs_recon_particles = *hpsEventObj.fs_recon_particles;
    *vtx_recon_particles = *hpsEventObj.vtx_recon_particles;
    *mc_particles = *hpsEventObj.mc_particles;
    *gbl_tracks = *hpsEventObj.gbl_tracks;
    *gbl_tracks_data = *hpsEventObj.gbl_tracks_data;
    *gbl_strips_data = *hpsEventObj.gbl_strips_data;
}


HpsEvent::~HpsEvent()
{
	Clear();
    delete tracks; 
    delete svt_hits;
    delete ecal_clusters; 
    delete ecal_hits;
    delete muon_clusters;
    delete fs_recon_particles;
    delete vtx_recon_particles;
    delete mc_particles;
    delete gbl_tracks;
    delete gbl_tracks_data;
    delete gbl_strips_data;
}

HpsEvent &HpsEvent::operator=(const HpsEvent &hpsEventObj)
{
    // Check for self-assignment
    if(this == &hpsEventObj) return *this;
    
    TObject::operator=(hpsEventObj);
    this->~HpsEvent();
    this->trigger_bits.clear();

    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_svt_hits       = hpsEventObj.n_svt_hits;
    this->n_ecal_clusters   = hpsEventObj.n_ecal_clusters;
    this->n_ecal_hits  = hpsEventObj.n_ecal_hits;
    this->n_muon_clusters   = hpsEventObj.n_muon_clusters;
    this->trigger_bits = hpsEventObj.trigger_bits;
    this->n_fs_recon_particles = hpsEventObj.n_fs_recon_particles;
    this->n_vtx_recon_particles = hpsEventObj.n_vtx_recon_particles;
    this->n_mc_particles = hpsEventObj.n_mc_particles;
    this->n_gbl_tracks     = hpsEventObj.n_gbl_tracks; 
    this->n_gbl_tracks_data     = hpsEventObj.n_gbl_tracks_data; 
    this->n_gbl_strips_data     = hpsEventObj.n_gbl_strips_data; 

    tracks = new TClonesArray("SvtTrack", 1000);
    svt_hits = new TClonesArray("SvtHit", 1000);
    ecal_clusters = new TClonesArray("EcalCluster", 1000); 
    ecal_hits = new TClonesArray("EcalHit", 1000);
    fs_recon_particles = new TClonesArray("HpsParticle", 1000);
    vtx_recon_particles = new TClonesArray("HpsParticle", 1000);
    mc_particles = new TClonesArray("HpsMCParticle", 1000);
    gbl_tracks = new TClonesArray("GblTrack", 1000);
    gbl_tracks_data = new TClonesArray("GblTrackData", 1000);
    gbl_strips_data = new TClonesArray("GblStripData", 1000);

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *ecal_hits = *hpsEventObj.ecal_hits;
    *muon_clusters = *hpsEventObj.muon_clusters;
    *fs_recon_particles = *hpsEventObj.fs_recon_particles;
    *vtx_recon_particles = *hpsEventObj.vtx_recon_particles;
    *mc_particles = *hpsEventObj.mc_particles;
    *gbl_tracks = *hpsEventObj.gbl_tracks;
    *gbl_tracks_data = *hpsEventObj.gbl_tracks_data;
    *gbl_strips_data = *hpsEventObj.gbl_strips_data;

    return *this;     
}

void HpsEvent::Clear(Option_t * /*option*/)
{
    TObject::Clear(); 
    tracks->Clear("C");
    svt_hits->Clear("C");
    ecal_clusters->Clear("C");
    ecal_hits->Clear("C");
    muon_clusters->Clear("C");
    fs_recon_particles->Clear("C");
    vtx_recon_particles->Clear("C");
    mc_particles->Clear("C");
    gbl_tracks->Clear("C");
    gbl_tracks_data->Clear("C");
    gbl_strips_data->Clear("C");
    n_ecal_clusters = 0;
    n_ecal_hits = 0;
    n_muon_clusters = 0;
    n_tracks = 0;  
    n_svt_hits = 0;
    n_fs_recon_particles = 0;
    n_vtx_recon_particles = 0;
    n_mc_particles = 0;
    n_gbl_tracks = 0;
    n_gbl_tracks_data = 0;
    n_gbl_strips_data = 0;
    trigger_bits.clear();
}


SvtTrack* HpsEvent::addTrack()
{
	return (SvtTrack*) tracks->ConstructedAt(n_tracks++);
}

SvtHit* HpsEvent::addSvtHit()
{
    return (SvtHit*) svt_hits->ConstructedAt(n_svt_hits++);
}

EcalCluster* HpsEvent::addEcalCluster()
{
    return (EcalCluster*) ecal_clusters->ConstructedAt(n_ecal_clusters++);
}

EcalHit* HpsEvent::addEcalHit()
{
	return (EcalHit*) ecal_hits->ConstructedAt(n_ecal_hits++);
}

MuonCluster* HpsEvent::addMuonCluster()
{
	return (MuonCluster*) muon_clusters->ConstructedAt(n_muon_clusters++);
}

HpsParticle* HpsEvent::addHpsParticle(int type)
{
	//
	assert(type == fs_type || type == vtx_type);

	if(type == fs_type){
		return (HpsParticle*) fs_recon_particles->ConstructedAt(n_fs_recon_particles++);
	} else {
		return (HpsParticle*) vtx_recon_particles->ConstructedAt(n_vtx_recon_particles++);
	}
}

HpsMCParticle* HpsEvent::addHpsMCParticle()
{
	return (HpsMCParticle*) mc_particles->ConstructedAt(n_mc_particles++);
}

GblTrack* HpsEvent::addGblTrack()
{
	return (GblTrack*) gbl_tracks->ConstructedAt(n_gbl_tracks++);
}

GblTrackData* HpsEvent::addGblTrackData()
{
	return (GblTrackData*) gbl_tracks_data->ConstructedAt(n_gbl_tracks_data++);
}

GblStripData* HpsEvent::addGblStripData()
{
	return (GblStripData*) gbl_strips_data->ConstructedAt(n_gbl_strips_data++);
}


SvtTrack* HpsEvent::getTrack(int track_n)
{
	return (SvtTrack*) tracks->At(track_n);
}

GblTrack* HpsEvent::getGblTrack(int track_n)
{
	return (GblTrack*) gbl_tracks->At(track_n);
}

GblTrackData* HpsEvent::getGblTrackData(int track_n)
{
	return (GblTrackData*) gbl_tracks_data->At(track_n);
}

GblStripData* HpsEvent::getGblStripData(int strip_n)
{
	return (GblStripData*) gbl_strips_data->At(strip_n);
}

SvtHit* HpsEvent::getSvtHit(int hit_n)
{
	return (SvtHit*) svt_hits->At(hit_n);
}

EcalCluster* HpsEvent::getEcalCluster(int ecal_cluster_n)
{
	return (EcalCluster*) ecal_clusters->At(ecal_cluster_n);
}

EcalHit* HpsEvent::getEcalHit(int ecal_hit_n)
{
	return (EcalHit*) ecal_hits->At(ecal_hit_n);
}

MuonCluster* HpsEvent::getMuonCluster(int muon_cluster_n)
{
	return (MuonCluster*) muon_clusters->At(muon_cluster_n);
}

HpsMCParticle* HpsEvent::getMCParticle(int mc_particle_n)
{
	return (HpsMCParticle*) mc_particles->At(mc_particle_n);
}

const int HpsEvent::fs_type  = 1;
const int HpsEvent::vtx_type = 2;
