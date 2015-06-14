// @doc OUTMAC
// $Id: OutMac.h,v 1.3 2007/11/07 13:07:00 djolson Exp $
// @module OutMac.h |
//
// Declarations for class to manage output file for a Mac.

#ifndef __OUTMAC_H
#define __OUTMAC_H    1    // Prevent multiple #includes

#include <LTextEditView.h>
#include "outfile.h"

// @class The OutMac80 class manages output on a Mac. It has some
//   limitations, but basically it does line-oriented output to the
//   screen and the printer, especially when they are not supposed
//   to look the same or when page breaks are in different places.
//
// @base public | Out80

class OutMac80 : public Out80
{
   // @access Public Members
   public:
      // @cmember The number of lines placed into the outfile thus far.
	  int fLines;
   // @access Private Members
   private:
      // @cmember Pointer to text edit view.
      LTextEditView *textEditView;
      // @cmember TRUE if refresh after scrolling, FALSE if not.
	  Boolean fRefresh;
   // @access Public Members
   public:
      // @cmember Constructor.
      OutMac80( Boolean refresh = true );
      // @cmember Destructor.
      ~OutMac80();
      // @cmember Advances one line.
      void advanceLine();
      // @cmember Clear out the text from the TextEdit record.
      void deleteContents();
      // @cmember Move to next page.
      void formfeed();
      // @cmember Print a line of output with a carriage return.
      void printLine( const std::string& string);
      // @cmember Print a line of output with a carriage return.
      void printLine( const char* string);
      // @cmember Print a line of output with no carriage return.
      void printLinena( const std::string& string );
      // @cmember Print a line of output with no carriage return.
      void printLinena( const char* string );
      // @cmember Set the text edit view.
      void setTextEditView( LTextEditView *newTextEditView );
};

// @mfunc Set the text edit view.
//
// @parm New text edit view.
inline void OutMac80::setTextEditView( LTextEditView *newTextEditView )
{ textEditView = newTextEditView; }

#endif  // __OUTMAC_H
