// Declarations needed for <see cref="AverageWage"/> class to manage array of average wages.

// $Id: avgwg.h 1.22 2010/11/22 16:14:43EST 044579 Development  $

#pragma once

#include <cmath>
#include "boost/serialization/base_object.hpp"
#include "dbleann.h"
#include "Floor.h"

class Awinc;

/// <summary>Manages an array of average wage indexing amounts, or any
///   other earnings series.</summary>
///
/// <remarks>Manages an array of average wage indexing amounts, or any
///   other earnings series.</remarks>
class AverageWage : public DoubleAnnual
{
  // <summary>Allows serialization of this class.</summary>
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
  { ar & boost::serialization::base_object<DoubleAnnual>(*this); }
public:
  AverageWage( int newBaseYear, int newLastYear );
  ~AverageWage();
  AverageWage& operator=( const AverageWage& newData );
   int endYear() const;
   void project( const Awinc& averageWageInc, int firstYeart );
   void project( double factor, int firstYeart, int lastYeart );
   void projectBackward( int year, double factor );
   void projectForward( int year, double factor );
   int startYear() const;
   /// <summary>Projects an earnings amount forward for one year.</summary>
   ///
   /// <returns>The projected earnings.</returns>
   ///
   /// <param name="amount">Earnings amount.</param>
   /// <param name="factor">Multiplicative factor used to increase previous
   /// year.</param>
   static double projectForwardStatic( double amount, double factor )
      { return round(factor * amount); }
   /// <summary>Projects an earnings amount back for one year.</summary>
   ///
   /// <returns>The backward-projected earnings.</returns>
   ///
   /// <param name="amount">Earnings amount.</param>
   /// <param name="factor">Multiplicative factor used to decrease from
   /// following year.</param>
   static double projectBackwardStatic( double amount, double factor )
      { return round(amount / factor); }
   /// <summary>Rounds an earnings amount to 2 decimal places.</summary>
   ///
   /// <returns>Rounded earnings.</returns>
   ///
   /// <param name="wage">Earnings to round.</param>
   static double round( double wage )
      { return (FLOOR(wage * 100.0 + 0.5)) / 100.0; }
};
