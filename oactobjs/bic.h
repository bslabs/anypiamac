// Declarations for the <see cref="Bic"/> class to manage bic (Beneficiary
// Identification code) code.
//
// $Id: bic.h 1.19 2011/07/29 11:25:02EDT 044579 Development  $

#pragma once

#include <string>

/// <summary>Represents one 2-character bic (Beneficiary Identification code)
/// code.</summary>
///
/// <remarks>The major bic code has the following possible values:
/// <table>
/// A   primary (old-age or disability).
/// B   wife or husband (young or aged).
/// C   child (minor, student, or disabled).
/// D   aged widow(er).
/// E   mother or father.
/// F   parent.
/// T   transitionally insured.
/// U   transitionally uninsured.
/// W   disabled widow(er).
/// </table>
/// The minor bic code has the following possible values:
///
/// If majorBic is B, then
/// <table>
///   blank   aged wife or husband.
///   2       young wife or husband.
///   6       divorced aged wife or husband.
/// </table>
/// If majorBic is D, then
/// <table>
///   blank   aged widow(er).
///   6       surviving divorced wife or husband.
/// </table>
/// If majorBic is W, then
/// <table>
///   blank   disabled widow(er).
///   6       surviving disabled divorced wife or husband.
/// </table>
/// An instance of this class is used in <see cref="Secondary"/>.</remarks>
class Bic
{
private:
  /// <summary>Major bic code.</summary>
  ///
  /// <remarks>
  /// A - primary (old-age or disability).
  /// B - wife or husband (young or aged).
  /// C - child (minor, student, or disabled).
  /// D - aged widow(er).
  /// E - mother or father.
  /// F - parent.
  /// T - transitionally insured.
  /// U - transitionally uninsured.
  /// W - disabled widow(er).</remarks>
  char majorBic;
  /// <summary>Minor bic code.  We use a subset of the actual codes.</summary>
  ///
  /// <remarks>
  /// If majorBic is B, then
  ///
  ///   blank - aged wife or husband.
  ///   2 - young wife or husband.
  ///   6 - divorced aged wife or husband.
  ///
  /// If majorBic is D, then
  ///
  ///   blank - aged widow(er).
  ///   6 - surviving divorced wife or husband.
  ///
  /// If majorBic is W, then
  ///
  ///   blank - disabled widow(er).
  ///   6 - surviving disabled divorced wife or husband.</remarks>
  char minorBic;
  /// <summary>Type of survivor titles.</summary>
  static const std::string survivorType[6];
  /// <summary>Type of life auxiliary titles.</summary>
  static const std::string lifeAuxType[4];
public:
  Bic();
  Bic( const std::string& newCode );
  Bic( char newMajorBic, char newMinorBic );
  /// <summary>Compares to another <see cref="Bic"/>.</summary>
  ///
  /// <returns>True if major and minor codes agree, false otherwise.</returns>
  ///
  /// <param name="bictemp">The <see cref="Bic"/> to compare to.</param>
  bool operator==( const Bic& bictemp ) const
    { return majorBic == bictemp.majorBic && minorBic == bictemp.minorBic; }
  /// <summary>Returns true if this benefit is affected by family maximum.
  /// </summary>
  ///
  /// <remarks>Divorced spouses are not affected by family maximum.</remarks>
  ///
  /// <returns>True if this benefit is affected by family maximum.</returns>
  bool eligibleForMax() const { return minorBic != '6'; }
  std::string getLifeAuxType() const;
  /// <summary>Returns the major bic code.</summary>
  ///
  /// <returns>The major bic code.</returns>
  char getMajorBic() const { return((majorBic == 'T') ? 'U' : majorBic); }
  /// <summary>Returns the minor bic code.</summary>
  ///
  /// <returns>The minor bic code.</returns>
  char getMinorBic() const { return(minorBic); }
  std::string getSurvivorType() const;
  /// <summary>Returns true if auxiliary beneficiary (major bic is B, C, D, E,
  /// F, or W).</summary>
  ///
  /// <returns>True if auxiliary beneficiary (major bic is B, C, D, E, F, or
  /// W).</returns>
  bool isAuxiliary() const
  { return(getMajorBic() == 'B' || getMajorBic() == 'C' ||
    getMajorBic() == 'D' || getMajorBic() == 'E' ||
    getMajorBic() == 'F' || getMajorBic() == 'W'); }
  /// <summary>Returns true if disabled widow(er), false otherwise.</summary>
  ///
  /// <returns>True if disabled widow(er), false otherwise.</returns>
  bool isDisabledWidow() const { return(getMajorBic() == 'W'); }
  /// <summary>Returns true if benefit is reducible for age.</summary>
  ///
  /// <remarks>Children's, young spouse's, and parent's benefits are not
  /// reducible for age.</remarks>
  ///
  /// <returns>True if benefit is reducible for age.</returns>
  bool isReducible() const
  { return(majorBic == 'D' || majorBic == 'W' ||
    (majorBic == 'B' && minorBic != '2') || majorBic == 'A'); }
  /// <summary>Returns true if widow(er) (aged or disabled), false
  /// otherwise.</summary>
  ///
  /// <returns>True if widow(er) (aged or disabled), false otherwise.
  /// </returns>
  bool isWidow() const
    { return(getMajorBic() == 'D' || getMajorBic() == 'W'); }
  /// <summary>Returns true if young spouse.</summary>
  ///
  /// <returns>True if young spouse.</returns>
  bool isYoungSpouse() const { return majorBic == 'B' && minorBic == '2'; }
  /// <summary>Returns true if young survivor (child or mother/father),
  /// false otherwise.</summary>
  ///
  /// <returns>True if young survivor (child or mother/father), 
  /// false otherwise.</returns>
  bool isYoungSurvivor() const
    { return(getMajorBic() == 'C' || getMajorBic() == 'E'); }
  void set( const std::string& newCode );
  /// <summary>Sets the major bic code.</summary>
  ///
  /// <param name="newMajorBic">New major bic code.</param>
  void setMajorBic( char newMajorBic ) { majorBic = newMajorBic; }
  /// <summary>Sets the minor bic code.</summary>
  ///
  /// <param name="newMinorBic">New minor bic code.</param>
  void setMinorBic( char newMinorBic ) { minorBic = newMinorBic; }
  static void survivorCheck( char newMajorBic );
  std::string toString() const;
};
