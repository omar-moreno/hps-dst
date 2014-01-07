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
	cal_hits_relations 
		= (IMPL::LCCollectionVec*) event->getCollection(cal_hits_relation_collection_name);

	// Loop over all clusters and fill the event
	EVENT::CalorimeterHit* cluster_seed = NULL; 
	for(int cluster_n = 0; cluster_n < clusters->getNumberOfElements(); ++cluster_n){ 

		// Get an Ecal cluster from the LCIO collection
		cluster = (IMPL::ClusterImpl*) clusters->getElementAt(cluster_n);

		// Add a cluster to the HPS Event
		hps_ecal_cluster = hps_event->addEcalCluster();

		// Set the cluster position
		hps_ecal_cluster->setClusterPosition((double*) cluster->getPosition());


		// Set the cluster energy
		hps_ecal_cluster->setClusterEnergy(cluster->getEnergy());

		// Get the ecal hits used to create the cluster
		EVENT::CalorimeterHitVec ecal_hits = cluster->getCalorimeterHits(); 

		// Set the number of crystals associated with the cluster
		hps_ecal_cluster->setNumberOfEcalHits(ecal_hits.size());

		// Calculate the shower moments
		std::vector<double> moments = EcalUtils::getShowerMoments(cluster, cal_hits_relations); 	
		hps_ecal_cluster->setM2(moments[1]);
		hps_ecal_cluster->setM3(moments[2]);

		cluster_seed = EcalUtils::getClusterSeed(cluster); 
		
        // Set the energy and position of the highest energy crystal in
        // the cluster
        hps_ecal_cluster->setHighestEnergyHit(cluster_seed->getEnergy());
        hps_ecal_cluster->setHighestEnergyHitPosition((double*) cluster_seed->getPosition());

	}

	delete cluster_seed; 
}
