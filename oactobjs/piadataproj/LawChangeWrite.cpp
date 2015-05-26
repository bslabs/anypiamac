// Functions for the <see cref="LawChangeWrite"/> class to write law
// changes to a file.

// $Id: LawChangeWrite.cpp 1.43 2011/08/01 11:38:36EDT 044579 Development  $

#include <fstream>
#include <iomanip>
#include "LawChangeWrite.h"
#include "Resource.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "LawChangeArray.h"

using namespace std;

/// <summary>Initializes references to data.</summary>
///
/// <param name="newLawChange">Law-change indicators to print out.</param>
LawChangeWrite::LawChangeWrite( LawChangeArray& newLawChange ) :
lawChange(newLawChange)
{ }

/// <summary>Writes indicators for change from present law.</summary>
///
/// <remarks>Opens an output file and passes it to the version with a stream
/// argument.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG8"/> if unable to open output file.</exception>
///
/// <param name="out">Open output stream.</param>
void LawChangeWrite::write()
{
  ofstream out(lawChange.getPathname().c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_LAWCHG8);
  write(out);
  out.close();
}

/// <summary>Writes indicators for change from present law to an open stream.
/// </summary>
///
/// <param name="out">Open output stream.</param>
void LawChangeWrite::write( std::ostream& out )
{
  // write line with indicators
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    out << setw(3) << lawChange.lawChange[i1]->getInd();
  }
  out << endl;
  // write details for each law-change
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    lawChange.lawChange[i1]->write(out);
  }
}
