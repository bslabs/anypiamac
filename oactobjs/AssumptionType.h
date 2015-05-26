// Declaration of the <see cref="AssumptionType"/> class containing the
// assumption types allowed in the piadata library.
//
// $Id: AssumptionType.h 1.7 2011/07/28 08:55:09EDT 044579 Development  $

#pragma once

/// <summary>The assumption types (about future average wage and benefit
/// increases) allowed in the piadata library.</summary>
///
/// <remarks>There is an enum and some static functions, but no member
/// variables, so there are no instances of this class.</remarks>
class AssumptionType
{
public:
  /// <summary>Type of assumptions.</summary>
  enum assum_type {
    NONE,        /// No assumptions chosen.
    ALTERN_I,    /// Alternative 1 from Trustees Report.
    ALTERN_IIB,  /// Alternative 2 from Trustees Report.
    ALTERN_III,  /// Alternative 3 from Trustees Report.
    ALTERN_IIA,  /// Flat assumptions (same as FLAT).
    FLAT,        /// Flat assumptions.
    PEBS_ASSUM,  /// Old PEBES assumptions.
    OTHER_ASSUM  /// User-specified assumptions.
  };
  /// <summary>True to use a wider range of possible assumptions for
  /// Statement calculations.</summary>
  static const bool EXPANDED_PEBES = false;
public:
  static void ialtawCheck1( int ialtawt );
  static void ialtawCheck2( int ialtawt );
  static void ialtbiCheck1( int ialtbit );
  static void ialtbiCheck2( int ialtbit );
  /// <summary>Returns true if argument is not a Trustees Report assumption.
  /// </summary>
  ///
  /// <returns>True if argument is not a Trustees Report assumption
  /// (<see cref="assum_type::FLAT"/>, <see cref="assum_type::PEBS_ASSUM"/>, or
  /// <see cref="assum_type::OTHER_ASSUM"/>).</returns>
  ///
  /// <param name="altNum">Assumption type to check.</param>
  static bool isNotTR( int altNum )
  { return (altNum == FLAT || altNum == PEBS_ASSUM || altNum == OTHER_ASSUM); }
  /// <summary>Returns true if argument is a Trustees Report assumption.
  /// </summary>
  ///
  /// <returns>True if argument is a Trustees Report assumption
  /// (<see cref="assum_type::ALTERN_I"/>,
  /// <see cref="assum_type::ALTERN_IIB"/>,
  /// <see cref="assum_type::ALTERN_III"/>, or
  /// <see cref="assum_type::ALTERN_IIA"/>).</returns>
  ///
  /// <param name="altNum">Assumption type to check.</param>
  static bool isTR( int altNum )
  { return (altNum == ALTERN_I || altNum == ALTERN_IIB ||
    altNum == ALTERN_III || altNum == ALTERN_IIA); }
  /// <summary>Returns true if argument is a statement-type constant-dollar
  /// assumption.</summary>
  ///
  /// <returns>True if argument is a statement-type assumption
  /// (<see cref="assum_type::FLAT"/> or <see cref="assum_type::PEBS_ASSUM"/>).
  /// </returns>
  ///
  /// <param name="altNum">Assumption type to check.</param>
  static bool isFlatOrPebs(int altNum) 
  { return (altNum == FLAT || altNum == PEBS_ASSUM); }
private:
  AssumptionType();
  AssumptionType( const AssumptionType& );
  ~AssumptionType();
  AssumptionType& operator=( const AssumptionType& );
};
