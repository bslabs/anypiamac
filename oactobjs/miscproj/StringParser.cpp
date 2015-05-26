// Function definitions to parse a string.

// $Id: StringParser.cpp 1.14 2011/10/24 16:27:52EDT 044579 Development  $

#include "StringParser.h"
#include "isspace.h"

using namespace std;

/// <summary> Parses a string into its delimited parts, using a space
/// separator.</summary>
///
/// <remarks>This version (with only one argument) uses a space as the
/// delimiter.</remarks>
///
/// <returns>A vector of the strings contained in the input.</returns>
///
/// <param name="str">The string to parse.</param>
std::vector< std::string > parseString( const std::string& str )
{
  bool atEnd = true;  // indicator for being at end of a word
  int begin = -1,  // beginning character of word
    end = -1;  // ending character of word
  vector< string > retvec;  // return vector
  for (string::size_type idx = 0; idx < str.size(); idx++) {
    if (ISSPACE(str[idx])) {
      // if not already at end of word, take care of this word
      if (!atEnd) {
        // this is new end of word
        end = static_cast<int>(idx) - 1;
        atEnd = true;
        // push this word onto vector
        retvec.push_back(str.substr(begin, end - begin + 1));
      }
    }
    else {
      // if at end of a word, this is now a new word
      if (atEnd) {
        begin = static_cast<int>(idx);
        atEnd = false;
      }
    }
  }
  // if at end of line without a space, finish parsing final word
  if (str.size() > 0 && end < begin) {
    end = static_cast<int>(str.size());
    retvec.push_back(str.substr(begin, end - begin + 1));
  }
  return retvec;
}

/// <summary> Parses a string into its delimited parts with a specified
/// delimiter.</summary>
///
/// <returns>A vector of the strings contained in the input.</returns>
///
/// <param name="str">The string to parse.</param>
/// <param name="sep">The separator character. If omitted, a space is used.
/// </param>
std::vector< std::string > parseString( const std::string& str, char sep )
{
  bool atEnd = true;  // indicator for being at end of a word
  int begin = -1,  // beginning character of word
    end = -1;  // ending character of word
  vector< string > retvec;  // return vector
  for (string::size_type idx = 0; idx < str.size(); idx++) {
    if (str[idx] == sep) {
      // if not already at end of word, take care of this word
      if (!atEnd) {
        // this is new end of word
        end = static_cast<int>(idx) - 1;
        atEnd = true;
        // push this word onto vector
        retvec.push_back(str.substr(begin, end - begin + 1));
      }
    }
    else {
      // if at end of a word, this is now a new word
      if (atEnd) {
        begin = static_cast<int>(idx);
        atEnd = false;
      }
    }
  }
  // if at end of line without a separator, finish parsing final word
  if (str.size() > 0 && end < begin) {
    end = static_cast<int>(str.size());
    retvec.push_back(str.substr(begin, end - begin + 1));
  }
  return retvec;
}
