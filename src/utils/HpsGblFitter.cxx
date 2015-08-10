/**
 * @file: HpsGblFitter.h
 * @brief: GBL track refit
 * @author: Per Hansson Adrian <phansson@slac.stanford.edu>
 *          SLAC
 * @date: February 13, 2014 
 */

#include "HpsGblFitter.h"

// constant to normalize curvature
static const double Bconst = 0.0002998; 
//variable that defines the reference point on the trajectory (typically at path length zero)
static const int refLabel = 1;
//variable indexes
static const unsigned int idx_qOverP = 0;
static const unsigned int idx_xT = 3;
static const unsigned int idx_yT = 4;

HpsGblFitter::HpsGblFitter() 
    : m_traj(NULL), 
      m_r(new TRandom3()),
      b_field(std::numeric_limits<double>::quiet_NaN()),
      b_fac(Bconst*b_field), 
      chi2(-1.), 
      lost_weight(-1.),
      ndf(-1),
      debug(false) {
}

HpsGblFitter::~HpsGblFitter() {
    delete m_r;
    if( m_traj != NULL) {
        delete m_traj;
    }
}

void HpsGblFitter::clear() {
    chi2 = -1.; 
    ndf = -1;
    lost_weight = -1.;
    if( m_traj != NULL) {
        delete m_traj;
        m_traj = NULL;
    }
}

HpsGblFitter::HpsGblFitStatus HpsGblFitter::fit(const GblTrackData* track) {

    // Check that the b-field has been set.  If it hasn't, throw a runtime 
    // exception. The b-field is needed by the GBL fitter.
    if (std::isnan(b_field)) { 
        throw std::runtime_error("[ HpsGblFitter ]: The b-field has not been set.");
    }

    // Time the fits
    //clock_t startTime = clock();

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
    TMatrixD measMsCov(2, 2);
    measMsCov.Zero();
    // Vector of the strip clusters used for the GBL fit
    std::vector<gbl::GblPoint> listOfPoints;

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

        if( debug ) {
            std::cout << "HpsGblFitter: " << "Processing strip " << istrip << " with id/layer " << strip->GetId() << std::endl;
        }

        // Path length step for this cluster
        double step = strip->GetPath3D() - s;

        if( debug ) {
            std::cout << "HpsGblFitter: " << "Path length step " << step << " from " << s << " to " << strip->GetPath3D() << std::endl;
        }

        // Measurement direction (perpendicular and parallel to strip direction)
        TMatrixD mDir(2,3);
        mDir[0][0] = strip->GetU().x();
        mDir[0][1] = strip->GetU().y();
        mDir[0][2] = strip->GetU().z();
        mDir[1][0] = strip->GetV().x();
        mDir[1][1] = strip->GetV().y();
        mDir[1][2] = strip->GetV().z();

        if(debug) {
            std::cout << "HpsGblFitter: " << "mDir" << std::endl;
            mDir.Print();
        }

        TMatrixD mDirT(TMatrixD::kTransposed,mDir);

        if(debug) {
            std::cout << "HpsGblFitter: " << "mDirT" << std::endl;
            mDirT.Print();
        }

        // Track direction 
        double sinLambda = sin(strip->GetLambda());
        double cosLambda = sqrt(1.0 - sinLambda*sinLambda);
        double sinPhi = sin(strip->GetPhi());
        double cosPhi = sqrt(1.0 - sinPhi*sinPhi);

        if(debug) {
            std::cout << "HpsGblFitter: " << "Track direction sinLambda=" << sinLambda << " sinPhi=" << sinPhi << std::endl;
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

        if(debug) {
            std::cout << "HpsGblFitter: " << "uvDir" << std::endl;
            uvDir.Print();
        }

        // projection from  measurement to local (curvilinear uv) directions (duv/dm)
        //TMatrixD proM2l(uvDir,TMatrixD::kMult,mDirT);
        //TMatrixD proM2l(uvDir);
        TMatrixD proM2l = uvDir * mDirT;

        //projection from local (uv) to measurement directions (dm/duv)
        //TMatrixD proL2m(TMatrixD::kInverted,proM2l);
        TMatrixD proL2m(proM2l);
        proL2m.Invert();
        if(proL2m_list.find(strip->GetId()) != proL2m_list.end()) {
            std::cout << "HpsGblFitter: " << strip->GetId() << " was already in list?" << std::endl;
            exit(1);
        }
        proL2m_list[strip->GetId()] = new TMatrixD(proL2m);

        if(debug) {
            std::cout << "HpsGblFitter: " << "proM2l:" <<std::endl;
            proM2l.Print();
            std::cout << "HpsGblFitter: " << "proL2m:" <<std::endl;
            proL2m.Print();
            std::cout << "HpsGblFitter: " << "proM2l*proL2m (should be unit matrix):" <<std::endl;
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

        if (debug) {
            std::cout << "HpsGblFitter: " << "meas: " << std::endl;
            meas.Print();
            std::cout << "HpsGblFitter: " << "measErr:" << std::endl;
            measErr.Print();
            std::cout << "HpsGblFitter: " << "measPrec:" << std::endl;
            measPrec.Print();
        }

        //Find the Jacobian to be able to propagate the covariance matrix to this strip position
        jacPointToPoint = gblSimpleJacobianLambdaPhi(step, cosLambda, fabs(b_fac));

        if (debug) {
            std::cout << "HpsGblFitter: " << "jacPointToPoint to extrapolate to this point:" << std::endl;
            jacPointToPoint.Print();
        }

        // Get the transpose of the Jacobian
        TMatrixD jacPointToPointTransposed(TMatrixD::kTransposed, jacPointToPoint);
        
        // Propagate the MS covariance matrix (in the curvilinear frame) to this strip position
        msCov = msCov*jacPointToPointTransposed;
        msCov = jacPointToPoint*msCov;

        // Get the MS covariance for the measurements in the measurement frame
        TMatrixD proL2mTransposed(TMatrixD::kTransposed, proL2m);
        measMsCov = proL2m*msCov.GetSub(3,4,3,4)*proL2mTransposed;

        if (debug) {
            std::cout << "HpsGblFitter: " << " msCov at this point:" << std::endl;
            msCov.Print();
            std::cout << "HpsGblFitter: " << "measMsCov at this point:" << std::endl;
            measMsCov.Print();
        }

        // Option to blow up measurement error according to multiple scattering
        // if useUncorrMS:
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
            if (debug) {
                std::cout << "HpsGblFitter: " << "adding scatError to this point:" << std::endl;
                scatErr.Print();
            }
        }


        // Add this GBL point to list that will be used in fit
        listOfPoints.push_back(point);
        unsigned int iLabel = listOfPoints.size();


        // Update MS covariance matrix 
        msCov(1, 1) += scatErr[0]*scatErr[0];
        msCov(2, 2) += scatErr[1]*scatErr[1];

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


        if(debug) {
            std::cout << "HpsGblFitter: " << "uRes " <<  strip->GetId() <<  " uRes " << uRes <<  " pred (" <<  strip->GetTrackPos().x() << "," << strip->GetTrackPos().y() << "," << strip->GetTrackPos().z() << ") s(3D) " << strip->GetPath3D() << std::endl;
        }

        //go to next point
        s += step;

        // save strip and label map
        stripLabelMap[strip] = iLabel;
    
    } //strips


    //create the trajectory
    m_traj = new gbl::GblTrajectory(listOfPoints); //,seedLabel, clSeed);

    if (! m_traj->isValid()) {
        std::cout << "HpsGblFitter: " << " Invalid GblTrajectory -> skip" << std::endl;
        return INVALIDTRAJ;
    }
    // fit trajectory
    m_traj->fit(chi2, ndf, lost_weight);
    if( debug ) {
        std::cout << "HpsGblFitter: Chi2 " << " Fit: " << chi2 << ", " << ndf << ", " << lost_weight << std::endl;
    }
    // write to MP binary file
    m_traj->milleOut(mille);

    // clean up local variables allocated
    for(std::map<unsigned int,TMatrixD*>::iterator it = proL2m_list.begin(); it!=proL2m_list.end(); ++it) {
        delete (it->second);
    }



    /*
    clock_t endTime = clock();
    double diff = endTime - startTime;
    double cps = CLOCKS_PER_SEC;
    if( debug ) {
        std::cout << "HpsGblFitter: " << " Time elapsed " << diff / cps << " s" << std::endl;
    }*/

    if(debug) {
        std::cout << "HpsGblFitter: " << "HpsGblFitter: Fit() done successfully." << std::endl;
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


void HpsGblFitter::setTrackProperties(GblTrack* track, const GblTrackData* track_data) {

    // Convert GBL trajectory information to the track object

    if( m_traj == NULL) {
        std::cout << "HpsGblFitter: ERROR there is no trajectory created so can't set properties!" << std::endl;
        return;
    }

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
    double pt_old = fabs((1.0/track_data->getKappa()) * b_fac);
    double qP_old = sin(track_data->getTheta())/pt_old;
    double qP_new = qP_old + qP_corr;
    double p_new = fabs(1/qP_new);
    double pt_new = p_new * sin(track_data->getTheta());
    double kappa_new = 1/pt_new * b_fac;
    double d0_corr = -1.0 * perParCorr[1]; // sign convention of d0 in curvilinear frame
    double z0_corr = perParCorr[2]; 

    // set the new parameters
    // TODO: Use the seed SvtTrack directly to get the seed track parameters
    track->setTrackParameters(track_data->getD0() + d0_corr, track_data->getPhi(), kappa_new, track_data->getTheta(), track_data->getZ0() + z0_corr);

    //set covariance matrix
    //TODO: do this correctly for perigee frame - right now it's the CL frame 
    track->setCov(localCov);

    // set momentum vector
    track->setMomentumVector(pt_new*cos(track->getPhi0()), pt_new*sin(track->getPhi0()), p_new*cos(track_data->getTheta()));

    // set chi2 
    track->setChi2(chi2);
    track->setNdf(ndf);

    if( debug) {
        std::cout << "HpsGblFitter: Corrections of at reference point " << refLabel << std::endl;
        std::cout << "locPar " << std::endl;
        localPar.Print();
        std::cout << "prjPerToCl:" << std::endl;
        track_data->getPrjPerToCl().Print();
        std::cout << "prjClToPer:" << std::endl;
        prjClToPer.Print();
        //std::cout << "locCov " << std::endl;
        //localCov.Print();
        std::cout << "clParCorr " << std::endl;
        clParCorr.Print();
        std::cout << "perParCorr " << std::endl;
        perParCorr.Print();
        double curv_corr = kappa_new - track_data->getKappa();   
        std::cout << "d0_gbl " << track->getD0() << "(" << track_data->getD0() << ") z0_gbl " << track->getZ0() << " (" << track_data->getZ0() << ")" << std::endl;
        std::cout << "kappa_gbl " << track->getKappa() << "(" << track_data->getKappa() << " from q/p_corr " << qP_corr << " qP_old " << qP_old << " qP_new " << qP_new << " pt_new " << pt_new <<  " curv_corr " << curv_corr << " theta " << track_data->getTheta() <<  " )" << std::endl;
        track->toString();
    }
}

void HpsGblFitter::setBField(const double b_field) { 
    this->b_field = b_field; 
    this->b_fac = Bconst*b_field; 
} 
