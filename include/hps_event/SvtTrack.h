/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @date:       February 19, 2013
 *
 */

#ifndef __SVT_TRACK_H__
#define __SVT_TRACK_H__

//--- C++ ---//
//-----------//
#include <iostream>

//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>

//--- HPS Event ---//
//-----------------//
#include <SvtHit.h>

class SvtTrack : public TObject {

    public:

		SvtTrack();
        SvtTrack(const SvtTrack &svtTrackObj);
        ~SvtTrack();
        SvtTrack &operator=(const SvtTrack &svtTrackObj);
        
        void Clear(Option_t *option="");

        void addHit(SvtHit*); 

        void setTrackParameters(double, double, double, double, double);
        void setChi2(double chi_squared){ this->chi_squared = chi_squared; };
        void setL1Isolation(double l1_isolation) {this->l1_isolation = l1_isolation; };
        void setL2Isolation(double l2_isolation) {this->l2_isolation = l2_isolation; };

        double getD0() const { return d0; };
        double getPhi() const { return phi; };
        double getOmega() const { return omega; };
        double getTanLambda() const { return tan_lambda; };
        double getZ0() const { return z0; };
        double getChi2() const {return chi_squared; };
        double getL1Isolation() const {return l1_isolation; };
        double getL2Isolation() const {return l2_isolation; };
        TRefArray* getSvtHits() const;

        ClassDef(SvtTrack, 1);

    private:

        TRefArray *svt_hits; 

        int n_hits; 

        double d0; 
        double phi;
        double omega;  
        double tan_lambda; 
        double z0;  
        double chi_squared;
        double l1_isolation;
        double l2_isolation;

}; // SvtTrack

#endif // __SVT_TRACK_H__
