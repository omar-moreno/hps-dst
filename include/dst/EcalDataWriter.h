/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 2, 2013
 *	@version: 1.0
 *
 */

#ifndef __ECAL_DATA_WRITER_H__
#define __ECAL_DATA_WRITER_H__

//--- DST ---//
//-----------//
#include <DataWriter.h>

//--- Utils ---//
//-------------//
#include <EcalUtils.h>

//--- LCIO ---//
//------------//
#include <IMPL/LCCollectionVec.h>
#include <IMPL/ClusterImpl.h>
#include <IMPL/CalorimeterHitImpl.h>

//--- HPS Event ---//
//-----------------//
#include <EcalCluster.h>
#include <EcalHit.h>

class EcalDataWriter : public DataWriter {

	public: 
		
		EcalDataWriter(); 
		~EcalDataWriter(); 

		// 
		void writeData(EVENT::LCEvent*, HpsEvent*); 	

	private:

		std::string cluster_collection_name;
		std::string cal_hits_relation_collection_name; 

		IMPL::LCCollectionVec* clusters;  
		IMPL::ClusterImpl* cluster;
		IMPL::CalorimeterHitImpl* calorimeter_hit;

		EcalCluster* ecal_cluster;
		EcalHit* ecal_hit;

};

#endif // __ECAL_DATA_WRITER_H__
