// $Id: CDocumentApp.h,v 1.3 2007/11/07 13:06:57 djolson Exp $
// ===========================================================================
//	CDocumentApp.h		©1994-1999 Metrowerks Inc. All rights reserved.
// ===========================================================================

#ifndef _H_CDocumentApp
#define _H_CDocumentApp
#pragma once

#include <LDocApplication.h>
class CTextDocument;

class CDocumentApp : public LDocApplication {

public:
                        CDocumentApp();
   virtual					~CDocumentApp();

   virtual void         OpenDocument(
                           FSSpec*   inMacFSSpec);

   virtual LModelObject*  MakeNewDocument();
   virtual void         ChooseDocument();

   virtual void         PrintDocument(
                           FSSpec*  inMacFSSpec);

   // pointer to the only document that is open
   CTextDocument*       fOnlyDoc;

protected:
   virtual void         StartUp();
   virtual void         DoReopenApp();

   void                 RegisterClasses();
};

#endif // _H_CDocumentApp