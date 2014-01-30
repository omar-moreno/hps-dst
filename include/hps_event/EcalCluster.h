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
#include <TRefArray.h>
#include <TRef.h>

//--- HPS Event ---//
//-----------------//
#include <Cluster.h>
#include <EcalHit.h>

class EcalCluster : public Cluster, public TObject {

    public:

        EcalCluster();
        EcalCluster(const EcalCluster &);
        ~EcalCluster();
        EcalCluster &operator=(const EcalCluster &);

        void Clear(Option_t *option="");

		void addHit(EcalHit*); 

        void setPosition(const std::vector<double>);
        void setEnergy(const double energy){ this->energy = energy; };
        void setHitTime(const double hit_time){ this->hit_time = hit_time; };
        void setM2(const double m2){ this->m2 = m2; };
        void setM3(const double m3){ this->m3 = m3; };

		std::vector<double> getPosition() const;  
        double getEnergy() const { return energy; };
        double getSeedEnergy() const;
		std::vector<double> getSeedPosition() const; 
		double getM2() const { return m2; };
        double getM3() const { return m3; };

        ClassDef(EcalCluster, 1);	

    private:
		
		TRefArray* ecal_hits; 
		TRef seed_hit; 

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
