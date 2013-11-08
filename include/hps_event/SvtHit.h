/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#ifndef _SVT_HIT_H_
#define _SVT_HIT_H_

//--- C++ ---//
#include <iostream>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

class SvtHit : public TObject { 

    public: 
        SvtHit(); 
        virtual ~SvtHit();
        
        void Clear(Option_t *option="");
        
        void setLayer(int hit_layer) { this->hit_layer = hit_layer; };
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
