// Functions to mimic AfxFormatString using Standard C++ Library strings.

#include "FormatString.h"

using namespace std;

void AfxFormatString1(string& result, unsigned int rsc,
                      const string& filler)
{
   Str255 the255String;
   string theString;
   unsigned int rsc0, rsc1, rsc2;

   if (rsc > 61203) {
      rsc0 = rsc / 4;
      rsc1 = rsc0 - 14300;
      rsc2 = rsc - 4 * rsc0 + 1;
   }
   else {
      rsc1 = 1001;
      rsc2 = 1;
   }
   GetIndString(the255String, rsc1, rsc2);
   char theCString[256];
   strncpy(theCString, (const char *)(the255String + 1),
      (unsigned long)the255String[0]);
   theCString[the255String[0]] = '\0';
   theString = theCString;
   string::size_type index = theString.find("%1");
   // if no "%1" found, return
   if (index == string::npos) {
      result = theString;
      return;
   }
   if (index > 0)
      result = theString.substr(0,index);
   else
      result = "";
   result += filler;
   result += theString.substr(index + 2);
}

void AfxFormatString2(string& result, unsigned int rsc,
   const string& filler1, const string& filler2)
{
   Str255 the255String;
   string theString;
   unsigned int rsc0, rsc1, rsc2;

   if (rsc > 61203) {
      rsc0 = rsc / 4;
      rsc1 = rsc0 - 14300;
      rsc2 = rsc - 4 * rsc0 + 1;
   }
   else {
      rsc1 = 1001;
      rsc2 = 1;
   }
   GetIndString(the255String, rsc1, rsc2);
   char theCString[256];
   strncpy(theCString, (const char *)(the255String + 1),
      (unsigned long)the255String[0]);
   theCString[the255String[0]] = '\0';
   theString = theCString;
   string::size_type index1 = theString.find("%1");
   string::size_type index2 = theString.find("%2");
   // if no "%1" found, return
   if (index1 == string::npos) {
      result = theString;
      return;
   }
   if (index1 > 0)
      result = theString.substr(0,index1);
   else
      result = "";
   result += filler1;
   // if no "%2" found, return
   if (index2 == string::npos) {
      result += theString.substr(index1 + 2);
      return;
   }
   result += theString.substr(index1 + 2, index2 - index1 - 2);
   result += filler2;
   result += theString.substr(index2 + 2);
}
