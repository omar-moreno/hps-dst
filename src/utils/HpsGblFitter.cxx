#include "HpsGblFitter.h"
#include "GblTrackData.h"
#include "GblStripData.h"

//--- C++ ---//
#include <cstdlib>
#include <cmath>
#include <map>

//--- GBL ---//
#include <GblTrajectory.h>

//--- ROOT ---//
#include "TRandom3.h"
#include "TMatrixD.h"

using namespace std;

static const double Bconst = 0.0002998; 


HpsGblFitter::HpsGblFitter(double bz) : m_Bz(bz), 
                                        m_bfac(Bconst*m_Bz), 
                                        m_r(new TRandom3()),
                                        m_debug(false) {
}

HpsGblFitter::~HpsGblFitter() {
  delete m_r;
}

void HpsGblFitter::Clear() {
}

void HpsGblFitter::SetDebug(bool debug) {
  m_debug = debug;
}

bool HpsGblFitter::GetDebug() {
  return m_debug;
}


void HpsGblFitter::Fit(const GblTrackData* track) {
  
  
  m_debug = true;

  clock_t startTime = clock();
  double Chi2Sum = 0.;
  double NdfSum = 0;
  double LostSum = 0.;
  double s = 0.; // path length
 
  std::map<unsigned int,TMatrixD*> proL2m_list; // need to use pointer for TMatrix here?

 
  // Loop over strips
  const unsigned int n_strips = track->getNStrips();  
  for(unsigned int istrip=0; istrip!=n_strips; ++istrip) {
    
    const GblStripData* strip = track->getStrip(istrip);
    

    cout << "Processing strip " << istrip << " with id/layer " << strip->GetId() << endl;


    double step = strip->GetPath3D() - s;

    cout << "Path length step " << step << " from " << s << " to " << strip->GetPath3D() << endl;
    
    // Measurement direction (perpendicular and parallel to strip direction)
    TMatrixD mDir(2,3);
    mDir[0][0] = strip->GetU().x();
    mDir[0][1] = strip->GetU().y();
    mDir[0][2] = strip->GetU().z();
    mDir[1][0] = strip->GetV().x();
    mDir[1][1] = strip->GetV().y();
    mDir[1][2] = strip->GetV().z();
 
    if(m_debug) {
      cout << "mDir" << endl;
      mDir.Print();
    }

    TMatrixD mDirT(TMatrixD::kTransposed,mDir);

    if(m_debug) {
      cout << "mDirT" << endl;
      mDirT.Print();
    }

    // Track direction 
    double sinLambda = sin(strip->GetLambda());
    double cosLambda = sqrt(1.0 - sinLambda*sinLambda);
    double sinPhi = sin(strip->GetPhi());
    double cosPhi = sqrt(1.0 - sinPhi*sinPhi);
    
    if(m_debug) {
      cout << "Track direction sinLambda=" << sinLambda << " sinPhi=" << sinPhi << endl;
    }

    // Track direction in curvilinear frame (U,V,T)
    // U = Z x T / |Z x T|, V = T x U
    TMatrixD uvDir(2,3);
    uvDir[0][0] = -sinPhi;
    uvDir[0][1] = cosPhi;
    uvDir[0][2] = 0.;
    uvDir[1][0] = -sinLambda * cosPhi;
    uvDir[1][1] = -sinLambda * sinPhi;
    uvDir[1][2] = cosLambda;

    if(m_debug) {
      cout << "uvDir" << endl;
      uvDir.Print();
    }

    cout << "test" <<endl;
    TMatrixD a(2,3);
    TMatrixD b(3,2);
    a.Print();
    b.Print();
    (a*b).Print();
    cout << "end test" << endl;
    

    // projection from  measurement to local (curvilinear uv) directions (duv/dm)
    //TMatrixD proM2l(uvDir,TMatrixD::kMult,mDirT);
    //TMatrixD proM2l(uvDir);
    TMatrixD proM2l = uvDir * mDirT;

    //projection from local (uv) to measurement directions (dm/duv)
    //TMatrixD proL2m(TMatrixD::kInverted,proM2l);
    TMatrixD proL2m(proM2l);
    cout << "hej" << endl;
    proL2m.Invert();
    if(proL2m_list.find(strip->GetId()) != proL2m_list.end()) {
      cout << strip->GetId() << " was already in list?" << endl;
      exit(1);
    }
    proL2m_list[strip->GetId()] = new TMatrixD(proL2m);

    if(m_debug) {
      cout << "proM2l:" <<endl;
      proM2l.Print();
      cout << "proL2m:" <<endl;
      proL2m.Print();
      (proM2l*proL2m).Print();
    }

    //measurement/residual in the measurement system
    TMatrixD meas(1,2);
    meas[0][0] = strip->GetUmeas() - strip->GetTrackPos().x(); // only measurement in u-direction
    meas[0][1] = 0.;
    //meas[0][0] += deltaU[iLayer] # misalignment
    TMatrixD measErr(1,2);
    measErr[0][0] = strip->GetUmeasErr();
    measErr[0][1] = 0.;
    TMatrixD measPrec(1,2);
    measPrec[0][0] = 1.0/ (measErr(0,0) * measErr(0,0));
    measPrec[0][1] = 0.; // 1D measurement perpendicular to strip direction
    
    if (m_debug) {
      cout << "meas: " << endl;
      meas.Print();
      cout << "measErr:" << endl;
      measErr.Print();
      cout << "measPrec:" << endl;
      measPrec.Print();
    }


  } //strips


  // clean up
  for(std::map<unsigned int,TMatrixD*>::iterator it = proL2m_list.begin(); it!=proL2m_list.end(); ++it) {
    delete (it->second);
  }
  
  if(m_debug) {
    cout << "HpsGblFitter: Fit() done." << endl;
  }
  
}
