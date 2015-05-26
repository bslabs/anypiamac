// Declarations for the <see cref="Assumptions"/> class - parent of classes
// that manage the average wage and benefit increase assumptions.

// $Id: Assumptions.h 1.22 2011/07/28 08:55:09EDT 044579 Development  $

#pragma once

#include "cachupf.h"
#include "biproj.h"
#include "AwincDoc.h"

/// <summary>Parent of classes that manage the average wage and benefit
///   increase assumptions.</summary>
///
/// <remarks>One of its subclasses should be instantiated:
/// <see cref="AssumptionsFile"/> if it is to be read from disk,
/// <see cref="AssumptionsNonFile"/> otherwise.</remarks>
class Assumptions
{
private:
  /// <summary>Current year (year after last known benefit increase).</summary>
  int istart;
  /// <summary>Maximum projected year.</summary>
  int maxyear;
public:
   Assumptions( int newIstart, int newMaxyear );
   Assumptions( int newMaxyear );
   virtual ~Assumptions();
   /// <summary>Set average wage increases from stored values.</summary>
   virtual void averageWageMenu( int ) = 0;
   /// <summary>Set benefit increases from stored values.</summary>
   virtual void benefitIncMenu( int ) = 0;
   const std::string& getAverageWage( int altNum );
   const std::string& getBenefitInc( int altNum );
   /// <summary>Returns projected average wage increases.</summary>
   ///
   /// <returns>Projected average wage increases.</returns>
   virtual const AwincDoc& getAwincProj() const = 0;
   /// <summary>Returns projected benefit increases.</summary>
   ///
   /// <returns>Projected benefit increases.</returns>
   virtual const Biproj& getBiProj() const = 0;
   /// <summary>Returns catch-up benefit increases.</summary>
   ///
   /// <returns>Catch-up benefit increases.</returns>
   virtual const CatchupDoc& getCatchupDoc() const = 0;
   /// <summary>Returns current year.</summary>
   ///
   /// <returns>Current year.</returns>
   int getIstart() const { return(istart); }
   /// <summary>Returns maximum projected year.</summary>
   ///
   /// <returns>Maximum projected year.</returns>
   int getMaxyear() const { return(maxyear); }
   /// <summary>Set title of projected average wage increases.</summary>
   virtual void setAwincProjTitle( int ) = 0;
   /// <summary>Set title of projected benefit increases.</summary>
   virtual void setBiProjTitle( int ) = 0;
   /// <summary>Sets the current year, including for the members.</summary>
   ///
   /// <param name="newIstart">The new current year.</param>
   virtual void setIstart( int newIstart ) {istart = newIstart;}
};
