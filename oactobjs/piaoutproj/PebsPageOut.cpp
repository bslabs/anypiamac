// Functions for the <see cref="PebsPageOut"/> class - parent of classes
// to write out results of Social Security Statement benefit calculation.

// $Id: PebsPageOut.cpp 1.9.1.3 2012/09/25 13:55:19EDT 277133 Development  $
//
#include "PebsPageOut.h"
#include "wrkrdata.h"

using namespace std;

const string PebsPageOut::equals = string(72, '=');
const string PebsPageOut::hyphens = string(72, '-');

/// <summary>Initializes this instance.</summary>
PebsPageOut::PebsPageOut() : PageOut(), thePage(0)
{ }

/// <summary>Destructor.</summary>
PebsPageOut::~PebsPageOut()
{ }

/// <summary>Prepares header string for output.</summary>
///
/// <param name="workerData">Worker data.</param>
void PebsPageOut::prepareHeader( const WorkerData& workerData )
{
  //strm.str("");
  //strm << workerData.getNhname() << "  " << workerData.ssn.toString();
  //const string::size_type slength = strm.str().size();
  //strm << string(66 - slength, ' ') << "Page " << thePage;
  //outputString.push_back(strm.str());
  //outputString.push_back(equals);
}
