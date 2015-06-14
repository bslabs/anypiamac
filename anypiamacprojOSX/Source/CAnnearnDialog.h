// $Id: CAnnearnDialog.h,v 1.3 2007/11/07 13:06:56 djolson Exp $
//  CAnnearnDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"

class CAnnearnDialog : public LDialogBox
{
   public:
      enum { class_ID = 'AneD' };
      
      static CAnnearnDialog * CreateAnnearnDialogStream(
         LStream * inStream );
      CAnnearnDialog( LStream * inStream );
      ~CAnnearnDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
      void setupdata();
      bool check();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
   public:
      CTextDocument * theDoc;
      int mStartyear;  // first year of regular earnings
      int mLastyear;  // last year of regular earnings
      int mStartyearRR;  // first year of railroad earnings
      int mLastyearRR;  // last year of railroad earnings
      int mStartyear1;  // first year to show
      int mLastyear1;  // last year to show
      int mEarnyear;  // year currently being shown
      bool mQcbyyear;  // indicator for annual quarters of coverage required
      bool mMqge;  // indicator for Medicare earnings
      bool mbPebes;  // true if this is PEBES calculation
      bool mIndrr;  // true if any railroad earnings
      bool mIndearn;  // true if any regular earnings
      bool mChildcare;  // true if this is a childcare year
      double mRRearn;
      int mRRqc;
      int mQcann;
      double mEarnoasdi;
      double mEarnhi;
      int mEarntype;
      int mTaxtype;
      DoubleAnnual mDAoasdi;  // OASDI earnings
      DoubleAnnual mDAhi;  // HI earnings
      IntAnnual mIAearntype;  // type of earnings
      IntAnnual mIAtaxtype;  // type of taxes
      IntAnnual mIAqc;  // regular quarters of coverage
      IntAnnual mIArrqc;  // railroad quarters of coverage
      BitAnnual mBAchildcare;  // childcare years
      DoubleAnnual mDArrearn;  // railroad earnings
      DoubleAnnual mDAbase;  // OASDI wage base
      DoubleAnnual mDAfq;  // average earnings
      DoubleAnnual mDAwaghigh;  // high earnings
      DoubleAnnual mDAwagmin;  // low earnings
};
