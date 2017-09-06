// Declarations for the <see cref="BaseYearNonFile"/> class to manage the
// computation year stored internally.
//
// $Id: BaseYearNonFile.h 1.40 2017/08/11 07:00:00EDT 277133 Development  $

#pragma once

#if !defined(SHORTFILENAMES)
#include "baseyear.h"
#else
#include "baseyrh"
#endif

/// <summary>Manages the computation year for a pia program where it is
///   stored with the code.</summary>
///
/// <remarks>Currently <see cref="YEAR"/> is 2017 and <see cref="TR_YEAR"/>
/// is 2017. See <see cref="BaseYearFile"/> for a class where the computation
/// year is stored on disk.</remarks>
///
/// <seealso cref="BaseYearFile"/>
class BaseYearNonFile : public BaseYear
{
public:
  /// <summary>Year for which data has been stored.</summary>
  ///
  /// <remarks>The value of 2017 means that the December 2016 benefit increase,
  /// 2015 average wage, and 2017 wage base are available. This is updated in
  /// October after the annual automatic adjustment announcement.</remarks>
  static const int YEAR = 2017;
  /// <summary>Year of Trustees Report assumptions.</summary>
  ///
  /// <remarks>This is updated when the TR is released, so it is equal to
  /// <see cref="YEAR"/> from the TR date to October, and 1 less than
  /// <see cref="YEAR"/> from October to the TR date.</remarks>
  static const int TR_YEAR = 2017;
public:
  BaseYearNonFile();
  void read();
  void write();
};
