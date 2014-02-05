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
    gbl_writer  = new GblDataWriter();
}

HpsEventBuilder::~HpsEventBuilder()
{
	delete svt_writer;
    delete ecal_writer; 	
    delete gbl_writer;
    
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	hps_event->Clear(); 
	
	// Write SVT Data to HpsEvent
	svt_writer->writeData(event, hps_event);
	
	// Write Ecal Data to HpsEvent
	ecal_writer->writeData(event, hps_event); 

    // Do GBL track re-fit
    if( getGblFlag() == true ) {
      gbl_writer->writeData(event, hps_event);
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
