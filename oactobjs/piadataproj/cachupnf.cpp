// Functions for the <see cref="CatchupNonFile"/> class to manage a set of
// catch-up benefit increases stored in arrays.
//
// $Id: cachupnf.cpp 1.17 2011/07/29 13:49:25EDT 044579 Development  $

#include "CatchupNonFile.h"
#include "AssumptionType.h"

/// <summary>Initializes catch-up benefit increases.</summary>
///
/// <param name="newCstart">Starting year of catch-up benefit increases.
/// </param>
CatchupNonFile::CatchupNonFile( int newCstart ) : CatchupDoc(newCstart)
{
  for (int i = 0; i < 4; i++) {
    cachuptr[i].setCstart(newCstart);
    read(i+1);
  }
}

/// <summary>Destructor.</summary>
CatchupNonFile::~CatchupNonFile()
{ }

/// <summary>Read stored values from memory.</summary>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void CatchupNonFile::read( int altNum )
{
  cachuptr[altNum - 1].deleteContents();
  allownonzero[altNum - 1] = false;
}

/// <summary>Dummy function.</summary>
void CatchupNonFile::write( int )
{ }

/// <summary>Sets catch-up increases with specified alternative, using the
/// data stored for the specified alternative.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void CatchupNonFile::setData( int altNum )
{
  if (AssumptionType::isTR(altNum)) {
    catchup = cachuptr[altNum - 1];
  }
  else {
    catchup.deleteContents();
  }
}

/// <summary>Sets catch-up increases, storing the data for the specified
/// alternative from the <see cref="Catchup"/>, then setting the current data
/// to that data.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
/// <param name="newCatchup">New catch-up benefit increases.</param>
void CatchupNonFile::setData( int altNum, const Catchup& newCatchup )
{
  if (AssumptionType::isTR(altNum)) {
    cachuptr[altNum - 1] = newCatchup;
  }
  setData(altNum);
}
