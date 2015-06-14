// $Id: CNonPebesDialog.h,v 1.5 2008/10/31 12:51:40 djolson Exp $
//  CNonPebesDialog.h

#pragma once

#include <LDialogBox.h>
#include <LGroupBox.h>
#include "CTextDocument.h"

class CNonPebesDialog : public LDialogBox
{
   public:
      enum { class_ID = 'NpbD' };
      
      static CNonPebesDialog * CreateNonPebesDialogStream(
         LStream * inStream );
      CNonPebesDialog( LStream * inStream );
      ~CNonPebesDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
      void OnBacktype();
      void OnFwrdtype();
   public:
      CTextDocument * theDoc;
      DateMoyr mEntdate;  // temporary entitlement date
      DateMoyr mBendate;  // temporary benefit date
      int mBegin;  // first year of earnings
      int mEnd;  // last year of earnings
      int mBirthyear;  // year of birth
      int mJoasdi;  // type of benefit
      bool mRecalc;  // recalculation indicator
      // @cmember Backwards projection indicator.
      bool mbBack;
      // @cmember Forwards projection indicator.
      bool mbFwrd;
      // @cmember Noncovered pension indicator.
      bool mbNonpen;
      int miBackType;  // type of backwards projection
      int miFwrdType;  // type of forwards projection
      int mIndearn;  // true if any regular earnings
      int mBackYear;
      double mBackPerc;
      int mFwrdYear;
      double mFwrdPerc;
      // @cmember Amount of monthly noncovered pension.
      double mNonpen;
      // Date of entitlement to noncovered pension.
      DateMoyr mNonpendate;
      // @cmember Number of periods of disability.
      unsigned mValdi;
      boost::gregorian::date mBirthdate;
      boost::gregorian::date mDeathdate;
};
