/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 7, 2013
 *	@version: 1.0
 *
 */

#include <EcalDataWriter.h>

EcalDataWriter::EcalDataWriter()
	: cluster_collection_name("EcalClusters"), 
	  cal_hits_relation_collection_name("EcalCalHitsRelation") 
{}

EcalDataWriter::~EcalDataWriter()
{}

void EcalDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	// Get the collection of Ecal clusters from the event
	clusters = (IMPL::LCCollectionVec*) event->getCollection(cluster_collection_name);

	// Loop over all clusters and fill the event
	for(int cluster_n = 0; cluster_n < clusters->getNumberOfElements(); ++cluster_n){ 

		// Get an Ecal cluster from the LCIO collection
		cluster = (IMPL::ClusterImpl*) clusters->getElementAt(cluster_n);

		// Add a cluster to the HPS Event
		ecal_cluster = hps_event->addEcalCluster();

		// Set the cluster position
		std::vector<double> position(((double*) cluster->getPosition()), ((double*) cluster->getPosition()) + 3); 
		ecal_cluster->setPosition(position);

		// Set the cluster energy
		ecal_cluster->setEnergy(cluster->getEnergy());

		// Get the ecal hits used to create the cluster
		EVENT::CalorimeterHitVec calorimeter_hits = cluster->getCalorimeterHits();

		//
		for(int ecal_hit_n = 0; ecal_hit_n < (int) calorimeter_hits.size(); ++ecal_hit_n){

			calorimeter_hit = (IMPL::CalorimeterHitImpl*) calorimeter_hits[ecal_hit_n];

			ecal_hit = hps_event->addEcalHit();
			ecal_hit->setEnergy(calorimeter_hit->getEnergy());

			int index_x = EcalUtils::getIdentifierFieldValue("ix", calorimeter_hit);
			int index_y = EcalUtils::getIdentifierFieldValue("iy", calorimeter_hit);

			ecal_hit->setCrystalIndices(index_x, index_y);

			ecal_cluster->addHit(ecal_hit);
		}

		// Calculate the shower moments
		/*std::vector<double> moments = EcalUtils::getShowerMoments(cluster, cal_hits_relations);
		ecal_cluster->setM2(moments[1]);
		ecal_cluster->setM3(moments[2]);*/

	}
}
