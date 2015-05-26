// Functions for the <see cref="Path"/> class that provides operations on
// string instances that contain file or directory path information.

// $Id: Path.cpp 1.9 2012/03/30 07:43:23EDT 277133 Development  $

#include "Path.h"

using namespace std;

// <summary>A string with all the possible separator characters.</summary>
const std::string Path::allSeparators = "\\/";

/// <summary>Combines two path strings.</summary>
///
/// <returns>A string containing the combined paths.</returns>
///
/// <param name="path1">The first path.</param>
/// <param name="path2">The second path.</param>
std::string Path::combine( const std::string& path1,
  const std::string& path2 )
{
  if (path1.length() == 0) {
    return path2;
  }
  if (path2.length() == 0) {
    return path1;
  }
  const string::size_type path1Length = path1.length();
  const char path1LastChar = path1[path1Length - 1];
  if (path1LastChar == directorySeparatorChar ||
    path1LastChar == altDirectorySeparatorChar) {
    return string(path1 + path2);
  }
  else {
    return string(path1 + directorySeparatorChar + path2);
  }
}

/// <summary>Returns the directory information for the specified path
/// string.</summary>
///
/// <returns>A string containing directory information for path.</returns>
///
/// <param name="path">The path of a file or directory.</param>
std::string Path::getDirectoryName( const std::string& path )
{
  const string::size_type index = path.find_last_of(allSeparators);
  return (index == string::npos) ? path : path.substr(index);
}

/// <summary>Returns the file name information for the specified path
/// string.</summary>
///
/// <returns>A string containing the file name information for path.</returns>
///
/// <param name="path">The path of a file or directory.</param>
std::string Path::getFileName( const std::string& path )
{
  const string::size_type index = path.find_last_of(allSeparators);
  std::string temp("");
  if (index != string::npos) {
    temp = path.substr(index+1,path.length()-index-1);
  }
  return temp;
}

/// <summary>Returns a relative path to the file for the specified path
/// string.  This is designed to provide a relative link to a file
/// from an html page.</summary>
///
/// <returns>A string containing directory information for path.</returns>
///
/// <param name="path">The path of a file or directory.</param>
/// <param name="height">The relative height of the file.</param>
std::string Path::getRelativeName( const std::string& path, const int height )
{
  if (height <= 0) {
    return getFileName(path);
  }
  string temp = getFileName(path);
  string temp2 = path.substr(0,path.length() - temp.length() - 1);
  temp = Path::combine(Path::getRelativeName(temp2,height-1),temp);
  return temp;
}

/// <summary>Changes the extension of a path string.</summary>
///
/// <returns>The string containing the modified path information.</returns>
///
/// <remarks>If the extension argument is empty or consists solely of a
/// period, this function removes any existing extension. Otherwise, if
/// neither path nor extension contains a period, this function adds the
/// period. Then the rest of the extension argument after the period is added.
/// </remarks>
///
/// <param name="path">The path information to modify.</param>
/// <param name="extension">The new extension (with a leading period).</param>
std::string Path::changeExtension( const std::string& path,
const std::string& extension )
{
  const string::size_type indexSeparator = path.find_last_of(allSeparators);
  const string::size_type indexPeriod = path.find_last_of('.');
  const string::size_type size = extension.size();
  if (indexPeriod == std::string::npos) {
    // there is no current extension
    string temp(path);
    if (size > 1 || (size == 1 && extension[0] != '.')) {
      if (extension[0] != '.') {
        temp += string(".");
      }
      temp += extension;
    }
    return temp;
  }
  else if (indexSeparator == std::string::npos ||
    indexSeparator < indexPeriod) {
    // there is no separator, or the separator comes before the extension,
    // so overwrite it
    string temp(path.substr(0, indexPeriod));
    if (size > 1 || (size == 1 && extension[0] != '.')) {
      if (extension[0] != '.') {
        temp += string(".");
      }
      temp += extension;
    }
    return temp;
  }
  else {
    // there is no extension after the separator
    string temp(path);
    if (size > 1 || (size == 1 && extension[0] != '.')) {
      if (extension[0] != '.') {
        temp += string(".");
      }
      temp += extension;
    }
    return temp;
  }
}
