// Functions for the <see cref="QcPacked"/> class to hold 8 quarters of
// coverage amounts in one word.

// $Id: QcPacked.cpp 1.13 2011/08/09 14:59:55EDT 044579 Development  $

#include "QcPacked.h"
#include "PiaException.h"
#include "Resource.h"

/// <summary>Return a quarter of coverage amount.</summary>
///
/// <returns>A quarter of coverage amount.</returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCPACKED1"/> if index is out of range
/// (only in debug mode).</exception>
///
/// <param name="index">Index of quarter of coverage amount (0 to
/// <see cref="QcPacked::NUM_QCS"/> - 1).</param>
unsigned QcPacked::get( int index ) const
{
#ifndef NDEBUG
  if (index < 0 || index >= NUM_QCS) {
    throw PiaException(PIA_IDS_QCPACKED1);
  }
#endif
  switch (index) {
  case 0: return qc0_1 & 0xf;
  case 1: return qc0_1 >> 4;
  case 2: return qc2_3 & 0xf;
  case 3: return qc2_3 >> 4;
  case 4: return qc4_5 & 0xf;
  case 5: return qc4_5 >> 4;
  case 6: return qc6_7 & 0xf;
  case 7: return qc6_7 >> 4;
  default: return 0;
  }
}

/// <summary>Set a quarter of coverage amount.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_QCPACKED2"/> if index is out of range
/// (only in debug mode).</exception>
///
/// <param name="index">Index of quarter of coverage amount (0 to
/// <see cref="QcPacked::NUM_QCS"/> - 1).</param>
/// <param name="value">New value to use.</param>
void QcPacked::set( int index, unsigned value )
{
#ifndef NDEBUG
  if (index < 0 || index >= NUM_QCS) {
    throw PiaException(PIA_IDS_QCPACKED2);
  }
#endif
  switch (index) {
  case 0: qc0_1 = (unsigned char)((qc0_1 & 0xf0) + value); return;
  case 1: qc0_1 = (unsigned char)((value << 4) + (qc0_1 & 0xf)); return;
  case 2: qc2_3 = (unsigned char)((qc2_3 & 0xf0) + value); return;
  case 3: qc2_3 = (unsigned char)((value << 4) + (qc2_3 & 0xf)); return;
  case 4: qc4_5 = (unsigned char)((qc4_5 & 0xf0) + value); return;
  case 5: qc4_5 = (unsigned char)((value << 4) + (qc4_5 & 0xf)); return;
  case 6: qc6_7 = (unsigned char)((qc6_7 & 0xf0) + value); return;
  case 7: qc6_7 = (unsigned char)((value << 4) + (qc6_7 & 0xf)); return;
  default: return;
  }
}
