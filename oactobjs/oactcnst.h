// Constants used in the various Oact C++ programs and utilities.
//
// $Id: oactcnst.h 1.17 2012/03/12 12:48:25EDT 277133 Development  $

#pragma once

// <summary>Number of years of data in trust fund projections, January 1
//   basis.</summary>
//
// <remarks>When using 52, data runs from January 1, 1975 through January 1,
//   2026.</remarks>
const int MAXYRSR = 52;

// <summary>Number of years of data in trust fund projections, during-year
//   basis.</summary>
//
// <remarks>When using 51, data runs from 1975 through 2025.</remarks>
const int MAXYRSR1 = 51;

// <summary>Base year for trust fund projection arrays on January 1 basis.
// </summary>
const int BASEYR = 1974;

// <summary>Last year of short-range period.</summary>
const int LASTYRSR = BASEYR + MAXYRSR1;

// <summary>Base year for trust fund projection arrays on during-year basis.
// </summary>
const int BASEYR1 = 1975;

// <summary>Base year for ORS trust fund projection data.</summary>
const int ORSYR = 1981;

// <summary>Year before first year of Social Security.</summary>
const int YEAR36 = 1936;

// <summary>Base year for many data arrays.</summary>
const int YEAR37 = 1937;

// <summary>Year before first year of major revisions to Social Security.
// </summary>
const int YEAR50 = 1950;

// <summary>First year of major revisions to Social Security.</summary>
const int YEAR51 = 1951;

// <summary>First year of many OACT projection arrays.</summary>
const int YEAR69 = 1969;

// <summary>First year of many OACT projection arrays.</summary>
const int YEAR70 = 1970;

// <summary>First year of many OACT projection arrays.</summary>
const int YEAR74 = 1974;

// <summary>First year of many OACT projection arrays.</summary>
const int YEAR75 = 1975;

// <summary>First year of bend points.</summary>
const int YEAR79 = 1979;

// <summary>Last possible year of projection.</summary>
const int YEAR2075 = 2075;

// <summary>Last possible year of projection.</summary>
const int YEAR2080 = 2080;

// <summary>Last possible year of projection.</summary>
const int YEAR2085 = 2085;

// <summary>Last possible year of projection.</summary>
const int YEAR2090 = 2090;

// <summary>Last possible year of projection.</summary>
const int YEAR2100 = 2100;

// <summary>Maximum annual earnings allowed.</summary>
const double MAXEARN = 9999999.99;

// <summary>Maximum number of years of earnings for one individual.</summary>
const int MAXYRS = 80;
