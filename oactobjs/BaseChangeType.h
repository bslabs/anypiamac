// Declaration of the <see cref="BaseChangeType"/> class with the wage base
// change types allowed in the piadata library.
//
// $Id: BaseChangeType.h 1.8 2011/07/28 11:50:13EDT 044579 Development  $

#pragma once

#include <string>

/// <summary>The wage base change types allowed in the piadata library.
/// </summary>
///
/// <remarks>There is an enum and some static functions, but no member
/// variables, so there are no instances of this class.</remarks>
class BaseChangeType
{
public:
  /// <summary>Type of wage base changes.</summary>
  enum change_type {
    NONE = 0,         /// No type of change chosen.
    AUTOMATIC,        /// Automatic provisions apply.
    ADHOC,            /// Ad hoc wage bases.
    NUM_CHANGE_TYPES  /// Number of wage base change types.
  };
private:
  /// <summary>Wage base titles.</summary>
  static const std::string mfwgbase[NUM_CHANGE_TYPES];
public:
  static void check( int ibascht );
  /// <summary>Returns description of wage base projection method.</summary>
  ///
  /// <returns>Description of wage base projection method.</returns>
  ///
  /// <param name="ibascht">The type of wage base change.</param>
  static std::string toString( change_type ibascht )
  { return(mfwgbase[ibascht]); }
private:
  BaseChangeType();
  BaseChangeType( const BaseChangeType& );
  ~BaseChangeType();
  BaseChangeType& operator=( const BaseChangeType& );
};
