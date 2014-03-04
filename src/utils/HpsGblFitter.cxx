/**
 *	@section purpose: 
 *		GBL track refit
 *	@author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *	@version: 1.0
 *
 */

#include "HpsGblFitter.h"

//--- DST ---//
#include "GblTrack.h"
#include "GblTrackData.h"
#include "GblStripData.h"

//--- C++ ---//
#include <cstdlib>
#include <cmath>
#include <map>

//--- GBL ---//
#include <GblPoint.h>
#include <MilleBinary.h>

//--- ROOT ---//
#include "TRandom3.h"

using namespace std;

// constant to normalize curvature
static const double Bconst = 0.0002998; 
//variable that defines the reference point on the trajectory (typically at path length zero)
static const int refLabel = 1;
//variable indexes
static const unsigned int idx_qOverP = 0;
static const unsigned int idx_xT = 3;
static const unsigned int idx_yT = 4;



HpsGblFitter::HpsGblFitter(double bz) : m_Bz(bz), 
                                        m_bfac(Bconst*m_Bz), 
                                        m_r(new TRandom3()),
                                        m_debug(false),
                                        m_chi2(-1.), 
                                        m_ndf(-1),
                                        m_lost_weight(-1.),
                                        m_traj(NULL) {
}

HpsGblFitter::~HpsGblFitter() {
  delete m_r;
  if( m_traj != NULL) {
    delete m_traj;
  }
}

void HpsGblFitter::Clear() {
  m_chi2 = -1.; 
  m_ndf = -1;
  m_lost_weight = -1.;
  if( m_traj != NULL) {
    delete m_traj;
    m_traj = NULL;
  }
}

void HpsGblFitter::SetDebug(bool debug) {
  m_debug = debug;
}

bool HpsGblFitter::GetDebug() {
  return m_debug;
}


HpsGblFitter::HpsGblFitStatus HpsGblFitter::Fit(const GblTrackData* track) {
  
  

  // Time the fits
  clock_t startTime = clock();
  // Number of track fits so far and the sum of results
  // TODO: obsolete since only one fit in in this function - need to move outside
  int nTry = 0;
  gbl::MilleBinary mille; // for producing MillePede-II binary file

  // path length along trajectory
  double s = 0.;
  // jacobian to transport errors between points along the path
  TMatrixD jacPointToPoint(5, 5);
  jacPointToPoint.UnitMatrix();
  // Option to use uncorrelated  MS errors
  // This is similar to what is done in lcsim seedtracker
  // The msCov below holds the MS errors
  // This is for testing purposes only.
  bool useUncorrMS = false;
  TMatrixD msCov(5, 5);
  msCov.Zero();
  // Vector of the strip clusters used for the GBL fit
  vector<gbl::GblPoint> listOfPoints;
  
  // Store the projection from local to measurement frame for each strip cluster
  // need to use pointer for TMatrix here?
  std::map<unsigned int,TMatrixD*> proL2m_list; 
  // Save the association between strip cluster and label
  std::map<const GblStripData*,unsigned int> stripLabelMap;
  
  //start trajectory at refence point (s=0) - this point has no measurement
  gbl::GblPoint ref_point(jacPointToPoint);
  listOfPoints.push_back(ref_point);
  
  // Loop over strips
  const unsigned int n_strips = track->getNStrips();  
  for(unsigned int istrip=0; istrip!=n_strips; ++istrip) {
    
    const GblStripData* strip = track->getStrip(istrip);
    
    if( m_debug ) {
      cout << "HpsGblFitter: " << "Processing strip " << istrip << " with id/layer " << strip->GetId() << endl;
    }
    
    // Path length step for this cluster
    double step = strip->GetPath3D() - s;
    
    if( m_debug ) {
      cout << "HpsGblFitter: " << "Path length step " << step << " from " << s << " to " << strip->GetPath3D() << endl;
    }
    
    // Measurement direction (perpendicular and parallel to strip direction)
    TMatrixD mDir(2,3);
    mDir[0][0] = strip->GetU().x();
    mDir[0][1] = strip->GetU().y();
    mDir[0][2] = strip->GetU().z();
    mDir[1][0] = strip->GetV().x();
    mDir[1][1] = strip->GetV().y();
    mDir[1][2] = strip->GetV().z();
 
    if(m_debug) {
      cout << "HpsGblFitter: " << "mDir" << endl;
      mDir.Print();
    }

    TMatrixD mDirT(TMatrixD::kTransposed,mDir);

    if(m_debug) {
      cout << "HpsGblFitter: " << "mDirT" << endl;
      mDirT.Print();
    }

    // Track direction 
    double sinLambda = sin(strip->GetLambda());
    double cosLambda = sqrt(1.0 - sinLambda*sinLambda);
    double sinPhi = sin(strip->GetPhi());
    double cosPhi = sqrt(1.0 - sinPhi*sinPhi);
    
    if(m_debug) {
      cout << "HpsGblFitter: " << "Track direction sinLambda=" << sinLambda << " sinPhi=" << sinPhi << endl;
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
      cout << "HpsGblFitter: " << "uvDir" << endl;
      uvDir.Print();
    }

    // if(m_debug) cout << "HpsGblFitter: " << "test" <<endl;
    // TMatrixD a(2,3);
    // TMatrixD b(3,2);
    // a.Print();
    // b.Print();
    // (a*b).Print();
    // if(m_debug) cout << "HpsGblFitter: " << "end test" << endl;

    // projection from  measurement to local (curvilinear uv) directions (duv/dm)
    //TMatrixD proM2l(uvDir,TMatrixD::kMult,mDirT);
    //TMatrixD proM2l(uvDir);
    TMatrixD proM2l = uvDir * mDirT;

    //projection from local (uv) to measurement directions (dm/duv)
    //TMatrixD proL2m(TMatrixD::kInverted,proM2l);
    TMatrixD proL2m(proM2l);
    proL2m.Invert();
    if(proL2m_list.find(strip->GetId()) != proL2m_list.end()) {
      cout << "HpsGblFitter: " << strip->GetId() << " was already in list?" << endl;
      exit(1);
    }
    proL2m_list[strip->GetId()] = new TMatrixD(proL2m);

    if(m_debug) {
      cout << "HpsGblFitter: " << "proM2l:" <<endl;
      proM2l.Print();
      cout << "HpsGblFitter: " << "proL2m:" <<endl;
      proL2m.Print();
      cout << "HpsGblFitter: " << "proM2l*proL2m (should be unit matrix):" <<endl;
      (proM2l*proL2m).Print();
    }

    // measurement/residual in the measurement system
    // only 1D measurement in u-direction, set strip measurement direction to zero
    TVectorD meas(2);
    double uRes = strip->GetUmeas() - strip->GetTrackPos().x();
    meas[0] = uRes;
    meas[1] = 0.;
    //meas[0][0] += deltaU[iLayer] # misalignment
    TVectorD measErr(2);
    measErr[0] = strip->GetUmeasErr();
    measErr[1] = 0.;
    TVectorD measPrec(2);
    measPrec[0] = 1.0/ (measErr(0) * measErr(0));
    measPrec[1] = 0.; 
    
    if (m_debug) {
      cout << "HpsGblFitter: " << "meas: " << endl;
      meas.Print();
      cout << "HpsGblFitter: " << "measErr:" << endl;
      measErr.Print();
      cout << "HpsGblFitter: " << "measPrec:" << endl;
      measPrec.Print();
    }


    //Find the Jacobian to be able to propagate the covariance matrix to this strip position
    jacPointToPoint = gblSimpleJacobianLambdaPhi(step, cosLambda, abs(m_bfac));
    
    
    if (m_debug) {
      cout << "HpsGblFitter: " << "jacPointToPoint to extrapolate to this point:" << endl;
      jacPointToPoint.Print();
    }
    
    //propagate MS covariance matrix (in the curvilinear frame) to this strip position
    //msCov = np.dot(jacPointToPoint, np.dot(msCov, jacPointToPoint.T))
    //measMsCov = np.dot(proL2m, np.dot(msCov[3:, 3:], proL2m.T))

    if (m_debug) {
      cout << "HpsGblFitter: " << " msCov at this point:" << endl;
      msCov.Print();
      //cout << "HpsGblFitter: " << "measMsCov at this point:" << endl;
      //measMsCov.Print();
    }

    //Option to blow up measurement error according to multiple scattering
    //if useUncorrMS:
    //measPrec[0] = 1.0 / (measErr[0] ** 2 + measMsCov[0, 0])
    //  if debug:
    //print 'Adding measMsCov ', measMsCov[0,0]
        
    // point with independent measurement
    gbl::GblPoint point(jacPointToPoint);

    //Add measurement to the point
    point.addMeasurement(proL2m,meas,measPrec);


    
    //Add scatterer in curvilinear frame to the point
    // no direction in this frame
    TVectorD scat(2);
    scat.Zero();

    // Scattering angle in the curvilinear frame
    //Note the cosLambda to correct for the projection in the phi direction
    TVectorD scatErr(2);
    scatErr[0] = strip->GetMSAngle();
    scatErr[1] = strip->GetMSAngle() / cosLambda;
    TVectorD scatPrec(2);
    scatPrec[0] = 1.0 / (scatErr(0) * scatErr(0));
    scatPrec[1] = 1.0 / (scatErr(1) * scatErr(1));
    
    // add scatterer if not using the uncorrelated MS covariances for testing
    if (! useUncorrMS) {
      point.addScatterer(scat, scatPrec);
      if (m_debug) {
        cout << "HpsGblFitter: " << "adding scatError to this point:" << endl;
        scatErr.Print();
      }
    }
       

    // Add this GBL point to list that will be used in fit
    listOfPoints.push_back(point);
    unsigned int iLabel = listOfPoints.size();
    
    
    // Update MS covariance matrix 
    msCov[1, 1] += scatErr[0]*scatErr[0];
    msCov[2, 2] += scatErr[1]*scatErr[1];

    /*

        ##### 
        ## Calculate global derivatives for this point
        # track direction in tracking/global frame
        tDirGlobal = np.array( [ [cosPhi * cosLambda, sinPhi * cosLambda, sinLambda] ] )        
        # Cross-check that the input is consistent
        if( np.linalg.norm( tDirGlobal - strip.tDir) > 0.00001):
          print 'ERROR: tDirs are not consistent!'
          sys.exit(1)
        # rotate track direction to measurement frame          
        tDirMeas = np.dot( tDirGlobal, np.array([strip.u, strip.v, strip.w]) )
        #tDirMeas = utils.rotateGlToMeas(strip,tDirGlobal)
        normalMeas = np.dot( strip.w , np.array([strip.u, strip.v, strip.w]) ) 
        #normalMeas = utils.rotateGlToMeas(strip,strip.w) 
        # non-measured directions 
        vmeas = 0.
        wmeas = 0.
        # calculate and add derivatives to point
        glDers = utils.globalDers(strip.layer,strip.meas,vmeas,wmeas,tDirMeas,normalMeas)
        ders = glDers.getDers(track.isTop())
        labGlobal = ders['labels']
        addDer = ders['ders']
        if debug:
          print 'global derivatives:'
          print labGlobal
          print addDer
        point.addGlobals(labGlobal, addDer)
        ##### 

    */
        
    
    //if (nTry==0) {
    if(m_debug) {
      cout << "HpsGblFitter: " << "uRes " <<  strip->GetId() <<  " uRes " << uRes <<  " pred (" <<  strip->GetTrackPos().x() << "," << strip->GetTrackPos().y() << "," << strip->GetTrackPos().z() << ") s(3D) " << strip->GetPath3D() << endl;
    }

    //go to next point
    s += step;
    
    // save strip and label map
    stripLabelMap[strip] = iLabel;
    
    
      


  } //strips


  //create the trajectory
  m_traj = new gbl::GblTrajectory(listOfPoints); //,seedLabel, clSeed);
  
  if (! m_traj->isValid()) {
    cout << "HpsGblFitter: " << " Invalid GblTrajectory -> skip" << endl;
    return INVALIDTRAJ;
  }
  // fit trajectory
  m_traj->fit(m_chi2, m_ndf, m_lost_weight);
  if( m_debug ) {
    std::cout << "HpsGblFitter: Chi2 " << " Fit: " << m_chi2 << ", " << m_ndf << ", " << m_lost_weight << std::endl;
  }
  // write to MP binary file
  m_traj->milleOut(mille);

  
  // clean up local variables allocated
  for(std::map<unsigned int,TMatrixD*>::iterator it = proL2m_list.begin(); it!=proL2m_list.end(); ++it) {
    delete (it->second);
  }
  
  

  clock_t endTime = clock();
  double diff = endTime - startTime;
  double cps = CLOCKS_PER_SEC;
  if( m_debug ) {
    std::cout << "HpsGblFitter: " << " Time elapsed " << diff / cps << " s" << std::endl;
  }

  if(m_debug) {
    cout << "HpsGblFitter: " << "HpsGblFitter: Fit() done successfully." << endl;
  }

  return OK;
}



TMatrixD HpsGblFitter::gblSimpleJacobianLambdaPhi(double ds, double cosl, double bfac) {
  /**
     Simple jacobian: quadratic in arc length difference.
     using lambda phi as directions
    
    @param ds: arc length difference
    @type ds: float
    @param cosl: cos(lambda)
    @type cosl: float
    @param bfac: Bz*c
    @type bfac: float
    @return: jacobian to move by 'ds' on trajectory
    @rtype: matrix(float)
       ajac(1,1)= 1.0D0
       ajac(2,2)= 1.0D0
       ajac(3,1)=-DBLE(bfac*ds)
       ajac(3,3)= 1.0D0
       ajac(4,1)=-DBLE(0.5*bfac*ds*ds*cosl)
       ajac(4,3)= DBLE(ds*cosl)
       ajac(4,4)= 1.0D0
       ajac(5,2)= DBLE(ds)
       ajac(5,5)= 1.0D0
    '''
    jac = np.eye(5)
    jac[2, 0] = -bfac * ds
    jac[3, 0] = -0.5 * bfac * ds * ds * cosl
    jac[3, 2] = ds * cosl
    jac[4, 1] = ds  
    return jac
  */
  TMatrixD jac(5, 5);
  jac.UnitMatrix();
  jac[2][0] = -bfac * ds;
  jac[3][0] = -0.5 * bfac * ds * ds * cosl;
  jac[3][2] = ds * cosl;
  jac[4][1] = ds;
  return jac;
}


void HpsGblFitter::SetTrackProperties(GblTrack* track, const GblTrackData* track_data) {

  // Convert GBL trajectory information to the track object
  
  if( m_traj == NULL) {
    cout << "HpsGblFitter: ERROR there is no trajectory created so can't set properties!" << endl;
    return;
  }

  // set original track parameters for comparison
  // TODO: should be removed as they are duplicated?
  track->setSeedTrackParameters(track_data->getKappa(),track_data->getTheta(),track_data->getPhi(),track_data->getD0(),track_data->getZ0());

  // get the track parameter corrections to the reference point (path length zero)
  TVectorD localPar(5);
  TMatrixDSym localCov(5);

  m_traj->getResults(refLabel,localPar, localCov);

  // convert xT,yT,zT correction in curvilinear frame to perigee frame
  TVectorD clParCorr(3);
  clParCorr[0] = localPar[idx_xT];
  clParCorr[1] = localPar[idx_yT];
  clParCorr[2] = 0.;
  
  // get the projection from perigee to curvilinear frame
  TMatrixD prjClToPer(TMatrixD::kInverted, track_data->getPrjPerToCl());
  //TMatrixD prjClToPer(track_data->getPrjPerToCl());
  
  // project into the perigee frame
  TVectorD perParCorr = prjClToPer * clParCorr;
  
  // corrections
  double qP_corr = localPar[idx_qOverP]; 
  double pt_old = fabs(1.0/track->getSeedKappa() * m_bfac);
  double qP_old = pt_old/sin(track->getSeedTheta());
  double qP_new = qP_old + qP_corr;
  double p_new = fabs(1/qP_new);
  double pt_new = p_new * sin(track->getSeedTheta());
  double kappa_new = 1/pt_new * m_bfac;
  double d0_corr = -1.0 * perParCorr[1]; // sign convention of d0 in curvilinear frame
  double z0_corr = perParCorr[2]; 
  
  // set the new parameters
  track->setTrackParameters(kappa_new,track->getSeedTheta(),track->getSeedPhi(),track->getSeedD0() + d0_corr,track->getSeedZ0() + z0_corr);

  //set covariance matrix
  //TODO: do this correctly for perigee frame - right now it's the CL frame 
  track->setCov(localCov);

  // set momentum vector
  track->setMomentumVector(pt_new*cos(track->getPhi()), pt_new*sin(track->getPhi()),p_new*cos(track->getSeedTheta()));

  // set chi2 
  track->setChi2(m_chi2);
  track->setNdf(m_ndf);

  if( m_debug) {
    cout << "HpsGblFitter: Corrections of at reference point " << refLabel << endl;
    cout << "locPar " << endl;
    localPar.Print();
    cout << "prjPerToCl:" << endl;
    track_data->getPrjPerToCl().Print();
    cout << "prjClToPer:" << endl;
    prjClToPer.Print();
    //cout << "locCov " << endl;
    //localCov.Print();
    cout << "clParCorr " << endl;
    clParCorr.Print();
    cout << "perParCorr " << endl;
    perParCorr.Print();
    double curv_corr = kappa_new - track->getSeedKappa();   
    cout << "d0_gbl " << track->getD0() << "(" << track->getSeedD0() << ") z0_gbl " << track->getZ0() << " (" << track->getSeedZ0() << ")" << endl;
    cout << "kappa_gbl " << track->getKappa() << "(" << track->getSeedKappa() << " from q/p_corr " << qP_corr << " qP_old " << qP_old << " qP_new " << qP_new << " pt_new " << pt_new <<  " curv_corr " << curv_corr << " theta " << track->getSeedTheta() <<  " )" << endl;
    track->print();

  }
  
  
  

}

