// Declarations for the <see cref="FloatAnnual"/> class to manage an array
// of annual floats.

// $Id: floatann.h 1.49 2011/08/11 16:23:33EDT 044579 Development  $

#pragma once

#include <numeric>
#include <vector>
#include "boost/serialization/access.hpp"
#include "Resource.h"
#include "PiaException.h"

/// <summary>Manages an array of annual floats.</summary>
//
/// <remarks>See <see cref="DoubleAnnual"/> and <see cref="IntAnnual"/> for
/// similar classes that manage arrays of annual doubles and integers.
/// </remarks>
///
/// <seealso cref="DoubleAnnual"/>
/// <seealso cref="IntAnnual"/>
class FloatAnnual
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  // <summary>Archives the data.</summary>
  //
  // <remarks>When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<. Likewise, when the class Archive is
  // a type of input archive, the & operator is defined similar to >>.
  // </remarks>
  //
  // <param name="ar">The archive to use.</param>
  // <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
    { ar & theData; }
public:
  /// <summary>Vector of floats.</summary>
  std::vector< float > theData;
  /// <summary>Base year of data.</summary>
  const int baseYear;
private:
  FloatAnnual& operator=( const FloatAnnual& newData );
public:
  FloatAnnual( int newBaseYear, int newLastYear );
  /// <summary>Destructor.</summary>
  virtual ~FloatAnnual() { }
  /// <summary>Returns one value of data, non const.</summary>
  ///
  /// <returns>One value of data for the specified year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_FLOATANN1"/> if year is out of range.
  /// (only in debug mode)</exception>
  ///
  /// <param name="year">Year for which data is desired.</param>
  float& operator[]( int year ) {
#if !defined(NDEBUG)
    if (year < baseYear || year > getLastYear()) {
      throw PiaException(PIA_IDS_FLOATANN1);
    }
#endif
    return theData[year - baseYear];
  }
  /// <summary>Returns one value of data, const.</summary>
  ///
  /// <returns>One value of data for the specified year.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_FLOATANN1"/> if year is out of range.
  /// (only in debug mode)</exception>
  ///
  /// <param name="year">Year for which data is desired.</param>
  const float operator[]( int year ) const
  {
#if !defined(NDEBUG)
    if (year < baseYear || year > getLastYear()) {
      throw PiaException(PIA_IDS_FLOATANN1);
    }
#endif
    return theData[year - baseYear];
  }
  /// <summary>Accumulates data over a range of years.</summary>
  ///
  /// <remarks>Does not check to be sure range of years is within allowable
  /// range, unless in debug mode.</remarks>
  ///
  /// <returns>Total of data in the vector over the specified range.</returns>
  ///
  /// <exception cref="PiaException"><see cref="PiaException"/> of type
  /// <see cref="PIA_IDS_FLOATANN1"/> if years are out of range (only in
  /// debug mode).</exception>
  ///
  /// <param name="year1">Year with which to start accumulating (inclusive).
  /// </param>
  /// <param name="year2">Year with which to end accumulating (inclusive).
  /// </param>
  /// <param name="startValue">Starting value.</param>
  float accumulate( int year1, int year2, float startValue ) const
  {
#if !defined(NDEBUG)
    if (year1 < baseYear || year1 > year2 || year2 > getLastYear()) {
      throw PiaException(PIA_IDS_FLOATANN1);
    }
#endif
    return std::accumulate(theData.begin() + year1 - baseYear,
      theData.begin() + (year2 + 1 - baseYear), startValue);
  }
  void assign( const FloatAnnual& floatAnnual, int firstYear,
    int lastYear );
  void assign( float newData, int firstYear, int lastYear );
  /// <summary>Fills all data slots with the given value.</summary>
  ///
  /// <param name="value">The value to fill the data slots with.</param>
  void fill( float value )
  {std::fill_n(theData.begin(), theData. size(), value); }
  /// <summary>Returns base year of data.</summary>
  ///
  /// <returns>Base year of data.</returns>
  int getBaseYear() const { return(baseYear); }
  /// <summary>Returns last year of data.</summary>
  ///
  /// <remarks>This is the base year plus the size of the data array, minus 1.
  /// Note that this is less than the base year if the size is zero.</remarks>
  ///
  /// <returns>Last year of data.</returns>
  int getLastYear() const
    { return(baseYear + static_cast< int >(theData.size()) - 1); }
};

namespace boost { namespace serialization {
  /// <summary>Starts to save the data.</summary>
  ///
  /// <remarks>After this function, the regular archiving function is called to
  /// complete the save process. We need this function because there is no
  /// default constructor.</remarks>
  ///
  /// <param name="ar">The archive to use.</param>
  /// <param name="t">The structure to save.</param>
  /// <param name="file_version">The version of the archive to use.</param>
  template<class Archive>
  inline void save_construct_data( Archive & ar, const FloatAnnual * t,
  const unsigned int file_version )
  {
    if (t != (FloatAnnual *)0) {
      const std::size_t count = t->theData.size();
      // save data required to construct instance
      ar << t->baseYear << count;
    }
  }

  /// <summary>Starts to load the data.</summary>
  ///
  /// <remarks>After this function, the regular archiving function is called to
  /// complete the load process. We need this function because there is no
  /// default constructor.</remarks>
  ///
  /// <param name="ar">The archive to use.</param>
  /// <param name="t">The structure to load.</param>
  /// <param name="file_version">The version of the archive to use.</param>
  template<class Archive>
  inline void load_construct_data( Archive & ar, FloatAnnual * t,
  const unsigned int file_version )
  {
    // retrieve data from archive required to construct new instance
    int baseYear;
    std::size_t count;
    ar >> baseYear >> count;
    // invoke inplace constructor to initialize instance of this class
    if (count > 0)
      ::new(t)FloatAnnual(baseYear, baseYear + (int)count - 1);
  }
}} // namespace ...
