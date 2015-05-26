// Declarations for the <see cref="AnypiaOut"/> class to manage all output
// pages from Anypia.

// $Id: AnypiaOut.h 1.17.1.1 2012/03/08 07:17:34EST 277133 Development 277133(2013/08/12 11:19:07EDT) $

#pragma once

#include <vector>
class WorkerData;
class PiaData;
class PiaCalAny;
class PiaOut;
class PebsOut;
class PageOut;

/// <summary>Manages all output pages from the Anypia program.</summary>
///
/// <remarks>The pages are in two objects: a <see cref="PiaOut"/> for
/// non-Statement calculations, and a <see cref="PebsOut"/> for Statement
/// calculations.
///
/// An instance of this class is used in the Anypia program.</remarks>
///
/// <seealso cref="PiaOut"/>
/// <seealso cref="PebsOut"/>
class AnypiaOut
{
private:
  /// <summary>Array of pages to print.</summary>
  std::vector< const PageOut* > pages;
  /// <summary>Benefit calculation data.</summary>
  const WorkerData* workerData;
  /// <summary>Benefit calculation data.</summary>
  const PiaData* piaData;
  /// <summary>Benefit calculation formulas.</summary>
  const PiaCalAny* piaCal;
  /// <summary>Output pages for non-Statement calculations.</summary>
  const PiaOut* piaOut;
  /// <summary>Output pages for Statement calculations.</summary>
  const PebsOut* pebsOut;
public:
  AnypiaOut();
  void fillPageSlots( bool valid, bool longOutput );
  /// <summary>Returns one page to print.</summary>
  ///
  /// <returns>One page to print.</returns>
  ///
  /// <param name="index">Index of page to print (0-based).</param>
  const PageOut* getPage( int index ) const { return pages[index]; }
  /// <summary>Returns number of pages to print.</summary>
  ///
  /// <returns>Number of pages to print.</returns>
  unsigned getPageCount() const { return (unsigned)pages.size(); }
  /// <summary>Sets pointer to Statement output.</summary>
  ///
  /// <param name="newPebsOut">New Statement output.</param>
  void setPebsOut( const PebsOut* newPebsOut ) { pebsOut = newPebsOut; }
  /// <summary>Sets pointer to pia calculation results.</summary>
  ///
  /// <param name="newPiaCal">New pia calculation results.</param>
  void setPiaCal( const PiaCalAny* newPiaCal ) { piaCal = newPiaCal; }
  /// <summary>Sets pointer to pia calculation data.</summary>
  ///
  /// <param name="newPiaData">New pia calculation data.</param>
  void setPiaData( const PiaData* newPiaData ) { piaData = newPiaData; }
  /// <summary>Sets pointer to pia calculation output.</summary>
  ///
  /// <param name="newPiaOut">New pia calculation output.</param>
  void setPiaOut( const PiaOut* newPiaOut ) { piaOut = newPiaOut; }
  /// <summary>Sets pointer to worker data.</summary>
  ///
  /// <param name="newWorkerData">New worker data.</param>
  void setWorkerData( const WorkerData* newWorkerData )
  { workerData = newWorkerData; }
};
