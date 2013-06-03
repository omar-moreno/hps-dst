/**
 * @author: Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 *              Santa Cruz Institute for Particle Physics
 *              University of California, Santa Cruz
 * @version     v 0.1
 * @date        April 22, 2013
 */

#include <EcalUtils.h>

namespace EcalUtils {

	double* getShowerMoments(IMPL::ClusterImpl* cluster, IMPL::LCCollectionVec* ecal_cal_hits_relations)
	{

		// Get the cluster position
		TVector3 cluster_position(cluster->getPosition());
		std::cout << "Cluster position: [ " << cluster_position(0)
				  << ", " << cluster_position(1)
				  << ", " << cluster_position(2) << " ] " << std::endl;

		// Get the cluster energy
		double cluster_energy = cluster->getEnergy();
		std::cout << "Cluster energy: " << cluster_energy << std::endl;

		// Get the calorimeter hits associated with the cluster
		EVENT::CalorimeterHitVec cluster_hits = cluster->getCalorimeterHits();
		int n_hits = cluster_hits.size();

		// Loop over all calorimeter hits and calculate the shower moments
		int n_moments = 3;
		double moments[3] = {0};
		IMPL::CalorimeterHitImpl *cluster_hit = NULL;
		IMPL::LCRelationImpl* ecal_hits_relation = NULL;
		TVector3 hit_position;
		for(int hit_n = 0; hit_n < n_hits; ++hit_n){
			cluster_hit = (IMPL::CalorimeterHitImpl*) cluster_hits[hit_n];
			for(int rel_n = 0; rel_n < ecal_cal_hits_relations->getNumberOfElements(); ++rel_n){
				std::cout << "Searching for hit match" << std::endl;
				ecal_hits_relation = (IMPL::LCRelationImpl*) ecal_cal_hits_relations->getElementAt(rel_n);
				if(ecal_hits_relation->getFrom() == cluster_hit){
					std::cout << "Found hit match" << std::endl;
					hit_position.SetXYZ(
							((IMPL::LCGenericObjectImpl*) ecal_hits_relation->getTo())->getDoubleVal(0),
							((IMPL::LCGenericObjectImpl*) ecal_hits_relation->getTo())->getDoubleVal(1),
							((IMPL::LCGenericObjectImpl*) ecal_hits_relation->getTo())->getDoubleVal(2));
					break;
				}
			}
			std::cout << "Cluster hit position: [ " << hit_position(0)
					  << ", " << hit_position(1)
				      << ", " << hit_position(2) << " ] " << std::endl;
			double r = (hit_position - cluster_position).Mag ();
			std::cout << "Residual " << hit_n << " : " <<  r << std::endl;
			moments[0] += cluster_hits [hit_n]-> getEnergy ()*r;
			moments[1] += cluster_hits[hit_n]->getEnergy()*pow(r,2);
			moments[2] += cluster_hits [hit_n]-> getEnergy ()*pow (r, 3);
		}

		for (int n_moment = 0; n_moment < n_moments; ++n_moment){
			moments[n_moment] /= cluster_energy;
		}

		std::cout << " Shower M1: " << moments[0]
		          << " Shower M2: " << moments[1]
		          << " Shower M3: " << moments[2] << std::endl;

		return moments;
	}
};
