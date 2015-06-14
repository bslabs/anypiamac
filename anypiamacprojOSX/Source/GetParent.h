// $Id: GetParent.h,v 1.3 2007/11/07 13:07:00 djolson Exp $
#ifndef	_GetParent_h_
#define	_GetParent_h_

/*
**	GetParent.c
*/

typedef Boolean (*tFileListFunc) (CInfoPBRec *cpb, void *arg);

Ptr GetFullPathName (FSSpec &spec);
Boolean	GetParent (FSSpec *spec);
short	GetFileList (short vRefNum, long dirID, tFileListFunc func, void *arg);

#endif