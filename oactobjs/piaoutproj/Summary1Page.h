// Declarations for the <see cref="Summary1Page"/> class to manage the top part
// of a summary of results page.

// $Id: Summary1Page.h 1.10.1.1 2012/03/08 07:17:54EST 277133 Development  $

#pragma once

#include "PiaPageOut.h"
class PiaCalAny;

/// <summary>Manages the top part of the summary of results page from the
/// calculation of a Social Security benefit.</summary>
///
/// <remarks>An instance of this class is created in <see cref="PiaOut"/>.
///
/// This is used in combination with <see cref="Summary2Page"/> and
/// <see cref="Summary3Page"/>.</remarks>
///
/// <seealso cref="Summary2Page"/>
/// <seealso cref="Summary3Page"/>
/// <seealso cref="PiaOut"/>
class Summary1Page : public PiaPageOut
{
private:
   /// <summary>Worker basic data.</summary>
   const WorkerDataGeneral& workerData;
   /// <summary>Pia calculation summary data.</summary>
   const PiaData& piaData;
   /// <summary>Pia calculation detailed data.</summary>
   const PiaCalAny& piaCal;
public:
   Summary1Page( const PiaCalAny& newPiaCal );
   ~Summary1Page();
   void prepareStrings();
   void printDisInfo();
private:
   // {secret}
   Summary1Page& operator=( Summary1Page& newSummary1Page );
};
