// Functions for the <see cref="CatchupDoc"/> class to manage a set of
// catch-up benefit increases.
//
// $Id: cachpdoc.cpp 1.11 2011/07/29 13:49:26EDT 044579 Development  $

#include "cachupf.h"

using namespace std;

/// <summary>Initializes catch-up benefit increase file with
/// specified starting year.</summary>
///
/// <param name="newCstart">Starting year of catch-up benefit
/// increases.</param>
CatchupDoc::CatchupDoc( int newCstart ) : catchup(newCstart)
{ }

/// <summary>Initializes catch-up benefit increase file with
/// starting year 0.</summary>
CatchupDoc::CatchupDoc() : catchup()
{ }

/// <summary>Destroys catch-up benefit increase file.</summary>
CatchupDoc::~CatchupDoc()
{ }
