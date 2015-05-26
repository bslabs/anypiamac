// Functions for the <see cref="LawChangeOut"/> class to manage the output
// file of law change descriptions.
//
// $Id: LawChangeOut.cpp 1.38 2011/08/01 10:06:07EDT 044579 Development  $

#include "LawChangeOut.h"
#include "LawChangeArray.h"

using namespace std;

/// <summary>Initializes data for output.</summary>
///
/// <param name="newLawChange">Law change data.</param>
LawChangeOut::LawChangeOut( const LawChangeArray& newLawChange ) :
PageOut(), lawChange(newLawChange)
{ }

/// <summary>Releases memory.</summary>
LawChangeOut::~LawChangeOut()
{ }

/// <summary>Prepares output strings for changes from present law.</summary>
///
/// <remarks>Calls <see cref="LawChange::prepareStrings"/> for each change in
/// law.</remarks>
void LawChangeOut::prepareStrings()
{
  string s;  // temporary string
  outputString.clear();
  outputString.push_back("Changes from present law:");
  if (lawChange.getIndTotal() == 0) {
    outputString.push_back("None");
    return;
  }
  for (int i1 = 0; i1 < LawChange::MAXLCH; i1++) {
    lawChange.lawChange[i1]->prepareStrings(outputString);
  }
}
