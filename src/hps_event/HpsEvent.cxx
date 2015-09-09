/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
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
        fs_particles(new TClonesArray("HpsParticle", 1000)),
        uc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        uc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
        bsc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        bsc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
        tc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        tc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
        mc_particles(new TClonesArray("HpsMCParticle", 1000)),
        gbl_tracks(new TClonesArray("GblTrack", 1000)),
        gbl_tracks_data(new TClonesArray("GblTrackData", 1000)),
        gbl_strips_data(new TClonesArray("GblStripData", 1000)),
        event_number(0),
        run_number(0),
        n_tracks(0),
        n_svt_hits(0),
        n_ecal_clusters(0), 
        n_ecal_hits(0), 
        n_fs_particles(0),
        n_uc_v0_candidates(0), 
        n_uc_moller_candidates(0), 
        n_bsc_v0_candidates(0),
        n_bsc_moller_candidates(0),
        n_tc_v0_candidates(0), 
        n_tc_moller_candidates(0), 
        n_mc_particles(0),
        n_gbl_tracks_data(0),
        n_gbl_strips_data(0)
{}

HpsEvent::HpsEvent(const HpsEvent &hpsEventObj)
    :   TObject(hpsEventObj),
        tracks(new TClonesArray("SvtTrack", 1000)),
        svt_hits(new TClonesArray("SvtHit", 1000)),
        ecal_clusters(new TClonesArray("EcalCluster", 1000)),
        ecal_hits(new TClonesArray("EcalHit", 1000)),
        fs_particles(new TClonesArray("HpsParticle", 1000)),
        uc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        uc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
        bsc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        bsc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
        tc_v0_candidates(new TClonesArray("HpsParticle", 1000)),
        tc_moller_candidates(new TClonesArray("HpsParticle", 1000)), 
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
    this->n_fs_particles = hpsEventObj.n_fs_particles;
    this->n_uc_v0_candidates = hpsEventObj.n_uc_v0_candidates;
    this->n_uc_moller_candidates = hpsEventObj.n_uc_moller_candidates;
    this->n_bsc_v0_candidates = hpsEventObj.n_bsc_v0_candidates;
    this->n_bsc_moller_candidates = hpsEventObj.n_bsc_moller_candidates;
    this->n_tc_v0_candidates = hpsEventObj.n_tc_v0_candidates;
    this->n_tc_moller_candidates = hpsEventObj.n_tc_moller_candidates;
    this->n_mc_particles = hpsEventObj.n_mc_particles;
    this->n_gbl_tracks = hpsEventObj.n_gbl_tracks;
    this->n_gbl_tracks_data = hpsEventObj.n_gbl_tracks_data;
    this->n_gbl_strips_data = hpsEventObj.n_gbl_strips_data;

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *ecal_hits = *hpsEventObj.ecal_hits;
    *fs_particles = *hpsEventObj.fs_particles;
    *uc_v0_candidates = *hpsEventObj.uc_v0_candidates;
    *uc_moller_candidates = *hpsEventObj.uc_moller_candidates;
    *bsc_v0_candidates = *hpsEventObj.bsc_v0_candidates;
    *bsc_moller_candidates = *hpsEventObj.bsc_moller_candidates;
    *tc_v0_candidates = *hpsEventObj.tc_v0_candidates;
    *tc_moller_candidates = *hpsEventObj.tc_moller_candidates;
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
    delete fs_particles;
    delete uc_v0_candidates;
    delete uc_moller_candidates;
    delete bsc_v0_candidates;
    delete bsc_moller_candidates;
    delete tc_v0_candidates;
    delete tc_moller_candidates;
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
    this->event_number = hpsEventObj.event_number; 
    this->run_number   = hpsEventObj.run_number; 
    this->n_tracks     = hpsEventObj.n_tracks; 
    this->n_svt_hits       = hpsEventObj.n_svt_hits;
    this->n_ecal_clusters   = hpsEventObj.n_ecal_clusters;
    this->n_ecal_hits  = hpsEventObj.n_ecal_hits;
    this->n_fs_particles = hpsEventObj.n_fs_particles;
    this->n_uc_v0_candidates = hpsEventObj.n_uc_v0_candidates;
    this->n_uc_moller_candidates = hpsEventObj.n_uc_moller_candidates;
    this->n_bsc_v0_candidates = hpsEventObj.n_bsc_v0_candidates;
    this->n_bsc_moller_candidates = hpsEventObj.n_bsc_moller_candidates;
    this->n_tc_v0_candidates = hpsEventObj.n_tc_v0_candidates;
    this->n_tc_moller_candidates = hpsEventObj.n_tc_moller_candidates;
    this->n_mc_particles = hpsEventObj.n_mc_particles;
    this->n_gbl_tracks     = hpsEventObj.n_gbl_tracks; 
    this->n_gbl_tracks_data     = hpsEventObj.n_gbl_tracks_data; 
    this->n_gbl_strips_data     = hpsEventObj.n_gbl_strips_data; 

    tracks = new TClonesArray("SvtTrack", 1000);
    svt_hits = new TClonesArray("SvtHit", 1000);
    ecal_clusters = new TClonesArray("EcalCluster", 1000); 
    ecal_hits = new TClonesArray("EcalHit", 1000);
    fs_particles = new TClonesArray("HpsParticle", 1000);
    uc_v0_candidates = new TClonesArray("HpsParticle", 1000);
    uc_moller_candidates = new TClonesArray("HpsParticle", 1000);
    bsc_v0_candidates = new TClonesArray("HpsParticle", 1000);
    bsc_moller_candidates = new TClonesArray("HpsParticle", 1000);
    tc_v0_candidates = new TClonesArray("HpsParticle", 1000);
    tc_moller_candidates = new TClonesArray("HpsParticle", 1000);
    mc_particles = new TClonesArray("HpsMCParticle", 1000);
    gbl_tracks = new TClonesArray("GblTrack", 1000);
    gbl_tracks_data = new TClonesArray("GblTrackData", 1000);
    gbl_strips_data = new TClonesArray("GblStripData", 1000);

    *tracks    = *hpsEventObj.tracks;
    *svt_hits  = *hpsEventObj.svt_hits;  
    *ecal_clusters = *hpsEventObj.ecal_clusters;
    *ecal_hits = *hpsEventObj.ecal_hits;
    *fs_particles = *hpsEventObj.fs_particles;
    *uc_v0_candidates = *hpsEventObj.uc_v0_candidates;
    *uc_moller_candidates = *hpsEventObj.uc_moller_candidates;
    *bsc_v0_candidates = *hpsEventObj.bsc_v0_candidates;
    *bsc_moller_candidates = *hpsEventObj.bsc_moller_candidates;
    *tc_v0_candidates = *hpsEventObj.tc_v0_candidates;
    *tc_moller_candidates = *hpsEventObj.tc_moller_candidates;
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
    fs_particles->Clear("C");
    uc_v0_candidates->Clear("C");
    uc_moller_candidates->Clear("C");
    bsc_v0_candidates->Clear("C");
    bsc_moller_candidates->Clear("C");
    tc_v0_candidates->Clear("C");
    tc_moller_candidates->Clear("C");
    mc_particles->Clear("C");
    gbl_tracks->Clear("C");
    gbl_tracks_data->Clear("C");
    gbl_strips_data->Clear("C");
    n_ecal_clusters = 0;
    n_ecal_hits = 0;
    n_tracks = 0;  
    n_svt_hits = 0;
    n_fs_particles = 0;
    n_uc_v0_candidates = 0;
    n_uc_moller_candidates = 0;
    n_bsc_v0_candidates = 0;
    n_bsc_moller_candidates = 0;
    n_tc_v0_candidates = 0;
    n_tc_moller_candidates = 0;
    n_mc_particles = 0;
    n_gbl_tracks = 0;
    n_gbl_tracks_data = 0;
    n_gbl_strips_data = 0;
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

HpsParticle* HpsEvent::addParticle(HpsParticle::particle_type type)
{
	switch(type) { 
        case HpsParticle::FINAL_STATE_PARTICLE:
            return (HpsParticle*) fs_particles->ConstructedAt(n_fs_particles++); 
		case HpsParticle::UC_V0_CANDIDATE: 
			return (HpsParticle*) uc_v0_candidates->ConstructedAt(n_uc_v0_candidates++);
		case HpsParticle::BSC_V0_CANDIDATE:
			return (HpsParticle*) bsc_v0_candidates->ConstructedAt(n_bsc_v0_candidates++);
		case HpsParticle::TC_V0_CANDIDATE:
			return (HpsParticle*) tc_v0_candidates->ConstructedAt(n_tc_v0_candidates++);
		default: 
			// TODO: If a collection type is invalid, throw an exception instead
			return NULL; 
	}
}

HpsMCParticle* HpsEvent::addHpsMCParticle()
{
	return (HpsMCParticle*) mc_particles->ConstructedAt(n_mc_particles++);
}


int HpsEvent::getNumberOfParticles(HpsParticle::particle_type type) const
{
	switch(type){
		case HpsParticle::FINAL_STATE_PARTICLE:
			return n_fs_particles;
		case HpsParticle::UC_V0_CANDIDATE: 
			return n_uc_v0_candidates;  
		case HpsParticle::BSC_V0_CANDIDATE:
			return n_bsc_v0_candidates;
		case HpsParticle::TC_V0_CANDIDATE:
			return n_tc_v0_candidates; 
		default: 
			// TODO: If a collection type is invalid, throw an exception instead
			return -1; 
	}
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

HpsMCParticle* HpsEvent::getMCParticle(int mc_particle_n)
{
	return (HpsMCParticle*) mc_particles->At(mc_particle_n);
}

HpsParticle* HpsEvent::getParticle(HpsParticle::particle_type type, int particle_n)
{
	switch(type){ 
        case HpsParticle::FINAL_STATE_PARTICLE:
            return (HpsParticle*) fs_particles->At(particle_n); 
		case HpsParticle::UC_V0_CANDIDATE: 
			return (HpsParticle*) uc_v0_candidates->At(particle_n);
		case HpsParticle::BSC_V0_CANDIDATE: 
			return (HpsParticle*) bsc_v0_candidates->At(particle_n);
		case HpsParticle::TC_V0_CANDIDATE: 
			return (HpsParticle*) tc_v0_candidates->ConstructedAt(particle_n);
		default:
			// TODO: If the collection type is invalid, throw an exception instead.	
			return NULL; 
	}
}
