// Functions for the <see cref="PageCounters"/> class to manage page
// counters.

// $Id: page.cpp 1.20 2011/08/19 15:57:44EDT 044579 Development  $

#include <fstream>
#include "page.h"
#include "Resource.h"
#include "PiaException.h"
#include "Trace.h"

using namespace std;

/// <summary>Initializes page file name and array size.</summary>
///
/// <param name="maxCounters">Maximum number of page counters.</param>
PageCounters::PageCounters( int maxCounters ) :
Document("page", ""), theData(maxCounters)
{ }

/// <summary>Zeroes out page counters.</summary>
void PageCounters::deleteContents()
{
  fill(theData.begin(), theData.end(), 0);
}

/// <summary>Reads page counters from a file named "page".</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PAGE1"/> if unable to open input file; of type
/// <see cref="PIA_IDS_PAGE2"/> if unable to read all values successfully.
/// </exception>
void PageCounters::read()
{
  int temp;  // temporary integer
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_PAGE1);
  Trace::writeLine("Reading page counters");
  for (unsigned i = 0; i < theData.size(); i++) {
    if (!(in >> temp))
      throw PiaException(PIA_IDS_PAGE2);
    theData[i] = temp;
  }
  in.close();
}

/// <summary>Updates all subsequent page counters and writes them out.
/// </summary>
///
/// <param name="slot">Slot after which all page counters are updated.</param>
void PageCounters::update( int slot )
{
  const int temp = theData[slot];
  fill(theData.begin() + slot + 1, theData.end(), temp);
  write();
}

/// <summary>Writes page counters out to a file named "page".</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PAGE3"/> if unable to open output file.
/// </exception>
void PageCounters::write()
{
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_PAGE3);
  out.setf(ios::fixed, ios::floatfield);
  Trace::writeLine("Writing page counters");
  for (unsigned i = 0; i < theData.size(); i++)
    out << " " << theData[i];
  out << endl;
  out.close();
}
