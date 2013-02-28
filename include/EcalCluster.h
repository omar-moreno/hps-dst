
#ifndef _ECAL_CLUSTER_H_
#define _ECAL_CLUSTER_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

using namespace std; 

class EcalCluster : public TObject { 

    public: 
        EcalCluster(); 
        ~EcalCluster();

        //--- Setters ---//
        //---------------//
        
        void Clear(Option_t *option="");
        
        void setClusterPosition(double*);
        void setClusterEnergy(double);
        void setNumberOfEcalHits(int);  

        ClassDef(EcalCluster, 1);	
    
    private:
        int n_ecal_hits; 

        double x; 
        double y; 
        double z;
        double energy; 

};

#endif
