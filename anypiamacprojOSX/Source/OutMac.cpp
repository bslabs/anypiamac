// @doc OUTMAC
// $Id: OutMac.cpp,v 1.4 2007/11/07 13:07:00 djolson Exp $
// @module OutMac.cpp |
//
// Functions to write output on a Mac.

#include "comma.h"
#include "OutMac.h"

using namespace std;

// @mfunc Initializes output file core name and extension.
//
// @parm New core file name.

OutMac80::OutMac80( Boolean refresh ) : Out80("",""),
fRefresh(refresh), fLines(0)
{ }

// @mfunc Destroys an OutMac80 structure.

OutMac80::~OutMac80( )
{ }

void OutMac80::deleteContents()
{
   TEHandle theHandle = textEditView->GetMacTEH();
   ::TESetSelect(0, 32767, theHandle);
   ::TEDelete(theHandle);
}

void OutMac80::printLine( const std::string& string )
{
   const char *thePtr = string.c_str();

   textEditView->Insert(thePtr, strlen(thePtr), NULL, fRefresh);
   textEditView->Insert("\r", 1, NULL, fRefresh);
   fLines++;
}

void OutMac80::printLine( const char *str )
{
   textEditView->Insert(str, strlen(str), NULL, fRefresh);
   textEditView->Insert("\r", 1, NULL, fRefresh);
   fLines++;
}

void OutMac80::advanceLine()
{
   textEditView->Insert("\r", 1);
   fLines++;
}

// @mfunc Print a line of output with no carriage return (not used).

void OutMac80::printLinena( const std::string& )
{ }
      
// @mfunc Print a line of output with no carriage return (not used).

void OutMac80::printLinena( const char* )
{ }

// @mfunc Move to next page (not used).

void OutMac80::formfeed()
{ }
