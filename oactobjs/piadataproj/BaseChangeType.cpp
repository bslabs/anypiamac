// Functions of the <see cref="BaseChangeType"/> class with the wage base
// change types allowed in the piadata library.
//
// $Id: BaseChangeType.cpp 1.7 2011/07/28 11:50:14EDT 044579 Development  $

#include "BaseChangeType.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

// <summary>Wage base titles.</summary>
const std::string BaseChangeType::mfwgbase[NUM_CHANGE_TYPES] = {
  string("none"),
  string("automatic provisions followed"),
  string("ad hoc wage bases (entered by user)")
};

/// <summary>Checks wage base indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASECH"/> if wage base indicator is not
/// <see cref="change_type::AUTOMATIC"/> or <see cref="change_type::ADHOC"/>.
/// </exception>
///
/// <param name="ibascht">Value to check.</param>
void BaseChangeType::check( int ibascht )
{
  if (ibascht != AUTOMATIC && ibascht != ADHOC)
    throw PiaException(PIA_IDS_BASECH);
}
