// Declarations for the <see cref="BaseYearNonFile"/> class to manage the
// computation year stored internally.
//
// $Id: BaseYearNonFile.h 1.48 2020/11/16 09:21:27EST 277133 Development  $

#pragma once

#if !defined(SHORTFILENAMES)
#include "baseyear.h"
#else
#include "baseyrh"
#endif

/// <summary>Manages the computation year for a pia program where it is
///   stored with the code.</summary>
///
/// <remarks>Currently <see cref="YEAR"/> is 2021 and <see cref="TR_YEAR"/>
/// is 2020. See <see cref="BaseYearFile"/> for a class where the computation
/// year is stored on disk.</remarks>
///
/// <seealso cref="BaseYearFile"/>
class BaseYearNonFile : public BaseYear
{
public:
  /// <summary>Year for which data has been stored.</summary>
  ///
  /// <remarks>The value of 2021 means that the December 2020 benefit increase,
  /// 2019 average wage, and 2021 wage base are available. This is updated in
  /// October after the annual automatic adjustment announcement.</remarks>
  static const int YEAR = 2021;
  /// <summary>Year of Trustees Report assumptions.</summary>
  ///
  /// <remarks>This is updated when the TR is released, so it is equal to
  /// <see cref="YEAR"/> from the TR date to the COLA update, and 1 less than
  /// <see cref="YEAR"/> from the COLA update to the TR date.</remarks>
  static const int TR_YEAR = 2020;
public:
  BaseYearNonFile();
  void read();
  void write();
};
