// Functions for the <see cref="SgaBlindA"/>, <see cref="SgaBlindM"/>,
// <see cref="SgaMinA"/>, <see cref="SgaMinM"/>, <see cref="SgaMaxA"/>, and
// <see cref="SgaMaxM"/> classes (derived classes of the
// <see cref="SgaGeneral"/> class).
//
// $Id: SgaDerived.cpp 1.8 2011/08/09 15:55:29EDT 044579 Development  $

#include "SgaDerived.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>Initializes data. Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaBlindA::SgaBlindA( int newLastYear ) : SgaGeneral(newLastYear, 1994)
{
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 1200.0;
  theData[1966 - getBaseYear()] = 1500.0;
  theData[1967 - getBaseYear()] = 1500.0;
  theData[1968 - getBaseYear()] = 1590.0;
  for (int yr = 1969; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 1680.0;
  theData[1974 - getBaseYear()] = 2400.0;
  theData[1975 - getBaseYear()] = 2400.0;
  theData[1976 - getBaseYear()] = 2760.0;
  theData[1977 - getBaseYear()] = 2880.0;
  theData[1978 - getBaseYear()] = 4008.0;
  theData[1979 - getBaseYear()] = 4500.0;
  theData[1980 - getBaseYear()] = 5004.0;
  theData[1981 - getBaseYear()] = 5508.0;
  theData[1982 - getBaseYear()] = 6000.0;
  theData[1983 - getBaseYear()] = 6600.0;
  theData[1984 - getBaseYear()] = 6960.0;
  theData[1985 - getBaseYear()] = 7320.0;
  theData[1986 - getBaseYear()] = 7800.0;
  theData[1987 - getBaseYear()] = 8160.0;
  theData[1988 - getBaseYear()] = 8400.0;
  theData[1989 - getBaseYear()] = 8880.0;
  theData[1990 - getBaseYear()] = 9360.0;
  theData[1991 - getBaseYear()] = 9720.0;
  theData[1992 - getBaseYear()] = 10200.0;
  theData[1993 - getBaseYear()] = 10560.0;
  theData[1994 - getBaseYear()] = 11160.0;
  theData[1995 - getBaseYear()] = 11280.0;
  theData[1996 - getBaseYear()] = 11520.0;
  theData[1997 - getBaseYear()] = 12000.0;
  theData[1998 - getBaseYear()] = 12600.0;
  theData[1999 - getBaseYear()] = 13320.0;
  theData[2000 - getBaseYear()] = 14040.0;
}

/// <summary>Projects SGA amounts.</summary>
///
/// <remarks>Should not be used because projections are for monthly amount, but
/// this class deals with annual amounts.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SGAPROJ"/> (always).</exception>
void SgaBlindA::project( const DoubleAnnual& )
{
  throw PiaException(PIA_IDS_SGAPROJ);
}

/// <summary>Constructor. Initializes data.</summary>
///
/// <remarks>Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaBlindM::SgaBlindM( int newLastYear ) : SgaGeneral(newLastYear, 1994)
{
  // For years 1961 to 1967, there was actually a min and max value
  // (the same as for the non-blind). We'll just use the max.
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 100.0;
  theData[1966 - getBaseYear()] = 125.0;
  theData[1967 - getBaseYear()] = 125.0;
  for (int yr = 1968; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 140.0;
  theData[1974 - getBaseYear()] = 200.0;
  theData[1975 - getBaseYear()] = 200.0;
  theData[1976 - getBaseYear()] = 230.0;
  theData[1977 - getBaseYear()] = 240.0;
  theData[1978 - getBaseYear()] = 334.0;
  theData[1979 - getBaseYear()] = 375.0;
  theData[1980 - getBaseYear()] = 417.0;
  theData[1981 - getBaseYear()] = 459.0;
  theData[1982 - getBaseYear()] = 500.0;
  theData[1983 - getBaseYear()] = 550.0;
  theData[1984 - getBaseYear()] = 580.0;
  theData[1985 - getBaseYear()] = 610.0;
  theData[1986 - getBaseYear()] = 650.0;
  theData[1987 - getBaseYear()] = 680.0;
  theData[1988 - getBaseYear()] = 700.0;
  theData[1989 - getBaseYear()] = 740.0;
  theData[1990 - getBaseYear()] = 780.0;
  theData[1991 - getBaseYear()] = 810.0;
  theData[1992 - getBaseYear()] = 850.0;
  theData[1993 - getBaseYear()] = 880.0;
  theData[1994 - getBaseYear()] = 930.0;
  theData[1995 - getBaseYear()] = 940.0;
  theData[1996 - getBaseYear()] = 960.0;
  theData[1997 - getBaseYear()] = 1000.0;
  theData[1998 - getBaseYear()] = 1050.0;
  theData[1999 - getBaseYear()] = 1110.0;
  theData[2000 - getBaseYear()] = 1170.0;
}

/// <summary>Constructor. Initializes data.</summary>
///
/// <remarks>Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaMinA::SgaMinA( int newLastYear ) : SgaGeneral(newLastYear, 2000)
{
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 600.0;
  theData[1966 - getBaseYear()] = 900.0;
  theData[1967 - getBaseYear()] = 900.0;
  theData[1968 - getBaseYear()] = 990.0;
  for (int yr = 1969; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 1080.0;
  theData[1974 - getBaseYear()] = 1560.0;
  theData[1975 - getBaseYear()] = 1560.0;
  theData[1976 - getBaseYear()] = 1800.0;
  theData[1977 - getBaseYear()] = 1920.0;
  theData[1978 - getBaseYear()] = 2040.0;
  theData[1979 - getBaseYear()] = 2160.0;
  for (int yr = 1980; yr <= 1989; yr++)
    theData[yr - getBaseYear()] = 2280.0;
  for (int yr = 1990; yr <= 2000; yr++)
    theData[yr - getBaseYear()] = 3600.0;
  // If FIRST_PROJ_YEAR ever gets moved up later than 2001, then we'll
  // need to add some values here. Use the same values as for the max.
}

/// <summary>Projects SGA amounts.</summary>
///
/// <remarks>Should not be used because projections are for monthly amount, but
/// this class deals with annual amounts.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SGAPROJ"/> (always).</exception>
void SgaMinA::project( const DoubleAnnual& )
{
  throw PiaException(PIA_IDS_SGAPROJ);
}

/// <summary>Constructor. Initializes data.</summary>
///
/// <remarks>Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaMinM::SgaMinM( int newLastYear ) : SgaGeneral(newLastYear, 2000)
{
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 50.0;
  theData[1966 - getBaseYear()] = 75.0;
  theData[1967 - getBaseYear()] = 75.0;
  for (int yr = 1968; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 90.0;
  theData[1974 - getBaseYear()] = 130.0;
  theData[1975 - getBaseYear()] = 130.0;
  theData[1976 - getBaseYear()] = 150.0;
  theData[1977 - getBaseYear()] = 160.0;
  theData[1978 - getBaseYear()] = 170.0;
  theData[1979 - getBaseYear()] = 180.0;
  for (int yr = 1980; yr <= 1989; yr++)
    theData[yr - getBaseYear()] = 190.0;
  for (int yr = 1990; yr <= 2000; yr++)
    theData[yr - getBaseYear()] = 300.0;
  // If FIRST_PROJ_YEAR ever gets moved up later than 2001, then we'll
  // need to add some values here. Use the same values as for the max.
}

/// <summary>Projects SGA amounts.</summary>
///
/// <remarks>Starting in <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>, there is no
/// more min amount in use. So we'll set the min to be the same as the max for
/// the projections. In order to duplicate the max projected values, we need to
/// temporarily set the <see cref="SgaGeneral::FIRST_PROJ_YEAR"/> - 1 min
/// amount to be the same as the max amount for that year and then start the
/// projection. Afterwards we're done with the projection, reset it to its real
/// value.</remarks>
///
/// <param name="averageWages">Average wages to use for projection.</param>
void SgaMinM::project( const DoubleAnnual& averageWages )
{
  const int index = FIRST_PROJ_YEAR - 1 - getBaseYear();
  const double tempAmount = theData[index];
  theData[index] = 700.0;
  SgaGeneral::project(averageWages);
  theData[index] = tempAmount;
}

/// <summary>Constructor. Initializes data.</summary>
///
/// <remarks>Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaMaxA::SgaMaxA( int newLastYear ) : SgaGeneral(newLastYear, 2000)
{
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 1200.0;
  theData[1966 - getBaseYear()] = 1500.0;
  theData[1967 - getBaseYear()] = 1500.0;
  theData[1968 - getBaseYear()] = 1590.0;
  for (int yr = 1969; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 1680.0;
  theData[1974 - getBaseYear()] = 2400.0;
  theData[1975 - getBaseYear()] = 2400.0;
  theData[1976 - getBaseYear()] = 2760.0;
  theData[1977 - getBaseYear()] = 2880.0;
  theData[1978 - getBaseYear()] = 3120.0;
  theData[1979 - getBaseYear()] = 3360.0;
  for (int yr = 1980; yr <= 1989; yr++)
    theData[yr - getBaseYear()] = 3600.0;
  for (int yr = 1990; yr <= 1998; yr++)
    theData[yr - getBaseYear()] = 6000.0;
  theData[1999 - getBaseYear()] = 7200.0;
  theData[2000 - getBaseYear()] = 8400.0;
}

/// <summary>Projects SGA amounts.</summary>
///
/// <remarks>Should not be used because projections are for monthly amount, but
/// this class deals with annual amounts.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_SGAPROJ"/> (always).</exception>
void SgaMaxA::project( const DoubleAnnual& )
{
  throw PiaException(PIA_IDS_SGAPROJ);
}

/// <summary>Constructor. Initializes data.</summary>
///
/// <remarks>Set all years up to (but not including)
/// <see cref="SgaGeneral::FIRST_PROJ_YEAR"/>.</remarks>
///
/// <param name="newLastYear">New last year.</param>
SgaMaxM::SgaMaxM( int newLastYear ) : SgaGeneral(newLastYear, 2000)
{
  for (int yr = 1961; yr <= 1965; yr++)
    theData[yr - getBaseYear()] = 100.0;
  theData[1966 - getBaseYear()] = 125.0;
  theData[1967 - getBaseYear()] = 125.0;
  for (int yr = 1968; yr <= 1973; yr++)
    theData[yr - getBaseYear()] = 140.0;
  theData[1974 - getBaseYear()] = 200.0;
  theData[1975 - getBaseYear()] = 200.0;
  theData[1976 - getBaseYear()] = 230.0;
  theData[1977 - getBaseYear()] = 240.0;
  theData[1978 - getBaseYear()] = 260.0;
  theData[1979 - getBaseYear()] = 280.0;
  for (int yr = 1980; yr <= 1989; yr++)
    theData[yr - getBaseYear()] = 300.0;
  for (int yr = 1990; yr <= 1998; yr++)
    theData[yr - getBaseYear()] = 500.0;
  theData[1999 - getBaseYear()] = 700.0;
  theData[2000 - getBaseYear()] = 700.0;
}
