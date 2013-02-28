
#include<EcalCluster.h>

ClassImp(EcalCluster)

EcalCluster::EcalCluster()
{
    n_ecal_hits = 0; 
}

EcalCluster::~EcalCluster()
{
    Clear(); 
}

void EcalCluster::Clear(Option_t* /*option*/)
{
    TObject::Clear();
}

void EcalCluster::setClusterPosition(double *position)
{
    x = position[0]; 
    y = position[1]; 
    z = position[2];
}

void EcalCluster::setClusterEnergy(double cluster_energy)
{
    energy = cluster_energy; 
}

void EcalCluster::setNumberOfEcalHits(int n_hits)
{
    n_ecal_hits = n_hits; 
    
}
