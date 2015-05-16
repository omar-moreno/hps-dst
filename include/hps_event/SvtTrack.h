/**
 * @file: SvtTrack.h
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section Institution \n
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 * @date:   February 19, 2013
 */

#ifndef __SVT_TRACK_H__
#define __SVT_TRACK_H__

//------------//
//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>

// Forward declarations
class SvtHit;

class SvtTrack : public TObject {

    // TODO: Add more documentation

    public:

        /**
         * Default Constructor
         */
        SvtTrack();

        /**
         * Copy constructor
         *
         * @param svtTrackObj : An SvtTrack object
         */
        SvtTrack(const SvtTrack &svtTrackObj);

        /**
         * Destructor
         */
        ~SvtTrack();

        /**
         *
         */
        SvtTrack &operator=(const SvtTrack &svtTrackObj);
       
        /**
         *
         */ 
        void Clear(Option_t *option="");

        /**
         * Add a reference to an SvtHit.
         *
         * @param hit : An SvtHit
         */
        void addHit(SvtHit* hit); 

        /**
         * Set the track parameters.
         *
         * @param d0 : Distance of closest approach
         * @param phi : 
         * @param omega : 
         * @param tan_lambda : 
         * @param z0 : 
         */
        void setTrackParameters(double d0, double phi, double omega, double tan_lambda, double z0);
        
        /**
         * Set the chi^2 of the fit to the track.
         *
         * @param chi_squared : The chi^2 of the fit to the track.
         */
        void setChi2(double chi_squared) { this->chi_squared = chi_squared; };
       
        /**
         * Set the track time.
         *
         * @param track_time : The track time.
         */
        void setTrackTime(double track_time) { this->track_time = track_time; };

        /**
         *
         */
        void setL1Isolation(double l1_isolation) { this->l1_isolation = l1_isolation; };
        
        /**
         *
         */
        void setL2Isolation(double l2_isolation) { this->l2_isolation = l2_isolation; };

        /**
         *
         */
        double getD0() const { return d0; };
        
        /**
         *
         */
        double getPhi() const { return phi; };
        
        /**
         *
         */
        double getOmega() const { return omega; };
        
        /**
         *
         */
        double getTanLambda() const { return tan_lambda; };
        
        /**
         *
         */
        double getZ0() const { return z0; };
        
        /**
         *
         */
        double getChi2() const { return chi_squared; };
        
        /**
         *
         */
        double getL1Isolation() const { return l1_isolation; };
        
        /**
         *
         */
        double getL2Isolation() const { return l2_isolation; };
        
        /**
         *
         */
        TRefArray* getSvtHits() const;

        /**
         *
         */
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
        double track_time;
        double l1_isolation;
        double l2_isolation;

}; // SvtTrack

#endif // __SVT_TRACK_H__
