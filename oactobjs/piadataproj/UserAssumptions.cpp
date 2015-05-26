// Functions for the <see cref="UserAssumptions"/> class to manage
// user-specified assumptions.
//
// $Id: UserAssumptions.cpp 1.21 2011/08/11 08:46:58EDT 044579 Development  $

#include <locale> // for islower
#include "UserAssumptions.h"
#include "wbgenrl.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"

using namespace std;

// Define static class members
int UserAssumptions::istart;
int UserAssumptions::maxyear;

/// <summary>Initializes user-specified assumptions.</summary>
///
/// <param name="newLastYear">Maximum projected year.</param>
UserAssumptions::UserAssumptions( int newLastYear ) :
awincproj(YEAR37, newLastYear), baseOasdi(YEAR37, newLastYear),
base77(YEAR37, newLastYear), biproj(YEAR51, newLastYear), catchup(),
titleAw(), titleBi(), ialtbi(AssumptionType::FLAT), anscch('N'),
ialtaw(AssumptionType::FLAT), ibasch(BaseChangeType::AUTOMATIC),
jaltbi(AssumptionType::NONE), jaltaw(AssumptionType::NONE),
jbasch(BaseChangeType::NONE)
{
  maxyear = newLastYear;
}

/// <summary>Initializes all variables.</summary>
void UserAssumptions::deleteContents()
{
  zeroJalt();
  ialtbi = ialtaw = AssumptionType::FLAT;
  ibasch = BaseChangeType::AUTOMATIC;
  anscch = 'N';
  titleAw = "";
  titleBi = "";
  catchup.deleteContents();
  baseOasdi.fill(0.0);
  base77.fill(0.0);
  biproj.fill(0.0);
  awincproj.fill(0.0);
}

/// <summary>Sets last year of data arrays. Should be called when benefit
/// date is set in <see cref="WorkerData"/>.</summary>
///
/// <param name="year">Year to use.</param>
void UserAssumptions::setLastYear( int year )
{
  lastYear = year;
}

/// <summary>Checks for valid assumption for Statement.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_PEBSASM"/> if benefit increase indicator or average
/// wage increase indicator is out of range in Statement case.</exception>
void UserAssumptions::pebsasmCheck() const
{
  // AssumptionType::EXPANDED_PEBES is constant, so ignore warning
#pragma warning(disable:4127)
  try {
    if (AssumptionType::EXPANDED_PEBES) {
      AssumptionType::ialtbiCheck1(ialtbi);
      AssumptionType::ialtawCheck1(ialtaw);
    }
    else {
      AssumptionType::ialtbiCheck2(ialtbi);
      AssumptionType::ialtawCheck2(ialtaw);
    }
  } catch (PiaException&) {
    throw PiaException(PIA_IDS_PEBSASM);
  }
#pragma warning(default:4127)
}

/// <summary>Sets average wage increase indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTAW1"/> if average wage increase
/// indicator is out of range in general case.</exception>
///
/// <param name="ialtawt">Value to use.</param>
void UserAssumptions::setIaltaw( int ialtawt )
{
  AssumptionType::ialtawCheck1(ialtawt);
  ialtaw = static_cast<AssumptionType::assum_type>(ialtawt);
}

/// <summary>Sets benefit increase indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_IALTBI1"/> if benefit increase
/// indicator is out of range in general case.</exception>
///
/// <param name="ialtbit">Value to use.</param>
void UserAssumptions::setIaltbi( int ialtbit )
{
  AssumptionType::ialtbiCheck1(ialtbit);
  ialtbi = static_cast<AssumptionType::assum_type>(ialtbit);
}

/// <summary>Sets current year.</summary>
///
/// <param name="istartt">Value to use.</param>
void UserAssumptions::setIstart( int istartt )
{
  try {
    istartCheck(istartt);
    istart = istartt;
    catchup.setCstart(istartt);
  } catch (PiaException&) {
    // do not do anything yet; eventually set an indicator saying
    // assumptions are not allowed
  }
}

/// <summary>Sets catch-up increase indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ANSCCH"/> if catch-up increase
/// indicator is out of range.</exception>
///
/// <param name="ansccht">Value to use.</param>
void UserAssumptions::setAnscch( char ansccht )
{
  anscchCheck(ansccht);
  if (islower(ansccht)) {
    ansccht = (char)toupper(ansccht);
  }
  anscch = ansccht;
}

/// <summary>Sets wage base indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASECH"/> if wage base indicator is not
/// <see cref="BaseChangeType::change_type::AUTOMATIC"/> or
/// <see cref="BaseChangeType::change_type::ADHOC"/>.</exception>
///
/// <param name="ibascht">Value to use.</param>
void UserAssumptions::setIbasch( int ibascht )
{
  BaseChangeType::check(ibascht);
  ibasch = static_cast<BaseChangeType::change_type>(ibascht);
}

/// <summary>Checks catch-up increase indicator.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_ANSCCH"/> if catch-up increase
/// indicator is out of range.</exception>
///
/// <param name="ansccht">Value to check.</param>
void UserAssumptions::anscchCheck( char ansccht )
{
  if (ansccht != 'Y' && ansccht != 'N' && ansccht != 'y' && ansccht != 'n')
    throw PiaException(PIA_IDS_ANSCCH);
}

/// <summary>Checks current year.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_START1"/> if starting year is before 1979; of type
/// <see cref="PIA_IDS_START2"/> if starting year is after maximum year
/// allowed.</exception>
///
/// <param name="istartt">Value to check.</param>
void UserAssumptions::istartCheck( int istartt )
{
  if (istartt < YEAR79)
    throw PiaException(PIA_IDS_START1);
  if (istartt > maxyear)
    throw PiaException(PIA_IDS_START2);
}

/// <summary>Resets assumptions triggers.</summary>
///
/// <remarks> Should be called after updating assumptions for user-specified
///  assumptions.</remarks>
void UserAssumptions::resetTriggers()
{
  setJaltbi(getIaltbi());
  setJaltaw(getIaltaw());
  setJbasch(getIbasch());
}
