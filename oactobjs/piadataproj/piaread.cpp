// Functions for the <see cref="PiaRead"/> class to read a file of
// worker data from disk.

// $Id: piaread.cpp 1.88 2011/08/08 09:16:42EDT 044579 Development  $

#include <stdexcept>
#include <fstream>
#include <iomanip>
#include "piaread.h"
#include "WorkerDataArray.h"
#include "PiadataArray.h"
#include "UserAssumptions.h"
#include "SecondaryArray.h"
#include "awinc.h"
#include "wbgenrl.h"
#include "oactcnst.h"
#include "PiaException.h"
#include "Resource.h"
#include "AssumptionType.h"
#include "DateFormatter.h"

using namespace std;

// <summary>Number of columns at beginning of each line reserved for
//   line number.</summary>
int PiaRead::width = 2;
// <summary>Number of columns for earnings and bases.</summary>
int PiaRead::earnWidth = 11;

/// <summary>Initializes references to data.</summary>
///
/// <param name="newWorkerData">Worker's calculation data.</param>
/// <param name="newWidowDataArray">Widow and other family members' basic data.
/// </param>
/// <param name="newWidowArray">Widow and other family members' calculation
///  data.</param>
/// <param name="newUserAssumptions">User-specified assumptions.</param>
/// <param name="newSecondaryArray">Array of <see cref="Secondary"/> benefit
/// information.</param>
PiaRead::PiaRead( WorkerData& newWorkerData,
WorkerDataArray& newWidowDataArray, PiaDataArray& newWidowArray,
UserAssumptions& newUserAssumptions, SecondaryArray& newSecondaryArray ) :
workerData(newWorkerData), widowDataArray(newWidowDataArray),
widowArray(newWidowArray), userAssumptions(newUserAssumptions),
secondaryArray(newSecondaryArray), inputLine(), unusedLine(false),
lineType(0)
{ }

/// <summary>Reads case from file.</summary>
///
/// <remarks>Call <see cref="WorkerDataGeneral::deleteContents"/> for the
/// worker and each family member before this function if reading more than one
/// case.</remarks>
///
/// <returns>Returns one of the following possible values:
///
/// * 0: successful read, with another record following.
/// * <see cref="PIA_IDS_READEOF"/> successful read, with end of file following.
/// * <see cref="PIA_IDS_READMORE"/> successful read, with additional data following (for subclass).
/// * Other positive value: unsuccessful read.
/// </returns>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_READERR"/> if error reading file.</exception>
///
/// <param name="in">Input stream.</param>
unsigned int PiaRead::read( std::istream& in )
{
  if (!unusedLine) {
    getline(in, inputLine);
    if (in.eof() || in.fail())
      return(PIA_IDS_READERR);
  }
  if (atoi(inputLine.substr(0, width).c_str()) != 1)
    return(PIA_IDS_READERR);
  parseSsn(inputLine.substr(width));
  // assume first there are no regular earnings, until some are read
  workerData.setIndearn(false);
  workerData.setMqge(false);

  // get next line(s), until a line type of 1
  do {
    getline(in, inputLine);
    if (in.eof() || inputLine.size() == 0) {
      unusedLine = false;
      if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
        userAssumptions.pebsasmCheck();
        setPebsData();
      }
      // extend user assumptions to end of period, in case benefit date
      // changes
      if (userAssumptions.getIaltbi() == AssumptionType::OTHER_ASSUM) {
        const int year = workerData.getBenefitDate().getYear();
        const double temp = userAssumptions.biproj[year];
        userAssumptions.biproj.assign(temp, year + 1,
          WorkerData::getMaxyear());
      }
      if (userAssumptions.getIaltaw() == AssumptionType::OTHER_ASSUM) {
        const int year = workerData.getBenefitDate().getYear();
        const double temp = userAssumptions.awincproj[year];
        userAssumptions.awincproj.assign(temp, year + 1,
          WorkerData::getMaxyear());
      }
      return(PIA_IDS_READEOF);
    }
    if (in.fail()) {
      unusedLine = false;
      return(PIA_IDS_READERR);
    }
    lineType = atoi(inputLine.substr(0, width).c_str());
    switch (lineType)
    {
    case 1:  // starting new record
      unusedLine = true;
      if (workerData.getJoasdi() == WorkerData::PEBS_CALC) {
        userAssumptions.pebsasmCheck();
        setPebsData();
      }
      return(0);
    case 2:  // date of death
      parseDeath(inputLine.substr(width));
      break;
    case 3:  // type of benefit
      parseTob(inputLine.substr(width));
      break;
    case 4:  // date of benefit
      parseBendate(inputLine.substr(width));
      break;
    case 5:  // pebes information
      parsePebes(inputLine.substr(width));
      break;
    case 6:  // years of earnings
      workerData.setIndearn(true);
      parseYears(inputLine.substr(width));
      break;
    case 7:  // backward projection amounts
      parseBack(inputLine.substr(width));
      break;
    case 8:  // forward projection amounts
      parseFwrd(inputLine.substr(width));
      break;
    case 9:  // disability information for most recent period
      parseDisab1(inputLine.substr(width));
      break;
    case 10:  // disability information for second most recent period
      parseDisab2(inputLine.substr(width));
      break;
    case 11:  // military service dates
      parseMsdates(inputLine.substr(width));
      break;
    case 12:  // noncovered pension
      parsePubpen(inputLine.substr(width));
      break;
    case 13:  // totalization indicator
      parseTotalize(inputLine.substr(width));
      break;
    case 14:  // blind indicator
      parseBlind(inputLine.substr(width));
      break;
    case 15:  // deemed insured indicator
      parseDeemed(inputLine.substr(width));
      break;
    case 16:  // worker's name
      parseNhname(inputLine.substr(width));
      break;
    case 17:  // worker's address
    case 18:
    case 19:
      parseNhaddr(lineType - 17, inputLine.substr(width));
      break;
    case 20:  // annual type of earnings
      parseEarnType(inputLine.substr(width));
      break;
    case 21:  // annual type of taxes
      parseTaxType(inputLine.substr(width));
      break;
    case 22:  // earnings
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
      parseEarnOasdi(lineType - 22, inputLine.substr(width));
      break;
    case 30:  // Medicare earnings
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      workerData.setMqge(true);
      parseEarnHi(lineType - 30, inputLine.substr(width));
      break;
    case 38:  // noncovered pension after military reservist pension removal
      parsePubpenReservist(inputLine.substr(width));
      break;
    case 39:  // oab entitlement before most recent dib
      parseOabent(inputLine.substr(width));
      break;
    case 40:  // assumption indicators
      parseAssump(inputLine.substr(width));
      break;
    case 41:  // projected benefit increases
    case 42:
    case 43:
    case 44:
      parseBi(lineType - 41, inputLine.substr(width));
      break;
    case 45:  // catch-up benefit increases
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
      parseCatchup(lineType - 45, inputLine.substr(width));
      break;
    case 55:  // title of projected benefit increases
      parseTitlebi(inputLine.substr(width));
      break;
    case 56:
    case 57:
    case 58:
    case 59:
      parseAw(lineType - 56, inputLine.substr(width));
      break;
    case 60:  // title of projected average wage increases
      parseTitleaw(inputLine.substr(width));
      break;
    case 61:  // parse OASDI bases
    case 62:
    case 63:
    case 64:
      parseBases(lineType - 61, inputLine.substr(width));
      break;
    case 65:  // parse old-law bases
    case 66:
    case 67:
    case 68:
      parseBases77(lineType - 65, inputLine.substr(width));
      break;
    case 69:  // family member information
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
    case 82:
    case 83:
      parseFamilyMember(lineType - 69, inputLine.substr(width));
      break;
    case 84:  // railroad qcs and earnings 1937 to 1950
      workerData.setIndrr(true);
      workerData.railRoadData.parse3750(inputLine.substr(width), earnWidth);
      break;
    case 85:  // first and last years of railroad earnings
      workerData.setIndrr(true);
      workerData.railRoadData.parseYears(inputLine.substr(width));
      break;
    case 86:  // annual quarters of coverage, 1951 to 1977
      workerData.railRoadData.parseQcs(inputLine.substr(width));
      break;
    case 87:  // first decade of earnings
    case 88:  // second decade of earnings
    case 89:  // third decade of earnings
    case 90:  // fourth decade of earnings
    case 91:  // fifth decade of earnings
    case 92:  // sixth decade of earnings
    case 93:  // seventh decade of earnings
    case 94:  // eighth decade of earnings
      workerData.railRoadData.parseEarnings(lineType - 87,
        inputLine.substr(width), earnWidth);
      break;
    case 95:  // summary quarters of coverage
      parseQc1(inputLine.substr(width));
      break;
    case 96:  // annual quarters of coverage
      parseQc2(inputLine.substr(width));
      break;
    case 97:  // annual child care years
      parseChildCareYears(inputLine.substr(width));
      break;
    default:
      return((lineType > 99) ? PIA_IDS_READMORE : PIA_IDS_READERR);
    }
  } while (lineType != 1);
  // should never reach here, due to return in switch statements
  return(PIA_IDS_READEOF);
}

/// <summary>Sets data for Social Security Statement.</summary>
///
/// <remarks>Calls <see cref="WorkerDataGeneral::setPebsData"/>.</remarks>
void PiaRead::setPebsData()
{
  workerData.setPebsData();
}

/// <summary>Parses Social Security number, sex, and date of birth.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseSsn( const std::string& line )
{
  try {
    workerData.ssn.setSsnFull(line.substr(0,9));
    workerData.setSex(
      static_cast<Sex::sex_type>(atoi(line.substr(9, 1).c_str())));
    const boost::gregorian::date dateModyyr =
      DateFormatter::date_from_undelimited_us_string(line.substr(10, 8));
    WorkerData::birth2Check(dateModyyr);
    workerData.setBirthDate(dateModyyr);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  } catch (PiaException& e) {
    if (e.getNumber() == PIA_IDS_DATEYEAR) {
      throw PiaException(PIA_IDS_BIRTH2);
    }
    if (e.getNumber() == PIA_IDS_DATEMONTH) {
      throw PiaException(PIA_IDS_BIRTH3);
    }
    if (e.getNumber() == PIA_IDS_DATEDAY) {
      throw PiaException(PIA_IDS_BIRTH4);
    }
    throw PiaException(e);
  }
}

/// <summary>Parses date of death.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseDeath( const std::string& line )
{
  try {
    boost::gregorian::date dateModyyr;
    // allow for file without day of death, from early versions
    if (line.size() > 6) {
      dateModyyr =
        DateFormatter::date_from_undelimited_us_string(line.substr(0, 8));
    }
    else {
      DateMoyr dateMoyr =
        DateFormatter::month_year_from_undelimited_us_string(
        line.substr(0, 6));
      dateModyyr = boost::gregorian::date(
        static_cast<unsigned short>(dateMoyr.getYear()),
        static_cast<unsigned short>(dateMoyr.getMonth()), 1);
    }
    workerData.setDeathDate(dateModyyr);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses type of benefit and date of entitlement.</summary>
///
/// <remarks>The date of entitlement is ignored in a survivor case. The date
/// of benefit is set equal to date of entitlement. This will be overwritten
/// if there is a line 4.</remarks>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseTob( const std::string& line )
{
  try {
    workerData.setJoasdi(atoi(line.substr(0, 1).c_str()));
    // do not use date of entitlement in a survivor case
    if (workerData.getJoasdi() != WorkerData::SURVIVOR) {
      DateMoyr dateMoyr =
        DateFormatter::month_year_from_undelimited_us_string(
        line.substr(1, 6));
      workerData.setEntDate(dateMoyr);
      workerData.setBenefitDate();
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses date of benefit.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseBendate( const std::string& line )
{
  try {
    workerData.setRecalc(true);
    DateMoyr dateMoyr =
      DateFormatter::month_year_from_undelimited_us_string(
      line.substr(0, 6));
    workerData.setBenefitDate(dateMoyr);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses Social Security Statement information.</summary>
///
/// <remarks>This version does nothing.</remarks>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parsePebes( const std::string& )
{ }

/// <summary>Parses years of earnings.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseYears( const std::string& line )
{
  try {
    const int year1 = atoi(line.substr(0, 4).c_str());
    workerData.setIbegin(year1);
    workerData.ibeginCheck();
    const int year2 = atoi(line.substr(4, 4).c_str());
    workerData.setIend(year2);
    setEarnProjectYears(year1, year2);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Sets the years in the <see cref="EarnProject"/> class.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaRead::setEarnProjectYears( int, int )
{ }

/// <summary>Parses backward projection amounts.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaRead::parseBack( const std::string& )
{ }

/// <summary>Parses forward projection amounts.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaRead::parseFwrd( const std::string& )
{ }

/// <summary>Parses disability information for most recent period.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseDisab1( const std::string& line )
{
  try {
    // fill in a temporary DisabPeriod
    DisabPeriod disabPeriod;
    disabPeriod.parseString(line);
    // set date of onset
    workerData.setOnsetDate(0, disabPeriod.getOnsetDate());
    workerData.setValdi(1);
    workerData.disCheck();
    // set date of prior entitlement
    if (workerData.getJoasdi() == WorkerData::NO_BEN) {
      workerData.setPriorentDate(0, disabPeriod.getEntDate());
    }
    else {
      workerData.setPriorentDateCheck(0, disabPeriod.getEntDate());
      workerData.priorentCheck();
    }
    // set first month of waiting period
    //if (workerData.getJoasdi() == WorkerData::DISABILITY ||
    //    workerData.getJoasdi() == WorkerData::NO_BEN) {
      DisabPeriod::waitperDateCheck(disabPeriod.getWaitperDate());
      workerData.setWaitperDate(0, disabPeriod.getWaitperDate());
      if (workerData.getJoasdi() == WorkerData::DISABILITY)
        workerData.waitpdCheck();
    //}
    if (workerData.getJoasdi() != WorkerData::DISABILITY) {
      // set month and year of disability cessation
      if (disabPeriod.getCessationDate().getYear() != 0 ||
        workerData.getJoasdi() != WorkerData::NO_BEN) {
        DisabPeriod::cessationDateCheck(disabPeriod.getCessationDate());
        workerData.setCessationDate(0, disabPeriod.getCessationDate());
        workerData.setCessationPia(0, disabPeriod.getCessationPia());
        workerData.setCessationMfb(0, disabPeriod.getCessationMfb());
      }
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses disability information for second most recent period.
/// </summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseDisab2( const std::string& line )
{
  try {
    // fill in a temporary DisabPeriod
    DisabPeriod disabPeriod;
    disabPeriod.parseString(line);
    // set date of onset
    workerData.setOnsetDate(1, disabPeriod.getOnsetDate());
    workerData.setValdi(2);
    workerData.dis1Check();
    // set date of prior entitlement
    if (workerData.getJoasdi() == WorkerData::NO_BEN) {
      workerData.setPriorentDate(1, disabPeriod.getEntDate());
    }
    else {
      workerData.setPriorentDateCheck(1, disabPeriod.getEntDate());
      workerData.priorent1Check();
    }
    // set first month of prior waiting period
    workerData.setWaitperDate(1, disabPeriod.getWaitperDate());
    // set month and year of prior disability cessation
    if (disabPeriod.getCessationDate().getYear() != 0 ||
      workerData.getJoasdi() != WorkerData::NO_BEN) {
      DisabPeriod::cessationDateCheck(disabPeriod.getCessationDate());
      workerData.setCessationDate(1, disabPeriod.getCessationDate());
      workerData.setCessationPia(1, disabPeriod.getCessationPia());
      workerData.setCessationMfb(1, disabPeriod.getCessationMfb());
    }
    if (workerData.getJoasdi() == WorkerData::DISABILITY) {
      workerData.waitpd1Check();
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses family member's information.</summary>
///
/// <param name="lineNumber">Family member number (0 to 14).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseFamilyMember( int lineNumber, const std::string& line )
{
  try {
    widowArray.setFamSize(lineNumber + 1);
    Secondary * secondaryptr = secondaryArray.secondary[lineNumber];
    WorkerData * widowDataptr = widowDataArray.workerData[lineNumber];
    // parse bic code
    secondaryptr->bic.set(line.substr(0,2));
    // parse date of birth
    boost::gregorian::date dateModyyr =
      DateFormatter::date_from_undelimited_us_string(line.substr(2, 8));
    WorkerData::birth2Check(dateModyyr);
    widowDataptr->setBirthDate(dateModyyr);
    // parse date of entitlement
    DateMoyr dateMoyr =
      DateFormatter::month_year_from_undelimited_us_string(
      line.substr(10, 6));
    secondaryptr->entDate = dateMoyr;
    // parse widow(er)'s date of onset
    if (secondaryptr->bic.getMajorBic() == 'W') {
      dateModyyr =
        DateFormatter::date_from_undelimited_us_string(line.substr(16, 8));
      widowDataptr->setOnsetDate(0, dateModyyr);
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses noncovered pension.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parsePubpen( const std::string& line )
{
  try {
    workerData.setPubpen(
      static_cast<float>(atof(line.substr(0, 10).c_str())));
    if (line.length() == 16) {
      DateMoyr dateMoyr =
        DateFormatter::month_year_from_undelimited_us_string(
        line.substr(10, 6));
      workerData.setPubpenDate(dateMoyr);
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses noncovered pension after military reservist pension
/// removal.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parsePubpenReservist( const std::string& line )
{
  workerData.setReservist(true);
  workerData.setPubpenReservist(
    static_cast<float>(atof(line.substr(0, 10).c_str())));
}

/// <summary>Parses totalization indicator.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseTotalize( const std::string& line )
{
  workerData.setTotalize(
    (atoi(line.substr(0, 1).c_str()) > 0) ? true : false);
}

/// <summary>Parses blind indicator.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseBlind( const std::string& line )
{
  workerData.setBlindind(
    (atoi(line.substr(0, 1).c_str()) > 0) ? true : false);
}

/// <summary>Parses deemed insured indicator.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseDeemed( const std::string& line )
{
  workerData.setDeemedind(
    (atoi(line.substr(0, 1).c_str()) > 0) ? true : false);
}

/// <summary>Parses worker's name.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseNhname( const std::string& line )
{
  workerData.setNhname(line);
}

/// <summary>Parses worker's address.</summary>
///
/// <param name="lineNumber">Address line number (0-2).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseNhaddr( int lineNumber, const std::string& line )
{
  workerData.setNhaddr(lineNumber, line);
}

/// <summary>Parses up to 20 projected benefit increases.</summary>
///
/// <param name="lineNumber">Number of increase line (0-3)</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseBi( int lineNumber, const std::string& line )
{
  try {
    userAssumptions.istartCheck(istart2);
    const int firstyear = istart2 + 20 * lineNumber;
    const int lastyear = min(firstyear + 19,
      static_cast<int>(workerData.getBenefitDate().getYear()));
    for (int yr = firstyear; yr <= lastyear; yr++) {
      double tempd = atof(line.substr(4 * (yr - firstyear), 4).c_str());
      Biproj::cpiincCheck(tempd);
      userAssumptions.biproj[yr] = tempd;
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses 8 catch-up benefit increases.</summary>
///
/// <param name="lineNumber">Line number of increases (0-9).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseCatchup( int lineNumber, const std::string& line )
{
  try {
    CatchupIncreases catchupIncreases;
    catchupIncreases.parseString(line.substr(4));
    const int cstart = userAssumptions.catchup.getCstart();
    for (int i1 = 0; i1 < 8; i1++) {
      userAssumptions.catchup.set(cstart + lineNumber,
        cstart + 2 + i1, catchupIncreases.get(i1));
    }
    userAssumptions.setAnscch('Y');
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses title of projected benefit increases.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseTitlebi( const std::string& line )
{
  userAssumptions.setTitleBi(line);
}

/// <summary>Parses title of projected average wage increases.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseTitleaw( const std::string& line )
{
  userAssumptions.setTitleAw(line);
}

/// <summary>Parses projected average wage increases.</summary>
///
/// <param name="lineNumber">Number of increase line (0-3).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseAw( int lineNumber, const std::string& line )
{
  try {
    userAssumptions.istartCheck(istart2 - 1);
    const int firstyear = istart2 - 1 + 20 * lineNumber;
    const int lastyear = min(firstyear + 19,
      static_cast<int>(workerData.getBenefitDate().getYear()));
    for (int yr = firstyear; yr <= lastyear; yr++) {
      const double tempd =
        atof(line.substr(6 * (yr - firstyear), 6).c_str());
      Awinc::fqincCheck(tempd);
      userAssumptions.awincproj[yr] = tempd;
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses type of earnings vector.</summary>
///
/// <remarks>This version does nothing.</remarks>
void PiaRead::parseEarnType( const std::string& )
{ }

/// <summary>Parses OASDI-covered earnings for up to 10 years.</summary>
///
/// <remarks>This version does not have any projection.</remarks>
///
/// <param name="lineNumber">Number of earnings line (0-7).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseEarnOasdi( int lineNumber, const std::string& line )
{
  const int firstYear = workerData.getIbegin() + 10 * lineNumber;
  const int lastYear = min(firstYear + 9, workerData.getIend());
  try {
    for (int yr = firstYear; yr <= lastYear; yr++) {
      workerData.setEarnOasdi(yr, atof(
        line.substr(earnWidth * (yr - firstYear), earnWidth).c_str()));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
  if ((int)line.length() < earnWidth * (lastYear - firstYear + 1)) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses assumption indicators.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseAssump( const std::string& line )
{
  try {
    istart2 = atoi(line.substr(0, 4).c_str());
    userAssumptions.istartCheck(istart2);
    userAssumptions.setIaltbi(atoi(line.substr(4, 1).c_str()));
    userAssumptions.setIaltaw(atoi(line.substr(5, 1).c_str()));
    // parse base change indicator
    userAssumptions.setIbasch(atoi(line.substr(6, 1).c_str()));
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses up to 20 OASDI wage bases.</summary>
///
/// <param name="lineNumber">Number of earnings line (0-3).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseBases( int lineNumber, const std::string& line )
{
  try {
    userAssumptions.istartCheck(istart2 + 1);
    userAssumptions.setJbasch(BaseChangeType::NONE);
    const int firstyear = istart2 + 1 + 20 * lineNumber;
    for (int yr = firstyear; yr <= min(firstyear + 19,
      static_cast<int>(workerData.getBenefitDate().getYear())); yr++) {
      const double temp =
        atof(line.substr(earnWidth * (yr - firstyear), earnWidth).c_str());
      WageBaseGeneral::check(temp);
      userAssumptions.baseOasdi[yr] = temp;
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses up to 20 old-law wage bases.</summary>
///
/// <param name="lineNumber">Number of earnings line (0-3).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseBases77( int lineNumber, const std::string& line )
{
  try {
    userAssumptions.istartCheck(istart2 + 1);
    userAssumptions.setJbasch(BaseChangeType::NONE);
    const int firstyear = istart2 + 1 + 20 * lineNumber;
    for (int yr = firstyear; yr <= min(firstyear + 19,
      static_cast<int>(workerData.getBenefitDate().getYear())); yr++) {
      const double temp =
        atof(line.substr(earnWidth * (yr - firstyear), earnWidth).c_str());
      WageBaseGeneral::check(temp);
      userAssumptions.base77[yr] = temp;
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses up to 10 years of Medicare earnings.</summary>
///
/// <param name="lineNumber">Number of earnings line (0-7).</param>
/// <param name="line">Line to parse.</param>
void PiaRead::parseEarnHi( int lineNumber, const std::string& line )
{
  try {
    const int firstyear = workerData.firstEarnHiYear() + 10 * lineNumber;
    const int lastyear = min(firstyear + 9, workerData.getIend());
    for (int yr = firstyear; yr <= lastyear; yr++) {
      workerData.setEarnHi(yr, atof(line.substr(earnWidth *
        (yr - firstyear), earnWidth).c_str()));
    }
    workerData.setMqge(true);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses type of taxes.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseTaxType( const std::string& line )
{
  try {
    const int firstyear = workerData.getIbegin();
    const int lastyear = workerData.getIend();
    for (int yr = firstyear; yr <= lastyear; yr++) {
      workerData.setTaxType(yr,
        atoi(line.substr(yr - firstyear, 1).c_str()));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses summary quarters of coverage.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseQc1( const std::string& line )
{
  try {
    workerData.setQctd(atoi(line.substr(0, 3).c_str()));
    workerData.setQc51td(atoi(line.substr(3, 3).c_str()));
    workerData.qctdCheck2();
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses annual quarters of coverage.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseQc2( const std::string& line )
{
  try {
    workerData.setQcsByYear(true);
    const int firstyear = workerData.getIbegin();
    const int lastyear = workerData.lastQcyr();
    for (int yr = firstyear; yr <= lastyear; yr++) {
      workerData.qc.set(yr, static_cast<unsigned>(atoi(
        line.substr(yr - firstyear, 1).c_str())));
    }
    workerData.setQctd(workerData.qc.accumulate(YEAR37, lastyear, 0u));
    workerData.setQc51td(workerData.qc.accumulate(YEAR51, lastyear, 0u));
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses military service dates.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseMsdates( const std::string& line )
{
  try {
    // there are 12 characters per military service date pair
    workerData.milServDatesVec.setMSCount(
      static_cast<unsigned int>(line.size() / 12));
    for (unsigned i = 0; i < workerData.milServDatesVec.getMSCount(); i++) {
      // first check dates in a temporary military service dates
      MilServDates msDates;
      msDates.parseDates(line.substr(12 * i));
      MilServDates::startDateCheck(msDates.startDate);
      MilServDates::endDateCheck(msDates.endDate);
      msDates.check();
      // if ok (no exception thrown), put into permanent dates
      workerData.milServDatesVec.msdates[i].parseDates(line.substr(12 * i));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses oab entitlement prior to most recent disability.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseOabent( const std::string& line )
{
  try {
    // parse date of oab entitlement
    DateMoyr dateMoyr =
      DateFormatter::month_year_from_undelimited_us_string(
      line.substr(0, 6));
    workerData.setOabEntDate(dateMoyr);
    // parse month and year of oab cessation
    dateMoyr = DateFormatter::month_year_from_undelimited_us_string(
      line.substr(6, 6));
    workerData.setOabCessDate(dateMoyr);
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}

/// <summary>Parses annual child care years.</summary>
///
/// <param name="line">Line to parse.</param>
void PiaRead::parseChildCareYears( const std::string& line )
{
  try {
    const int firstyear = workerData.getIbegin();
    const int lastyear = workerData.getIend();
    for (int yr = firstyear; yr <= lastyear; yr++) {
      workerData.childCareYears.setBit(yr,
        atoi(line.substr(yr - firstyear, 1).c_str()));
    }
  } catch (out_of_range&) {
    throw PiaException(PIA_IDS_READERR);
  }
}
