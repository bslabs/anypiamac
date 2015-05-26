// Functions for the <see cref="PageOut"/> class - parent of classes to
// manage a page of printed output.

// $Id: PageOut.cpp 1.17 2011/08/19 15:57:44EDT 044579 Development  $

#include <fstream>
#include "PageOut.h"
#include "outfile.h"

using namespace std;

/// <summary>Initializes PageOut.</summary>
///
/// <remarks>Reserves <see cref="MAXLINES"/> entries for
/// <see cref="outputString"/>. Creates <see cref="punct"/> and
/// <see cref="loc"/> and imbues <see cref="strm"/> with the facet to allow for
/// inserting commas into formatted numbers for output.</remarks>
PageOut::PageOut() : outputString(), punct(new PunctFacet), strm(),
loc(strm.getloc(), punct)
{
  outputString.reserve(MAXLINES);
  strm.imbue(loc);
  strm.setf(ios::fixed,ios::floatfield);
}

/// <summary>Destructor.</summary>
PageOut::~PageOut()
{ }

/// <summary>Prints the output strings to the specified <see cref="Outfile"/>.
/// </summary>
///
/// <param name="outfile">Output file with page counters.</param>
void PageOut::print( Outfile& outfile ) const
{
  for (unsigned i1 = 0; i1 < outputString.size(); i1++) {
    outfile.printLine(outputString[i1]);
  }
}

/// <summary>Prints the output strings to the specified stream.</summary>
///
/// <param name="ostrm">Output stream.</param>
void PageOut::print( std::ostream& ostrm ) const
{
  for (unsigned i1 = 0; i1 < outputString.size(); i1++) {
    ostrm << outputString[i1] << endl;
  }
}
