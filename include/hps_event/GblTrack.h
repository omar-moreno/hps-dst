/**
 * @file: GblTrack.h
 * @author: Per Hansson <phansson@slac.stanford.edu>
 * @section Institutiion \n
 *          SLAC
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section Institution \n
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date: February 3, 2014
 */

#ifndef __GBL_TRACK_H__
#define __GBL_TRACK_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <iostream>

//------------//
//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TMatrixD.h>

class GblTrack : public TObject {

    public:

        /**
         * Default Constructor
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
        double getPx()  	  const { return px; };
        
        /**
         *
         */
        double getPy()  	  const { return py; };
        
        /**
         *
         */
        double getPz()  	  const { return pz; };
        
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
        void print();

        ClassDef(GblTrack,1) //Track class for use with GBL

    private:
           
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
            TMatrixD cov;

}; // GblTrack

#endif // _GBL_TRACK_H_
