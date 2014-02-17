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

HpsEventBuilder::HpsEventBuilder() : m_run_gbl(false)
{
	svt_writer = new SvtDataWriter(); 
	ecal_writer = new EcalDataWriter(); 
    gbl_data_writer  = new GblDataWriter();
    gbl_track_writer  = new GblTrackWriter();
}

HpsEventBuilder::~HpsEventBuilder()
{
	delete svt_writer;
    delete ecal_writer; 	
    delete gbl_data_writer;
    delete gbl_track_writer;
    
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	hps_event->Clear(); 
	
	// Write SVT Data to HpsEvent
	svt_writer->writeData(event, hps_event);
	
	// Write Ecal Data to HpsEvent
	ecal_writer->writeData(event, hps_event); 

    if( getGblFlag() == true ) {
      // Write info for GBL to HpsEvent
      gbl_data_writer->writeData(event, hps_event);
      // Write GBL track refit to HpsEvent    
      std::cout << "makeHpsEvent: " << hps_event->getNumberOfGblStripData() << " GBL strips " << std::endl;
      gbl_track_writer->writeData(hps_event);
      std::cout << "makeHpsEvent: " << hps_event->getNumberOfGblTracksData() << " GBL tracks data " << std::endl;
      std::cout << "makeHpsEvent: " << hps_event->getNumberOfGblTracks() << " GBL tracks " << std::endl;
    }

}

void HpsEventBuilder::setBField(double b_field){
	svt_writer->setBField(b_field); 
}

void HpsEventBuilder::setGblFlag(bool flag) {
  m_run_gbl = flag;
}

 bool HpsEventBuilder::getGblFlag() const{
   return m_run_gbl;
}
