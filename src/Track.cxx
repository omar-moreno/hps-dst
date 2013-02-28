
#include <Track.h>

ClassImp(Track)

Track::Track()
{
    svt_hits = new TRefArray();
    n_hits = 0; 
}

Track::~Track()
{
}

void Track::Clear(Option_t* /* option */)
{
    cout << "Clearing Track" << endl;
    TObject::Clear(); 
    svt_hits->Delete(); 
    n_hits = 0; 
}

void Track::setTrackParameters(double track_d0, double track_phi, 
                               double track_omega, double track_tan_lambda, 
                               double track_z0)
{
    d0 = track_d0; 
    phi = track_phi;
    omega = track_omega; 
    tan_lambda = track_tan_lambda; 
    z0 = track_z0; 
}

void Track::setMomentum(double track_px, double track_py, double track_pz)
{
	px = track_px; 
	py = track_py;
	pz = track_pz;
}


void Track::addHit(SvtHit* hit)
{
    cout << "Adding a hit to Track" << endl;
    ++n_hits; 
    svt_hits->Add(hit); 
}

