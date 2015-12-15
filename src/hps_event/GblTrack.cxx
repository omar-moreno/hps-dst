/**
 * 
 * @file SvtTrack.h
 * @brief Class used to describe an HPS SVT GBL track.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date December 11, 2015
 * 
 */

#include <GblTrack.h>

ClassImp(GblTrack)

GblTrack::GblTrack() 
    : SvtTrack() {  

          // Initialize the lamda and phi kinks to a default value
          std::fill_n(lambda_kinks, 12, -9999); 
          std::fill_n(phi_kinks, 12, -9999); 
}

      
