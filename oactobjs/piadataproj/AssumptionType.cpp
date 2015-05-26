// Functions of the <see cref="AssumptionType"/> class containing the
// assumption types allowed in the piadata library.
//
// $Id: AssumptionType.cpp 1.7 2011/07/28 08:55:10EDT 044579 Development  $

#include "AssumptionType.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

/// <summary>Checks average wage increase indicator in general case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTAW1"/> if average wage increase
/// indicator is out of range in general case.</exception>
///
/// <param name="ialtawt">Assumption type to check.</param>
void AssumptionType::ialtawCheck1( int ialtawt )
{
  if (ialtawt < static_cast<int>(ALTERN_I) ||
    ialtawt > static_cast<int>(OTHER_ASSUM)) {
    throw PiaException(PIA_IDS_IALTAW1);
  }
}

/// <summary>Checks average wage increase indicator in Statement case.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTAW2"/> if average wage
/// increase indicator is out of range in Statement case.</exception>
///
/// <param name="ialtawt">Assumption type to check.</param>
void AssumptionType::ialtawCheck2( int ialtawt )
{
  if (ialtawt != static_cast<int>(FLAT) &&
    ialtawt != static_cast<int>(PEBS_ASSUM)) {
    throw PiaException(PIA_IDS_IALTAW2);
  }
}

/// <summary>Checks benefit increase indicator in general case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTBI1"/> if benefit increase
/// indicator is out of range in general case.</exception>
///
/// <param name="ialtbit">Assumption type to check.</param>
void AssumptionType::ialtbiCheck1( int ialtbit )
{
  if (ialtbit < static_cast<int>(ALTERN_I) ||
    ialtbit > static_cast<int>(OTHER_ASSUM)) {
    throw PiaException(PIA_IDS_IALTBI1);
  }
}

/// <summary>Checks benefit increase indicator in Statement case.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTBI2"/> if benefit increase
/// indicator is out of range in Statement case.</exception>
///
/// <param name="ialtbit">Assumption type to check.</param>
void AssumptionType::ialtbiCheck2( int ialtbit )
{
  if (ialtbit != static_cast<int>(FLAT) &&
    ialtbit != static_cast<int>(PEBS_ASSUM)) {
    throw PiaException(PIA_IDS_IALTBI2);
  }
}
