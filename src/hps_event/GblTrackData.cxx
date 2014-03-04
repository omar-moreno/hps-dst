/**
 * @section purpose: Stores track information needed by GBL
 * @author: 	Per Hansson Adrian <phansson@slac.stanford.edu>
 * @version:    v1.0
 * @date:       February 3, 2014
 */

#include <GblTrackData.h>
#include <GblStripData.h>

//-- c++ --//
#include <sstream>

//-- ROOT --//
//#include <TIter.h>


ClassImp(GblTrackData)

GblTrackData::GblTrackData()
: 	TObject(), m_gbl_strip_hits(new TRefArray()), n_gbl_strip_hits(0), m_prjPerToCl(3,3)
{}

void GblTrackData::Clear(Option_t* /* option */) {
  TObject::Clear();
  m_gbl_strip_hits->Delete();
  n_gbl_strip_hits = 0;
}

GblTrackData::~GblTrackData()
{
  delete m_gbl_strip_hits;
}

void GblTrackData::addStrip(GblStripData* strip) {
  ++n_gbl_strip_hits;
  m_gbl_strip_hits->Add(strip);
}

std::string GblTrackData::toString() const {
    std::ostringstream oss;
    oss << "GblTrackData:\n";
    oss << "\t" << "kappa " << getKappa() << "\n";
    oss << "\t" << "theta " << getTheta() << "\n";
    oss << "\t" << "phi   " << getPhi() << "\n";
    oss << "\t" << "d0    " << getD0() << "\n";
    oss << "\t" << "z0    " << getZ0() << "\n";
    oss << "\t" << n_gbl_strip_hits << " GBL strips:\n";
    TIter iter(m_gbl_strip_hits->MakeIterator());
    const GblStripData* strip = NULL;
    //TObject* obj = NULL;
    while( (strip = (GblStripData*)iter()) != NULL) {
      oss << strip->toString() << "\n";
    }
    return oss.str();
  }
