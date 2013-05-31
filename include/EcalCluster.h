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
        EcalCluster(const EcalCluster &ecalClusterObj);
        virtual ~EcalCluster();
        EcalCluster &operator=(const EcalCluster &ecalClusterObj);

        void Clear(Option_t *option="");

        void setClusterPosition(double*);
        void setClusterEnergy(double energy)    { this->energy = energy; };
        void setNumberOfEcalHits(int n_hits)    { this->n_ecal_hits = n_hits; };
        void setClusterHitTime(double hit_time) { this->hit_time = hit_time; };
        void setHighestEnergyHit(double high_energy) { this->high_energy = high_energy; };
        void setHighestEnergyHitPosition(double*);
        void setM2(double m2) { this->m2 = m2; };
        void setM3(double m3) { this->m3 = m3; };

        double getNumberOfEcalHits() const { return n_ecal_hits; }
        double getXposition()		  const { return x; };
        double getYPosition()		  const { return y; };
        double getZPosition()		  const { return z; };
        double getClusterEnergy()	  const { return energy; };
        double getHighestEnergyHit()  const { return high_energy; };
        double getHighestEnergyHitX() const { return x_high_energy; };
        double getHighestEnergyHitY() const { return y_high_energy; };
        double getM2() const { return m2; };
        double getM3() const { return m3; };

        ClassDef(EcalCluster, 1);	

    private:
        int n_ecal_hits; 

        double x; 
        double y; 
        double z;
        double energy; 
        double hit_time;
        double high_energy;
        double x_high_energy;
        double y_high_energy;
        double z_high_energy;
        double m2;
        double m3;

}; // EcalCluster

#endif // _ECAL_CLUSTER_H_
