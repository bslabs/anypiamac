// $Id: CAssumptionsDialog.h,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CAssumptionsDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CAssumptionsDialog : public LDialogBox
{
   public:
      enum { class_ID = 'AssD' };
      
      static CAssumptionsDialog * CreateAssumptionsDialogStream(
         LStream * inStream );
      CAssumptionsDialog( LStream * inStream );
      ~CAssumptionsDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
   public:
      CTextDocument * theDoc;
      int mIaltbi;  // benefit increase code
      int mIaltaw;  // average wage increase code
      std::string mBeninc[7];  // description of benefit increase assumptions
      std::string mAvgwage[7];  // description of average wage increase assumptions
      int mBases;  // 1 if automatic bases, 2 otherwise
      bool mbBases;  // true if type of wage base projection is needed
      std::string mBitext;  // user-specified benefit increase title
      std::string mAwtext;  // user-specified average wage increase title
      bool mbCachup;  // true if catch-up increases are allowed
};
