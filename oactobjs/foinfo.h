// Declarations for the <see cref="FieldOfficeInfo"/> class to manage field
// office information.
//
// $Id: foinfo.h 1.17 2011/07/29 15:26:07EDT 044579 Development  $

#pragma once

#include <string>

/// <summary>Parent of classes that manage the name and address of the field
/// office.</summary>
///
/// <remarks>This class is abstract; use <see cref="FieldOfficeInfoFile"/> or
/// <see cref="FieldOfficeInfoNonFile"/>.</remarks>
///
/// <seealso cref="FieldOfficeInfoFile"/>
/// <seealso cref="FieldOfficeInfoNonFile"/>
class FieldOfficeInfo
{
public:
  /// <summary>Number of address lines (4).</summary>
  static const int numLines = 4;
private:
  /// <summary>Field office address.</summary>
  std::string foaddr[numLines];
  /// <summary>Field office manager name.</summary>
  std::string foname;
  /// <summary>Field office manager title.</summary>
  std::string fotitle;
public:
  FieldOfficeInfo();
  virtual ~FieldOfficeInfo();
  /// <summary>Returns one line of field office address.</summary>
  ///
  /// <returns>One line of field office address.</returns>
  //
  /// <param name="line">Desired line (0 to 3).</param>
  const std::string& getAddress( int line ) const { return(foaddr[line]); }
  /// <summary>Returns field office manager's name.</summary>
  ///
  /// <returns>Field office manager's name.</returns>
  const std::string& getFieldOfficeName() const { return(foname); }
  /// <summary>Returns field office manager's title.</summary>
  ///
  /// <returns>Field office manager's title.</returns>
  const std::string& getFieldOfficeTitle() const { return(fotitle); }
  /// <summary>Sets one line of field office address.</summary>
  ///
  /// <param name="line">Desired line (0 to 3).</param>
  /// <param name="newAddress">New address line.</param>
  void  setAddress( int line, const std::string& newAddress )
  { foaddr[line] = newAddress; }
  /// <summary>Sets field office info.</summary>
  virtual void setData() = 0;
  /// <summary>Sets field office manager's name.</summary>
  ///
  /// <param name="newName">New name.</param>
  void setFieldOfficeName( const std::string& newName ) { foname = newName; }
  /// <summary>Sets field office manager's title.</summary>
  ///
  /// <param name="newTitle">New title.</param>
  void setFieldOfficeTitle( const std::string& newTitle )
  { fotitle = newTitle; }
  /// <summary>Read field office info.</summary>
  virtual void read() = 0;
  /// <summary>Write field office info.</summary>
  virtual void write() = 0;
};
