// $Id: GetParent.c,v 1.3 2007/11/07 13:07:00 djolson Exp $
/********************************************************************************
*
*	This module has been added to the piamac project. It contains functions to
*	convert a FSSpec to a full path name. -MDM
*
********************************************************************************/
#include	<stdio.h>
#include	<string.h>
#include	"GetParent.h"

Boolean GetParent (FSSpec *spec)
{
	CInfoPBRec	CIBlock;
	OSErr		ierr;

	if (spec->parID == fsRtParID) return FALSE;
/*
**	set up for getting folder names
*/
	CIBlock.hFileInfo.ioNamePtr = spec->name;
	CIBlock.hFileInfo.ioVRefNum = spec->vRefNum;

	CIBlock.dirInfo.ioDrParID = spec->parID;
	CIBlock.dirInfo.ioDrDirID = spec->parID;
	CIBlock.dirInfo.ioCompletion = NULL;
	CIBlock.dirInfo.ioFDirIndex = 0x8000;			// get info about aDirID

	ierr = PBGetCatInfo (&CIBlock, false);
	spec->parID = CIBlock.dirInfo.ioDrParID;		// return aDirID of parent
	spec->vRefNum = CIBlock.hFileInfo.ioVRefNum;	// return vRefNum
	
	return TRUE;
}

Ptr GetFullPathName (FSSpec &spec)
{
	unsigned char	*retVal = NULL;
	Ptr				temp;
	FSSpec	parentSpec = spec;
	
//	If no parent, just return the name of the current level
	if (!GetParent (&parentSpec))
	{
		int		n = parentSpec.name[0];
		
		if ((retVal = (unsigned char *) NewPtr (n+1)) != NULL)
			memcpy (retVal, parentSpec.name, n+1);
	}
	
//	Return the name of the upper level, then append :current level
	else if ((temp = GetFullPathName (parentSpec)) == NULL)
		;
	
	else if ((retVal = (unsigned char *) NewPtr (GetPtrSize (temp) + spec.name[0] + 2)) == NULL)
		DisposePtr (temp);
	
	else
	{
		int		n = temp[0];
		
		memcpy (retVal, temp, n + 1);
		memcpy (retVal + n + 1, ":", 1);	n++;
		memcpy (retVal + n + 1, spec.name+1, spec.name[0] + 1);		n += spec.name[0];
		retVal[0] = n;
		DisposePtr (temp);
	}
	
	return (Ptr) retVal;
}

short GetFileList (short vRefNum, long dirID, tFileListFunc func, void *arg)
{
/*
**	Call a func for each file in a folder
*/
	CInfoPBRec		cpb;
	OSErr			ierr = noErr;
	short			i;
	Str63			name;
	short			retVal = 0;

/*
**	Get the cat info for the directory
*/
	cpb.dirInfo.ioCompletion = NULL;
	cpb.dirInfo.ioVRefNum = vRefNum;
	cpb.dirInfo.ioNamePtr = name;

/*
**	Cycle through the files in that directory and call the func for each file
*/	
	for (i=0; ; i++)
	{
		cpb.hFileInfo.ioDirID = dirID;
		cpb.hFileInfo.ioFDirIndex = i+1;
		
	/*
	**	When we get to the last file, just break
	*/
		if ((ierr = PBGetCatInfo (&cpb, FALSE)) != noErr)
			break;
		
	/*
	**	Call the func
	*/
		if (func (&cpb, arg))
			retVal++;
	
	//	Let the func give us a way out
		if (cpb.hFileInfo.ioResult != noErr)
			break;
	}
	return retVal;
}