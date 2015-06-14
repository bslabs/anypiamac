// $Id: CSurvivorDialog.h,v 1.5 2010/08/16 18:18:07 djolson Exp $
//  CSurvivorDialog.h

#pragma once

#include <LDialogBox.h>
#include "CTextDocument.h"
#include "wrkrdata.h"
#include "PiaDataArray.h"

class CSurvivorDialog : public LDialogBox
{
   public:
      enum { class_ID = 'SurD' };
      
      static CSurvivorDialog * CreateSurvivorDialogStream(
         LStream * inStream );
      CSurvivorDialog( LStream * inStream );
      ~CSurvivorDialog();
      virtual void FindCommandStatus( CommandT inCommand,
         Boolean &outEnabled, Boolean &outUsesMark,
         UInt16 &outMark, Str255 outName );
      virtual void ListenToMessage( MessageT inMessage, void * ioParam );
      void SaveResults();
      virtual void SetupDialog();
   private:
      virtual void FinishCreateSelf();
      std::string getTextBoxString( long textBoxID );
      void onChildSurv();
      void onMother();
      void onDisabWid();
      void onDivDisabWid();
      void onDivAgedWid();
      void onAgedWid();
      void onChildLife();
      void onYoungspouse();
      void onAgedspouse();
      void onDivAgedspouse();
      void onPrevbenie();
      void onNextbenie();
   public:
      CTextDocument * theDoc;
      boost::gregorian::date mJbirth[PiaDataArray::MAXFAMSIZE];
      boost::gregorian::date mJonset[PiaDataArray::MAXFAMSIZE];
      char mSurv[PiaDataArray::MAXFAMSIZE];
      char mYoung[PiaDataArray::MAXFAMSIZE];  // '1' if young spouse,
         // ' ' otherwise
      int mFammem;  // family member currently being worked on (0-14)
      int mFamsize;  // size of family (1-15)
      DateMoyr mEnt[PiaDataArray::MAXFAMSIZE];
      WorkerData::ben_type mIoasdi;
      bool check();
      void setupdata();
};
