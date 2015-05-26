// Functions of the <see cref="AwbiData"/> class to manage historical Social
// Security parameters.
//
// $Id: awbidat.cpp 1.20 2011/07/28 10:16:26EDT 044579 Development  $

#include "awbidat.h"
#include "oactcnst.h"
#include "BaseYearNonFile.h"

using namespace std;

/// <summary>Initializes historical parameters info, where the last year of
/// historical wage bases is specified.</summary>
///
/// <param name="newLastYear">Last year of wage bases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
AwbiData::AwbiData( int newLastYear, int newMaxyear ) :
cpiinc(YEAR51, newMaxyear), fq(YEAR37, newMaxyear),
baseOasdi(YEAR37, newMaxyear), base77(YEAR37, newMaxyear),
baseHi(YEAR37, newMaxyear), lastYear(newLastYear), maxyear(newMaxyear)
{ }

/// <summary>Initializes historical parameters info, where the last year of
/// historical wage bases is set to <see cref="BaseYearNonFile::YEAR"/>.
/// </summary>
///
/// <param name="newMaxyear">Maximum projected year.</param>
AwbiData::AwbiData( int newMaxyear ) :
cpiinc(YEAR51, newMaxyear), fq(YEAR37, newMaxyear),
baseOasdi(YEAR37, newMaxyear), base77(YEAR37, newMaxyear),
baseHi(YEAR37, newMaxyear), lastYear(BaseYearNonFile::YEAR),
maxyear(newMaxyear)
{ }

/// <summary>Destructor.</summary>
AwbiData::~AwbiData()
{ }

/// <summary>Dummy function (descendant class may use it).</summary>
void AwbiData::read()
{ }

/// <summary>Dummy function (descendant class may use it).</summary>
void AwbiData::write()
{ }
