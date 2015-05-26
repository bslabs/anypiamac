// Declarations for the <see cref="AssumptionsFile"/> class that reads the
// average wage and benefit increase parameters from disk storage.

// $Id: AssumptionsFile.h 1.14 2011/07/28 08:55:09EDT 044579 Development  $

#pragma once

#include "Assumptions.h"
#include "AwincFile.h"
#include "BiprojFile.h"
#include "CatchupFile.h"

/// <summary>Reads the average wage and benefit increase parameters from disk
/// storage.</summary>
///
/// <remarks>Reads the average wage and benefit increase parameters from disk
/// storage.</remarks>
class AssumptionsFile : public Assumptions
{
public:
   /// <summary>Projected average wage increases.</summary>
   AwincFile awincProj;
   /// <summary>Projected benefit increases.</summary>
   BiprojFile biProj;
   /// <summary>Catch-up benefit increases.</summary>
   CatchupFile catchupDoc;
public:
   AssumptionsFile( int newIstart, int newMaxyear,
      const std::string& newDirectory );
   ~AssumptionsFile();
   void averageWageMenu( int altNum );
   void benefitIncMenu( int altNum );
   /// <summary>Returns projected average wage increases.</summary>
   ///
   /// <returns>Projected average wage increases.</returns>
   const AwincDoc& getAwincProj() const { return awincProj; }
   /// <summary>Returns projected benefit increases.</summary>
   ///
   /// <returns>Projected benefit increases.</returns>
   const Biproj& getBiProj() const { return biProj; }
   /// <summary>Returns catch-up benefit increases.</summary>
   ///
   /// <returns>Catch-up benefit increases.</returns>
   const CatchupDoc& getCatchupDoc() const { return catchupDoc; }
   /// <summary>Set title of projected average wage increases.</summary>
   ///
   /// <param name="number">Trustees Report assumption.</param>
   void setAwincProjTitle( int number ) { awincProj.setTitle(number); }
   /// <summary>Set title of projected benefit increases.</summary>
   ///
   /// <param name="number">Trustees Report assumption.</param>
   void setBiProjTitle( int number ) { biProj.setTitle(number); }
   void setIstart( int newIstart );
};
