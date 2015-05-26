// Functions for the <see cref="LawChangeRead"/> class to read law
// changes from a file.

// $Id: LawChangeRead.cpp 1.45 2011/08/01 10:06:07EDT 044579 Development  $

#include <fstream>
#include "LawChangeRead.h"
#include "Resource.h"
#include "oactcnst.h"
#include "StringParser.h"
#include "PiaException.h"
#include "LawChangeArray.h"

using namespace std;

/// <summary>Initializes law-change reference.</summary>
///
/// <param name="newLawChange">Law-change indicators.</param>
LawChangeRead::LawChangeRead ( LawChangeArray& newLawChange ) :
lawChange(newLawChange)
{ }

/// <summary>Reads indicators for change from present law.</summary>
///
/// <remarks>Opens an input file and passes it to the version with a stream
/// argument.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG7"/> if unable to open input file; of type
/// <see cref="PIA_IDS_LAWCHG6"/> if unable to parse input line.</exception>
void LawChangeRead::read()
{
  ifstream in(lawChange.getPathname().c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_LAWCHG7);
  read(in);
  in.close();
}

/// <summary>Reads indicators for change from present law from an open stream.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_LAWCHG6"/> if unable to parse input line.</exception>
///
/// <param name="infile">Open input stream.</param>
void LawChangeRead::read( std::istream& infile )
{
  string mksstr; //input string for MKS Id
  getline(infile, mksstr);
  if (infile.eof() || infile.fail())
    throw PiaException("Unable to read MKS Id line");

  string str;  // input string
  // read line with indicators
  getline(infile, str);
  vector< string > strvec = parseString(str);
  if (strvec.size() < LawChange::MAXLCH)
    throw PiaException(PIA_IDS_LAWCHG6);
  // parse the indicators
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    const int i3 = atoi(strvec[i1].c_str());
    if (i3 < 0)
      throw PiaException(PIA_IDS_LAWCHG6);
    lawChange.lawChange[i1]->setInd(i3);
  }
  lawChange.indTotalCal();
  // read additional parameters
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    lawChange.lawChange[i1]->read(infile);
  }
}
