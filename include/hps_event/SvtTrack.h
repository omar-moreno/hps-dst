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

//------------------//
//--- C++ StdLib ---//
//------------------//
#include<vector>
#include <stdio.h>

//------------//
//--- ROOT ---//
//------------//
#include <TObject.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TRef.h>

//-----------------//
//--- HPS Event ---//
//-----------------//
#include <HpsParticle.h>

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
         * @param phi0 : 
         * @param omega : 
         * @param tan_lambda : 
         * @param z0 : 
         */
        void setTrackParameters(const double d0, 
                const double phi0, 
                const double omega, 
                const double tan_lambda,
                const double z0);
        
        /**
         * Set the chi^2 of the fit to the track.
         *
         * @param chi_squared : The chi^2 of the fit to the track.
         */
        void setChi2(const double chi_squared) { this->chi_squared = chi_squared; };
       
        /**
         * Set the track time.
         *
         * @param track_time : The track time.
         */
        void setTrackTime(const double track_time) { this->track_time = track_time; };

        /**
         *
         */ 
        void setIsolation(const int layer, const double isolation) { this->isolation[layer] = isolation; };

        /**
         *
         */
        void setTrackVolume(const int track_volume) { this->track_volume = track_volume; };

        /**
         * Set the HpsParticle associated with this track.  This can be used to
         * retrieve additional track properties such as the momentum and charge.
         *
         * @param fs_particle : Final state HpsParticle associated with this track
         */
        void setParticle(HpsParticle* fs_particle) { this->fs_particle = (TObject*) fs_particle; };

        /**
         *
         */
        double getD0() const { return d0; };
        
        /**
         *
         */
        double getPhi0() const { return phi0; };
        
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
        double getTrackTime() const { return track_time; }; 

        /**
         *
         */
        double getIsolation(const int layer) const { return isolation[layer]; }; 

        /**
         *
         */
        int getCharge(); 

        /**
         *
         */
        std::vector<double> getMomentum(); 
        
        /**
         *
         */
        TRefArray* getSvtHits() const;

        /**
         *
         */
        TRef getParticle() const { return fs_particle; }; 

        /**
         *
         */
        bool isTopTrack() const { return track_volume ? false : true; };

        /**
         *
         */
        bool isBottomTrack() const { return track_volume ? true : false; };

        /**
         *
         */
        ClassDef(SvtTrack, 1);

    private:

        /** Reference to the 3D hits associated with this track */
        TRefArray* svt_hits; 

        /** Reference to the reconstructed particle associated with this track */
        TRef fs_particle;

        /** Array used to store the isolation variables for each of the sensor layers */
        double isolation[12];

        /** The number of 3D hits associated with this track */
        int n_hits; 

        /** The volume to which this track belongs to */
        int track_volume; 

        double d0; 
        double phi0;
        double omega;  
        double tan_lambda; 
        double z0;  
        double chi_squared;
        double track_time;

}; // SvtTrack

#endif // __SVT_TRACK_H__
