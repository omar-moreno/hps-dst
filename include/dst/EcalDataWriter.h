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

//--- HPS Event ---//
//-----------------//
#include <EcalCluster.h>

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
		IMPL::LCCollectionVec* cal_hits_relations; 		
		IMPL::ClusterImpl* cluster; 

		EcalCluster* hps_ecal_cluster;


};

#endif // __ECAL_DATA_WRITER_H__
