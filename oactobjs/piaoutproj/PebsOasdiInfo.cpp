// Functions for the <see cref="PebsOasdiInfo"/> class to manage the Social
// Security Statement oasdi benefit info page.

// $Id: PebsOasdiInfo.cpp 1.8.1.2 2012/03/08 10:56:07EST 277133 Development  $

#include "PebsOasdiInfo.h"
#include "FormatString.h"
#include "PiaException.h"
#include "Resource.h"
#include "PebsOut.h"

using namespace std;

/// <summary>Initializes PebsOasdiInfo.</summary>
///
/// <param name="newWorkerData">Worker data.</param>
PebsOasdiInfo::PebsOasdiInfo( const WorkerData& newWorkerData ) :
PebsPageOut(), workerData(newWorkerData)
{ }

/// <summary>Destructor.</summary>
PebsOasdiInfo::~PebsOasdiInfo()
{ }

/// <summary>Prepares oasdi benefit estimate information.</summary>
void PebsOasdiInfo::prepareStrings()
{
  //try {
  //  outputString.clear();
  //  prepareHeader(workerData);
  //  outputString.push_back("Retirement Benefits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "You can get reduced benefits as early as 62 or get full-retirement");
  //  outputString.push_back(
  //    "at age 65.  (Starting in the year 2000 for people born in 1938 or");
  //  outputString.push_back(
  //    "later, this age will increase gradually.  By 2027, full-retirement");
  //  outputString.push_back(
  //    "age wil be 67 for people born after 1959.)  Your benefits may be");
  //  outputString.push_back(
  //    "higher if you delay retiring until after full-retirement age.");
  //  outputString.push_back("");
  //  outputString.push_back(equals);
  //  outputString.push_back("Disability Benefits");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "These benefits are paid if you become totally disabled before you");
  //  outputString.push_back(
  //    "reach full-retirement age.  To get disability benefits, three things");
  //  outputString.push_back("are necessary:");
  //  outputString.push_back(
  //    "  You need a certain number of work credits and they had to be");
  //  outputString.push_back(
  //    "  earned during a specific period of time;");
  //  outputString.push_back(
  //    "  You must have a physical or mental condition that has lasted, or");
  //  outputString.push_back(
  //    "  is expected to last, at least 12 months or to end in death; and");
  //  outputString.push_back(
  //    "  Your disability must be severe enough to keep you from doing any");
  //  outputString.push_back(
  //    "  substantial work, not just your last job.");
  //  outputString.push_back("");
  //  outputString.push_back(equals);
  //  outputString.push_back("Benefits for Your Family");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "As you work, you also build up protection for your family.  Benefits");
  //  outputString.push_back("may be payable to:");
  //  outputString.push_back(
  //    "  Your unmarried children under age 18 (under age 19 if in high");
  //  outputString.push_back(
  //    "  school) or 18 and older if disabled before age 22;");
  //  outputString.push_back(
  //    "  Your spouse who is age 62 or older or who is any age and caring");
  //  outputString.push_back(
  //    "  for your qualified child who is under age 16 or disabled; and");
  //  outputString.push_back(
  //    "  Your divorced spouse who was married to you for at least 10 years");
  //  outputString.push_back(
  //    "  and who is age 62 or older and unmarried.");
  //  outputString.push_back(
  //    "Usually each family member qualifies for a monthly benefit that is");
  //  outputString.push_back(
  //    "up to 50 percent of your retirement or disability benefit, subject");
  //  outputString.push_back("to the limit explained below.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "If you die, your unmarried young or disabled children may qualify");
  //  outputString.push_back(
  //    "for monthly payments.  Your widow or widower, even if divorced, may");
  //  outputString.push_back("also qualify for payments starting:");
  //  outputString.push_back(
  //    "  At age 60 or age 50 if disabled (if divorced, your marriage must");
  //  outputString.push_back("  have lasted 10 years); or");
  //  outputString.push_back(
  //    "  At any age if caring for your qualified child who is under age 16");
  //  outputString.push_back("  or disabled.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "There is a limit on the amount we can pay to your and your family");
  //  outputString.push_back(
  //    "altogether.  This total depends on the amount of your benefit and");
  //  outputString.push_back(
  //    "the number of family members who also qualify.  The total varies,");
  //  outputString.push_back(
  //    "but is generally equal to about 150 to 180 percent of your");
  //  outputString.push_back(
  //    "retirement benefit.  (It may be less for disability benefits.)  The");
  //  outputString.push_back(
  //    "family limit also applies to benefits for your survivors.");
  //  outputString.push_back("");
  //  outputString.push_back(
  //    "Your spouse cannot get both his or her own benefit plus a full");
  //  outputString.push_back(
  //    "benefit on your record.  We can only pay an amount equal to the");
  //  outputString.push_back(
  //    "larger of the two benefits.  Your spouse should call us and ask how");
  //  outputString.push_back(
  //    "to get a Social Security Statement like this.  When you both have");
  //  outputString.push_back(
  //    "statements, we can help estimate your spouse's future benefits on");
  //  outputString.push_back("the two records.");
  //} catch (PiaException&) {
  //  throw PiaException("Error in PebsOasdiInfo::prepareStrings");
  //}
}
