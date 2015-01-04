/**
 *	@file   HpsEventBuilder.cxx
 *	@brief
 *	@author Omar Moreno <omoreno1@ucsc.edu>
 *		    Santa Cruz Institute for Particle Physics
 *			University of California, Santa Cruz
 *	@date   January 6, 2014
 *
 */

#include <HpsEventBuilder.h>

HpsEventBuilder::HpsEventBuilder() 
    : svt_writer(new SvtDataWriter()), ecal_writer(new EcalDataWriter()),
      mc_particle_writer(new MCParticleDataWriter()),
      particle_writer(new HpsParticleDataWriter()),
      gbl_data_writer(new GblDataWriter()),
      gbl_track_writer(new GblTrackWriter()),
      run_gbl(false), ecal_only(false)
{}

HpsEventBuilder::~HpsEventBuilder() {
	delete svt_writer;
	delete ecal_writer; 	
	delete mc_particle_writer;
	delete particle_writer; 
	delete gbl_data_writer;
	delete gbl_track_writer;
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event) {
	
    // Clear the HpsEvent from all previous information
    hps_event->Clear();

    //  Check that the event contains more than just the trigger collection.
    //  This is a temp solution to the recon not having an ECal collection
    //  in every event.
    //std::cout << event->getCollectionNames()->size() << std::endl;
    if (event->getCollectionNames()->size() <= 2) return;  

	// Set the event number
	hps_event->setEventNumber(event->getEventNumber());
    //std::cout << event->getEventNumber() << std::endl;

	// Set the run number
	hps_event->setRunNumber(event->getRunNumber());

	// Write Ecal data to the HpsEvent
	ecal_writer->writeData(event, hps_event); 

    // If only Ecal data is going to be written to the DST, skip the rest
    // of the writers
    if (ecal_only) return;

	// Write SVT data to the HpsEvent
	svt_writer->writeData(event, hps_event);

	// Write MC particle data to the HpsEvent
	mc_particle_writer->writeData(event, hps_event);

	// Write the HpsParticle data to the HpsEvent
	particle_writer->writeData(event, hps_event); 

	// If GBL has been enabled, process the SVT tracks using GBL and write the
    // data to the HpsEvent
    if (!run_gbl) return;

	// Write info used for GBL to the HpsEvent
	gbl_data_writer->writeData(event, hps_event);

	// Write GBL track refit to the HpsEvent    
	gbl_track_writer->writeData(hps_event);
}

void HpsEventBuilder::setBField(const double b_field) {
	gbl_track_writer->setBField(b_field);
}
