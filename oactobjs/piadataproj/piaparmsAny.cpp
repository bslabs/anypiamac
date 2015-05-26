// Functions for the <see cref="PiaParamsAny"/> class to manage the Social
// Security parameters required to calculate Social Security benefits in the
// Anypia program.
//
// $Id: piaparmsAny.cpp 1.12 2011/08/05 16:30:21EDT 044579 Development  $

#include <math.h>
#include "piaparmsAny.h"
#include "oactcnst.h"

/// <summary>Initializes arrays.</summary>
///
/// <param name="newIstart">Year after last known benefit increase.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="awbiData">Structure with historical amounts.</param>
/// <param name="newLawChange">Law change parameters.</param>
PiaParamsAny::PiaParamsAny( int newIstart, int newMaxyear,
AwbiData& awbiData, const LawChangeArray& newLawChange ) :
PiaParamsLC(newIstart, newMaxyear, awbiData, newLawChange),
wageHigh(YEAR37, newMaxyear), wageMin(YEAR37, newMaxyear)
{ }

/// <summary>Destroys arrays.</summary>
PiaParamsAny::~PiaParamsAny( )
{ }

/// <summary>Projects low earnings.</summary>
void PiaParamsAny::minProj()
{
  for (int yr = YEAR37; yr <= maxyear; yr++)
    wageMin[yr] = 0.45 * fq[yr];
}

/// <summary>Projects high earnings.</summary>
void PiaParamsAny::highProj()
{
  for (int yr = YEAR37; yr <= maxyear; yr++)
    wageHigh[yr] = 1.60 * fq[yr];
}

/// <summary>Projects average annual wage and amounts dependent on it.</summary>
void PiaParamsAny::projectFq()
{
  PiaParamsLC::projectFq();
  minProj();
  highProj();
}
