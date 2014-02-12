/**
 *	@section purpose: 
 *	@author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *	@date: January 6, 2014
 *	@version: 1.0
 *
 */

#include <HpsEventBuilder.h>

HpsEventBuilder::HpsEventBuilder()
{
	svt_writer = new SvtDataWriter(); 
	ecal_writer = new EcalDataWriter();
	mc_particle_writer = new MCParticleDataWriter();
}

HpsEventBuilder::~HpsEventBuilder()
{
	delete svt_writer;
    delete ecal_writer; 	
    delete mc_particle_writer;
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	hps_event->Clear(); 
	
	// Set the event number
	hps_event->setEventNumber(event->getEventNumber());

	// Set the run number
	hps_event->setRunNumber(event->getRunNumber());

	// Write SVT data to HpsEvent
	svt_writer->writeData(event, hps_event);
	
	// Write Ecal data to HpsEvent
	ecal_writer->writeData(event, hps_event); 

	// Write MC particle data to HpsEvent
	mc_particle_writer->writeData(event, hps_event);
}

void HpsEventBuilder::setBField(double b_field){
	svt_writer->setBField(b_field); 
}
