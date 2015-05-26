// Functions for the <see cref="PiaDataArray"/> class to manage an array
// of <see cref="PiaData"/>'s, representing one family.
//
// $Id: PiadataArray.cpp 1.8 2011/08/01 16:22:23EDT 044579 Development  $

#include "PiadataArray.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Initializes arrays used in PiadataArray.</summary>
///
/// <remarks>Allocates memory for the <see cref="PiaData"/>'s.</remarks>
PiaDataArray::PiaDataArray() : famSize(0)
{
  // allocate memory for pia data
  for (int i = 0; i < MAXFAMSIZE; i++)
    piaData[i] = new PiaData();
}

/// <summary>Releases memory.</summary>
///
/// <remarks>Deletes memory for the <see cref="PiaData"/>'s.</remarks>
PiaDataArray::~PiaDataArray()
{
  // release memory for pia data
  for (int i = 0; i < MAXFAMSIZE; i++)
    delete piaData[i];
}

/// <summary>Resets all values to zero.</summary>
///
/// <remarks>Calls <see cref="PiaData::deleteContents"/> for each item in the
/// array.</remarks>
void PiaDataArray::deleteContents()
{
  // reset all benefit data
  for (int i = 0; i < MAXFAMSIZE; i++)
    piaData[i]->deleteContents();
  famSize = 0;
}

/// <summary>Sets family size (not including worker).</summary>
///
/// <param name="newFamSize">Family size to use.</param>
void PiaDataArray::setFamSize( int newFamSize )
{
  famSize = newFamSize;
}

/// <summary>Checks if family size is ok.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FAMSIZE"/> if family size is out of
/// bounds.</exception>
///
/// <param name="newFamSize">Family size to check.</param>
void PiaDataArray::famSizeCheck( int newFamSize )
{
  if (newFamSize < 0 || newFamSize > MAXFAMSIZE)
    throw PiaException(PIA_IDS_FAMSIZE);
}
