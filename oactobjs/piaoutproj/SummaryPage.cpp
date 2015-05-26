// Functions for the <see cref="SummaryPage"/> class to manage the summary
// page of results.

// $Id: SummaryPage.cpp 1.6.1.1 2012/03/08 07:17:55EST 277133 Development  $

#include "SummaryPage.h"

using namespace std;

/// <summary>Initializes SummaryPage.</summary>
///
/// <param name="newSummary1Page">Top part of summary page.</param>
/// <param name="newSummary2Page">Middle part of summary page.</param>
/// <param name="newSummary3Page">Bottom part of summary page.</param>
SummaryPage::SummaryPage( const Summary1Page& newSummary1Page,
const Summary2Page& newSummary2Page, const Summary3Page& newSummary3Page ) :
PageOut(), summary1Page(newSummary1Page),
summary2Page(newSummary2Page), summary3Page(newSummary3Page)
{ }

/// <summary>Destructor.</summary>
SummaryPage::~SummaryPage()
{ }

/// <summary>Prepares the three parts of the summary page.</summary>
void SummaryPage::prepareStrings()
{
  outputString.clear();
  for (unsigned i = 0u; i < summary1Page.getCount(); i++) {
    outputString.push_back(summary1Page.getString(i));
  }
  for (unsigned i = 0u; i < summary2Page.getCount(); i++) {
    outputString.push_back(summary2Page.getString(i));
  }
  for (unsigned i = 0u; i < summary3Page.getCount(); i++) {
    outputString.push_back(summary3Page.getString(i));
  }
}
