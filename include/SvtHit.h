
#ifndef _SVT_HIT_H_
#define _SVT_HIT_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

using namespace std; 

class SvtHit : public TObject { 

    public: 
        SvtHit(); 
        ~SvtHit();

        //--- Setters ---//
        //---------------//
        
        void Clear(Option_t *option="");
        
        void setLayer(int); 
        void setHitPosition(const double*);
        void setHitPositionError(double, double, double);

        ClassDef(SvtHit, 1);	
    
    private:
        
        

        int hit_layer;
        double x; 
        double y; 
        double z;

        double x_err; 
        double y_err; 
        double z_err;  

};

#endif
