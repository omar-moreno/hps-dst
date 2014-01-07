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
}

HpsEventBuilder::~HpsEventBuilder()
{
	delete svt_writer; 
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event)
{
	hps_event->Clear(); 
	svt_writer->writeData(event, hps_event);
}
