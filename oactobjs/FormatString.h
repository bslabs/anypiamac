// Declarations of functions to mimic AfxFormatString using Standard C++
// Library strings.

// $Id: FormatString.h 1.5 2011/08/11 16:23:34EDT 044579 Development  $

#include <string>

void AfxFormatString1( std::string& result, unsigned int rsc,
  const std::string& filler );
void AfxFormatString2( std::string& result, unsigned int rsc,
  const std::string& filler1, const std::string& filler2 );
