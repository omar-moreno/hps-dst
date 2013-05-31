/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 * @version     v 0.1
 * @date        April 22, 2013
 */

#include <EcalUtils.h>

namespace EcalUtil {

double getClusterM2(IMPL::ClusterImpl* cluster, IMPL::LCCollectionVec* ecal_cal_hits_relations)
{

	// Get the cluster position
	double m2_x = cluster->getPosition()[0];
	std::cout << "Cluster position: [ " << cluster->getPosition()[0]
	          << ", " << cluster->getPosition()[1]
	          << ", " << cluster->getPosition()[2] << " ] " << std::endl;

	// Get the cluster energy
	double cluster_energy = cluster->getEnergy();
	std::cout << "Cluster energy: " << cluster_energy << std::endl;

	// Get the calorimeter hits associated with the cluster
    EVENT::CalorimeterHitVec cluster_hits = cluster->getCalorimeterHits();
	int n_hits = cluster_hits.size();

	// Loop over all calorimeter hits and calculate m2
	double m2 = 0;
	IMPL::CalorimeterHitImpl *cluster_hit = NULL;
    IMPL::LCRelationImpl* ecal_cal_hits_relation = NULL;
	for(int hit_n = 0; hit_n < n_hits; ++hit_n){
		cluster_hit = (IMPL::CalorimeterHitImpl*) cluster_hits[hit_n];
		float position[3] = { 0 };
		for(int rel_n = 0; rel_n < ecal_cal_hits_relations->getNumberOfElements(); ++rel_n){
			std::cout << "Searching for hit match" << std::endl;
			ecal_cal_hits_relation = (IMPL::LCRelationImpl*) ecal_cal_hits_relations->getElementAt(rel_n);
			if(ecal_cal_hits_relation->getFrom() == cluster_hit){
				// Quick hack to cast double array to float array ... will fix later
				std::cout << "Found hit match" << std::endl;
				for(int i = 0; i < 3; ++i){
					position[i] = (float) ((IMPL::LCGenericObjectImpl*) ecal_cal_hits_relation->getTo())->getDoubleVal(i);
				}
	//			cluster_hit->setPosition(position);
				break;
			}
		}
		std::cout << "Cluster hit position: [ " << position[0]
	          << ", " << position[1]
	          << ", " << position[2] << " ] " << std::endl;
		double res = position[0] - m2_x;
		std::cout << "Residual " << hit_n << " : " << res << std::endl;
		m2 += cluster_hits[hit_n]->getEnergy()*pow((position[0] - m2_x),2);
	}

	m2 /= cluster_energy;

	std::cout << "M2: " << m2 << std::endl;

	return m2;
}

double getClusterM3(IMPL::ClusterImpl* cluster, IMPL::LCCollectionVec* ecal_cal_hits_relation)
{


	// Get the cluster position
	double m3_x = cluster->getPosition()[0];

	// Get the cluster energy
	double cluster_energy = cluster->getEnergy();

	// Get the calorimeter hits associated with the cluster
    EVENT::CalorimeterHitVec cluster_hits = cluster->getCalorimeterHits();
	int n_hits = cluster_hits.size();

	// Loop over all calorimeter hits and calculate m2
	double m3 = 0;
	for(int hit_n = 0; hit_n < n_hits; ++hit_n){
		m3 += cluster_hits[hit_n]->getEnergy()*pow((cluster_hits[hit_n]->getPosition()[0] - m3_x),3);
	}

	m3 /= cluster_energy;

	std::cout << "M3: " << m3 << std::endl;

	return m3;
}

};
