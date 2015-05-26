// Declarations for the <see cref="Path"/> class that provides operations on
// string instances that contain file or directory path information.

// $Id: Path.h 1.7 2012/03/30 07:43:08EDT 277133 Development  $

#pragma once

#include <string>

/// <summary>Provides operations on string instances that
/// contain file or directory path information.</summary>
///
/// <remarks>All members of this class are static and can therefore be called
/// without having an instance of a <see cref="Path"/>.</remarks>
class Path
{
public:
  /// <summary>The alternate character used to separate directory levels in
  /// a path string (forward slash).</summary>
  static const char altDirectorySeparatorChar = '/';
  /// <summary>The character used to separate directory levels in
  /// a path string (backward slash).</summary>
  static const char directorySeparatorChar = '\\';
  /// <summary>A string with all the possible separator characters.</summary>
  static const std::string allSeparators;
public:
  static std::string changeExtension( const std::string& path,
    const std::string& extension );
  static std::string combine( const std::string& path1,
    const std::string& path2 );
  static std::string getDirectoryName( const std::string& path );
  static std::string getFileName( const std::string& path );
  static std::string getRelativeName( const std::string& path, const int height);
};
