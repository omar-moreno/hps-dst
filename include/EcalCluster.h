/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#ifndef _ECAL_CLUSTER_H_
#define _ECAL_CLUSTER_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

class EcalCluster : public TObject {

    public: 
        EcalCluster();
        virtual ~EcalCluster();

        void Clear(Option_t *option="");

        void setClusterPosition(double*);
        void setClusterEnergy(double energy) { this->energy = energy; };
        void setNumberOfEcalHits(int n_hits) { this->n_ecal_hits = n_hits; };

        double getNumberOfEcalHits() const { return n_ecal_hits; }
        double getXposition()		 const { return x; };
        double getYPosition()		 const { return y; };
        double getZPosition()		 const { return z; };
        double getClusterEnergy()	 const { return energy; };

        ClassDef(EcalCluster, 1);	

    private:
        int n_ecal_hits; 

        double x; 
        double y; 
        double z;
        double energy; 

}; // EcalCluster

#endif // _ECAL_CLUSTER_H_
