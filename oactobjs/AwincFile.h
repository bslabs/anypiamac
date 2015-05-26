// Declarations needed for <see cref="AwincFile"/> class to manage average wage
// increases stored on disk.

// $Id: AwincFile.h 1.13 2010/11/22 16:14:40EST 044579 Development  $

#pragma once

#include "AwincDoc.h"

/// <summary>Manages the file of projected average wage increases stored
///   on disk.</summary>
///
/// <remarks>Manages the file of projected average wage increases stored
///   on disk.</remarks>
class AwincFile : public AwincDoc
{
private:
   /// <summary>File information.</summary>
   std::string pathname;
protected:
   void setAwFile( int altNum );
public:
   AwincFile( int newIstart, int newMaxyear,
      const std::string& newDirectory );
   void read( int altNum );
   void setData ( int );
   virtual void setData( int, const DoubleAnnual& );
   void setTitle( int altNum );
   void setTitle( int altNum, const std::string& );
   void write( int altNum );
};
