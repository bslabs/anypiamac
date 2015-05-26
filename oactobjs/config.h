// Declarations for the <see cref="Config"/> class to manage configuration
// information used by the PIA calculation program.
//
// $Id: config.h 1.19 2011/07/29 13:49:25EDT 044579 Development  $

#pragma once

/// <summary>Parent of classes that manage the configuration information
/// used by the PIA calculation program.</summary>
///
/// <remarks>This class is abstract; use <see cref="ConfigFile"/> or
/// <see cref="ConfigNonFile"/>.</remarks>
///
/// <seealso cref="ConfigFile"/>
/// <seealso cref="ConfigNonFile"/>
class Config
{
private:
  /// <summary>Long output indicator.</summary>
  ///
  /// <remarks>It has the following possible values:
  /// <table>
  ///   True    long output.
  ///   False   short output.
  /// </table></remarks>
  bool longOutput;
  /// <summary>Screen font point size.</summary>
  int screenPoints;
  /// <summary>Printer font point size.</summary>
  int printerPoints;
  /// <summary>Top margin in inches.</summary>
  float topMargin;
  /// <summary>Left margin in inches.</summary>
  float leftMargin;
public:
  Config();
  virtual ~Config();
  /// <summary>Returns left margin in inches.</summary>
  ///
  /// <returns>Left margin in inches.</returns>
  float getLeftMargin() const { return(leftMargin); }
  /// <summary>Returns long output indicator.</summary>
  ///
  /// <returns>Long output indicator.
  /// <table>
  ///   True    long output.
  ///   False   short output.
  /// </table>   /// </returns>
  bool getLongOutput() const { return(longOutput); }
  /// <summary>Returns printer font point size.</summary>
  ///
  /// <returns>Printer font point size.</returns>
  int getPrinterPoints() const { return(printerPoints); }
  /// <summary>Returns screen font point size.</summary>
  ///
  /// <returns>Screen font point size.</returns>
  int getScreenPoints() const { return(screenPoints); }
  /// <summary>Returns top margin in inches.</summary>
  ///
  /// <returns>Top margin in inches.</returns>
  float getTopMargin() const { return(topMargin); }
  /// <summary>Reads this structure.</summary>
  virtual void read() = 0;
  /// <summary>Sets left margin in inches.</summary>
  ///
  /// <param name="newLeftMargin">New left margin in inches.</param>
  void setLeftMargin( float newLeftMargin )  { leftMargin = newLeftMargin; }
  /// <summary>Sets long output indicator.</summary>
  ///
  /// <param name="newLongOutput">New long output indicator.</param>
  void setLongOutput( bool newLongOutput ) { longOutput = newLongOutput; }
  /// <summary>Sets printer font point size.</summary>
  ///
  /// <param name="newPrinterPoints">New printer font point size.</param>
  void setPrinterPoints( int newPrinterPoints )
  { printerPoints = newPrinterPoints; }
  /// <summary>Sets screen font point size.</summary>
  ///
  /// <param name="newScreenPoints">New screen font point size.</param>
  void setScreenPoints( int newScreenPoints )
  { screenPoints = newScreenPoints; }
  /// <summary>Sets top margin in inches.</summary>
  ///
  /// <param name="newTopMargin">New top margin in inches.</param>
  void setTopMargin( float newTopMargin ) { topMargin = newTopMargin; }
  /// <summary>Writes this structure.</summary>
  virtual void write() = 0;
};
