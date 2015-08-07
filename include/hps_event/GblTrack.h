/**
 * @file GblTrack.cxx
 * @brief Class used to encapsulate GBL track information.
 * @author Per Hansson Adrian <phansson@slac.stanford.edu>
 *          SLAC
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date February 3, 2014
 */

#ifndef _GBL_TRACK_H_
#define _GBL_TRACK_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TRef.h>
#include <TMatrixD.h>


// Forward declarations
class SvtTrack; 

class GblTrack : public TObject {

    public:
        
        /**
         * Default constructor
         */
        GblTrack();

        /**
         * Destructor
         */
        ~GblTrack();

        /**
         *
         */
        void setSeedTrackParameters(double kappa, double theta, double phi, double d0, double z0);

        /**
         *
         */
        void setTrackParameters(double C, double th, double phi0, double dca, double z);

        /**
         *
         */
        void setChi2(double c);

        /**
         *
         */
        void setNdf(double ndof);

        /**
         *
         */
        void setMomentumVector(double x, double y, double z);

        /**
         *
         */
        void setCov(const TMatrixD& mat);

        /**
         *
         */
        void setSeedTrack(SvtTrack* seed_track) { this->seed_track = (TObject*) seed_track; }; 

        /**
         *
         */
        double getPx() const { return px; };

        /**
         *
         */
        double getPy() const { return py; };

        /**
         *
         */
        double getPz() const { return pz; };

        /**
         *
         */
        double getKappa() const { return kappa;}

        /**
         *
         */
        double getTheta() const { return theta;}

        /**
         *
         */
        double getPhi() const { return phi;}

        /**
         *
         */
        double getD0() const { return d0;}

        /**
         *
         */
        double getZ0() const { return z0;}

        /**
         *
         */
        double getChi2() const { return chi2;}

        /**
         *
         */
        double getNdf() const { return ndf;}

        /**
         *
         */
        double getSeedKappa() const { return seed_kappa;}

        /**
         *
         */
        double getSeedTheta() const { return seed_theta;}

        /**
         *
         */
        double getSeedPhi() const { return seed_phi;}

        /**
         *
         */
        double getSeedD0() const { return seed_d0; }

        /**
         *
         */
        double getSeedZ0() const { return seed_z0; }

        /**
         *
         */
        TRef getSeedTrack() const { return seed_track; };

        /**
         *
         */
        void print();

        ClassDef(GblTrack, 1) //Track class for use with GBL

    private:

            TRef seed_track; 
            TMatrixD cov;

            double kappa;
            double theta;
            double phi;
            double d0;
            double z0;
            double seed_kappa;
            double seed_theta;
            double seed_phi;
            double seed_d0;
            double seed_z0;
            double chi2;
            double ndf;
            double px;
            double py;
            double pz;

}; // GblTrack

#endif // _GBL_TRACK_H_
