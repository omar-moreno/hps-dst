

#ifndef _TRACK_H_
#define _TRACK_H_

//--- C++ ---//
#include <iostream>

//--- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

//---//
#include <TRefArray.h>
#include <SvtHit.h>

using namespace std; 

class Track : public TObject { 

    public:
        Track();
        ~Track(); 

        //--- methods ---//
        //---------------//
        
        void Clear(Option_t *option="");
        void addHit(SvtHit*); 

        void setTrackParameters(double, double, double, double, double);
        void setMomentum(double, double, double);

        ClassDef(Track, 1);

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
};

#endif
