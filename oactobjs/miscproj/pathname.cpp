// $Id: pathname.cpp,v 1.8 2010/11/08 15:02:40 djolson Exp $
//
// Functions for Pathname class.

#include "pathname.h"
#include "PathnameSeparator.h"

using namespace std;

/// <summary>Constructor.</summary>
///
/// <remarks>The parent directory is set to "" in the default constructor.
/// </remarks>
Pathname::Pathname() : pathname(), corename(), extension()
{
   pastename();
}

/// <param name="newDirectory">New parent directory to use.</param>
Pathname::Pathname( const std::string& newDirectory ) : pathname(newDirectory),
corename(), extension()
{
   pastename();
}

/// <summary>Sets full filename from the three parts.
void Pathname::pastename()
{
   fullname = pathname;
   if (pathname.length() > 0) {
      fullname += separator;
   }
   fullname += corename;
   fullname += extension;
}
