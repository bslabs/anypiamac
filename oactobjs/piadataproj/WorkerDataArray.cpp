// Functions for the <see cref="WorkerDataArray"/> class to manage array
// of <see cref="WorkerData"/>'s.

// $Id: WorkerDataArray.cpp 1.9 2011/08/11 13:55:33EDT 044579 Development  $

#include "WorkerDataArray.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Initializes arrays used in WorkerDataArray.</summary>
///
/// <remarks>Allocates memory for the <see cref="WorkerData"/>'s.</remarks>
WorkerDataArray::WorkerDataArray() : famSize(0)
{
  // allocate memory for worker data
  for (int i = 0; i < MAXFAMSIZE; i++)
    workerData[i] = new WorkerData();
}

/// <summary>Releases memory.</summary>
///
/// <remarks>Deletes memory for the <see cref="WorkerData"/>'s.</remarks>
WorkerDataArray::~WorkerDataArray()
{
  // release memory for worker data
  for (int i = 0; i < MAXFAMSIZE; i++)
    delete workerData[i];
}

/// <summary>Resets all values to zero.</summary>
///
/// <remarks>Calls <see cref="WorkerData::deleteContents"/> for each item in
/// the array.</remarks>
void WorkerDataArray::deleteContents()
{
  // reset all worker data
  for (int i = 0; i < MAXFAMSIZE; i++)
    workerData[i]->deleteContents();
  famSize = 0;
}

/// <summary>Sets family size (not including worker).</summary>
///
/// <param name="newFamSize">Family size to use.</param>
void WorkerDataArray::setFamSize( int newFamSize )
{
  famSize = newFamSize;
}

/// <summary>Checks if family size is ok.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FAMSIZE"/> if out of bounds.</exception>
///
/// <param name="newFamSize">Family size to check.</param>
void WorkerDataArray::famSizeCheck( int newFamSize )
{
  if (newFamSize < 0 || newFamSize > MAXFAMSIZE)
    throw PiaException(PIA_IDS_FAMSIZE);
}
