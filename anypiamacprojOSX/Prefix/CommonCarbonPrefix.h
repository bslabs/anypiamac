// ===========================================================================
//	CommonCarbonPrefix.h	©1999-2003 Metrowerks Corporation. All rights reserved.
// ===========================================================================
//	Common options for a Carbon Target

// ---------------------------------------------------------------------------
//	Carbon Target

#define PP_Target_Carbon		1

#define PP_Target_Classic		(!PP_Target_Carbon)

#define TARGET_API_MAC_CARBON	PP_Target_Carbon
#define TARGET_API_MAC_OS8		PP_Target_Classic


// ---------------------------------------------------------------------------
//	Options
	
#define PP_Uses_PowerPlant_Namespace	0
#define PP_StdDialogs_Option			PP_StdDialogs_NavServicesOnly
#define PP_Uses_Aqua_MenuBar			1

#define PP_Warn_Obsolete_Classes		1

#define PP_Suppress_Notes_221			1

// 'check' is used as a function name in many classes, and this conflicts
// with the check() macro defined in <AssertMacros.h>
// Define __ASSERTMACROS__ so this file doesn't get included
#define __ASSERTMACROS__


#include <MSLCarbonPrefix.h>