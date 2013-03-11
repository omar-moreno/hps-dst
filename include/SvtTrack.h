/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#ifndef _SVT_TRACK_H_
#define _SVT_TRACK_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

//---//
#include <TRefArray.h>
#include <SvtHit.h>

class SvtTrack : public TObject {

    public:
        SvtTrack();
        SvtTrack(const SvtTrack &svtTrackObj);
        virtual ~SvtTrack();
        SvtTrack &operator=(const SvtTrack &svtTrackObj);
        
        void Clear(Option_t *option="");
        void addHit(SvtHit*); 

        void setTrackParameters(double, double, double, double, double);
        void setMomentum(double, double, double);

        double getPx()  	  const { return px; };
        double getPy()  	  const { return py; };
        double getPz()  	  const { return pz; };
        double getD0()  	  const { return d0; };
        double getPhi()       const { return phi; };
        double getOmega() 	  const { return omega; };
        double getTanLambda() const { return tan_lambda; };
        double getZ0() 		  const { return z0; };

        ClassDef(SvtTrack, 1);

    private:
        TRefArray *svt_hits; 

        int n_hits; 

        double px;
        double py; 
        double pz;
        double d0; 
        double phi;
        double omega;  
        double tan_lambda; 
        double z0;  
}; // SvtTrack

#endif // _SVT_TRACK_H_
