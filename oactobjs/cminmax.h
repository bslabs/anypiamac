// $Id: cminmax.h 1.8 2008/12/23 12:30:48EST 729915 Development  $
//
// Defines for min and max needed for other than MSVC.

#if !defined(_MIN)
#include <algorithm>
#if !defined(_WIN32)
/// <summary>Definition of minimum when using other than MSVC.</summary>
///
/// <remarks>MSVC uses _MIN in its own include files, so we do not define it
///   here.</remarks>
///
/// <param name="a">First item to compare.</param>
/// <param name="b">Second item to compare.</param>
#define _MIN(a,b) min(a,b)
/// <summary>Definition of maximum when using other than MSVC.</summary>
///
/// <remarks>MSVC uses _MAX in its own include files, so we do not
///   define it here.</remarks>
///
/// <param name="a">First item to compare.</param>
/// <param name="b">Second item to compare.</param>
#define _MAX(a,b) max(a,b)
#endif
#endif
