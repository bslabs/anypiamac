// Declarations for the <see cref="Ssn"/> class to manage a Social Security
// number.

// $Id: ssn.h 1.18 2010/11/23 10:21:00EST 044579 Development  $

#pragma once

#include <string>
#include "boost/serialization/access.hpp"

/// <summary>Manages a Social Security number string, and the 3 parts of an
/// ssn separately as strings.</summary>
///
/// <remarks>An instance of this class is a member of
/// <see cref="WorkerData"/>.</remarks>
class Ssn
{
  /// <summary>Friend class to allow serialization.</summary>
  friend class boost::serialization::access;
  /// <summary>Archives the data.</summary>
  ///
  /// <remarks>When the class Archive corresponds to an output archive, the
  /// & operator is defined similar to <<. Likewise, when the class Archive is
  /// a type of input archive, the & operator is defined similar to >>.
  /// </remarks>
  ///
  /// <param name="ar">The archive to use.</param>
  /// <param name="version">The version of the archive to use.</param>
  template< class Archive >
  void serialize( Archive& ar, const unsigned int version )
    { ar & ssnPart & ssnFull & ssnWithHyphens; }
private:
  /// <summary>First 3 digits, middle 2 digits, and last 4 digits.</summary>
  std::string ssnPart[3];
  /// <summary>Full 9 digits, without hyphens.</summary>
  std::string ssnFull;
  /// <summary>Output as 11 characters:  xxx-xx-xxxx.</summary>
  std::string ssnWithHyphens;
public:
  Ssn();
  Ssn( const std::string& newSsnPart1, const std::string& newSsnPart2,
    const std::string& newSsnPart3 );
  Ssn( const std::string& newSsn );
  Ssn( const Ssn& ssn );
  /// <summary>Destructor.</summary>
  ///
  /// <remarks>Does nothing.</remarks>
  ~Ssn() { }
  Ssn& operator=( const Ssn& ssn );
  bool operator<( const Ssn& ssntemp ) const;
  bool operator==( const Ssn& ssntemp ) const;
  void deleteContents();
  /// <summary>Returns first part of ssn.</summary>
  ///
  /// <returns>First part of ssn.</returns>
  const std::string& getPart1() const { return(ssnPart[0]); }
  /// <summary>Returns second part of ssn.</summary>
  ///
  /// <returns>Second part of ssn.</returns>
  const std::string& getPart2() const { return(ssnPart[1]); }
  /// <summary>Returns third part of ssn.</summary>
  ///
  /// <returns>Third part of ssn.</returns>
  const std::string& getPart3() const { return(ssnPart[2]); }
  void setPart1( const std::string& newPart1 );
  void setPart2( const std::string& newPart2 );
  void setPart3( const std::string& newPart3 );
  void setSsnFull( const std::string& newSsnFull );
  void setSsnFull( const std::string& newSsnPart1,
    const std::string& newSsnPart2, const std::string& newSsnPart3 );
  long toNumber() const;
  std::string toString() const;
  std::string toString( const std::string& format ) const;
  void update();
};
