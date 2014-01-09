/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 1.0
 * @date:       February 19, 2013
 */

#ifndef _ECAL_CLUSTER_H_
#define _ECAL_CLUSTER_H_

//--- C++ ---//
//-----------//
#include <iostream>

//-- ROOT ---//
//-----------//
#include <TObject.h>
#include <TClonesArray.h>

class EcalCluster : public TObject {

    public:

        EcalCluster();
        EcalCluster(const EcalCluster &ecalClusterObj);
        ~EcalCluster();
        EcalCluster &operator=(const EcalCluster &ecalClusterObj);

        void Clear(Option_t *option="");

        void setClusterPosition(double*);
        void setClusterEnergy(double energy)    { this->energy = energy; };
        void setNumberOfEcalHits(int n_hits)    { this->n_ecal_hits = n_hits; };
        void setClusterHitTime(double hit_time) { this->hit_time = hit_time; };
        void setSeedEnergy(double seed_energy)  { this->seed_energy = seed_energy; };
        void setSeedPosition(double*);
        void setM2(double m2) { this->m2 = m2; };
        void setM3(double m3) { this->m3 = m3; };

        double getNumberOfEcalHits() const { return n_ecal_hits; }
        double getXposition()		  const { return x; };
        double getYPosition()		  const { return y; };
        double getZPosition()		  const { return z; };
        double getClusterEnergy()	  const { return energy; };
        double getSeedEnergy()  const { return seed_energy; };
        double getSeedX() const { return seed_x; };
        double getSeedY() const { return seed_y; };
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
        double seed_energy;
        double seed_x;
        double seed_y;
		double seed_z; 
        double m2;
        double m3;

}; // EcalCluster

#endif // _ECAL_CLUSTER_H_
