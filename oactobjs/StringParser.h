// Declarations of functions to parse a string.

// $Id: StringParser.h 1.5 2011/08/19 15:57:43EDT 044579 Development  $

#include <string>
#include <vector>

std::vector< std::string > parseString( const std::string& str );
std::vector< std::string > parseString( const std::string& str, char sep );
