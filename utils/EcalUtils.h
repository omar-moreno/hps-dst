/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 * @version     v 0.1
 * @date        April 22, 2013
 */


//--- C++ ---//
#include <iostream>
#include <math.h>

//--- lcio ---//
#include <IMPL/ClusterImpl.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <EVENT/LCEvent.h>
#include <IMPL/LCRelationImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>

//--- ROOT ---//
#include <TVector3.h>

namespace EcalUtils {
        
	double* getShowerMoments(IMPL::ClusterImpl*, IMPL::LCCollectionVec*);

};



