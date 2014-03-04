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

HpsEventBuilder::HpsEventBuilder() : m_run_gbl(false), m_debug(false)
{
	svt_writer = new SvtDataWriter(); 
	ecal_writer = new EcalDataWriter();
	mc_particle_writer = new MCParticleDataWriter();
    gbl_data_writer  = new GblDataWriter();
    gbl_track_writer  = new GblTrackWriter();
}

HpsEventBuilder::~HpsEventBuilder()
{
	delete svt_writer;
    delete ecal_writer; 	
    delete mc_particle_writer;
    delete gbl_data_writer;
    delete gbl_track_writer;
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

    // Write GBL data to HpsEvent
    if( getGblFlag() == true ) {

      if(m_debug) {
        std::cout << "makeHpsEvent: write gbl data to event " << std::endl;
      }

      // Write info used for GBL to HpsEvent
      gbl_data_writer->writeData(event, hps_event);

      if(m_debug) {
        std::cout << "makeHpsEvent: " << hps_event->getNumberOfGblStripData() << " GBL strips available for the GBL track writer" << std::endl;
      }

      // Write GBL track refit to HpsEvent    
      gbl_track_writer->writeData(hps_event);
      
      if(m_debug) {
        std::cout << "makeHpsEvent: made " << hps_event->getNumberOfGblTracks() << " GBL tracks from " << hps_event->getNumberOfGblTracksData() << " GBL track data objects" << std::endl;
      }
    }
    
}

void HpsEventBuilder::setBField(double b_field){
	svt_writer->setBField(b_field); 
    gbl_track_writer->setBField(b_field);
}

void HpsEventBuilder::setDebug(bool debug) {
  m_debug = debug;
  //gbl_data_writer->setDebug(debug);
  //gbl_track_writer->setDebug(debug);
}

void HpsEventBuilder::setGblFlag(bool flag) {
  m_run_gbl = flag;
}

 bool HpsEventBuilder::getGblFlag() const{
   return m_run_gbl;
}
