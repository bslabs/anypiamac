// Functions needed for <see cref="AwincFile"/> class to manage average wage
// increases stored on disk.

// $Id: awincfl.cpp 1.30 2010/11/22 16:15:25EST 044579 Development  $

#include <sstream>
#include <fstream>
#include "AwincFile.h"
#include "avgwg.h"
#include "Resource.h"
#include "PiaException.h"
#include "AssumptionType.h"
#include "Path.h"
#include "DoubleAnnualRW.h"

using namespace std;

/// <summary>Initializes projected average wage increase info.</summary>
///
/// <param name="newIstart">Starting year of average wage increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newDirectory">Directory with average wage increase file.
/// </param>
AwincFile::AwincFile( int newIstart, int newMaxyear,
const std::string& newDirectory ) : AwincDoc(newIstart, newMaxyear),
pathname(Path::combine(newDirectory, "awxxx.dat"))
{ }

/// <summary>Reads projected average wage increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FQINC4"/> if unable to open input file.
/// <see cref="PiaException"/> of type <see cref="PIA_IDS_FQINC3"/>
/// if unable to read all required data.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void AwincFile::read( int altNum )
{
  int temp;  // temporary istart
  string str;  // temporary string
  setAwFile(altNum);
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_FQINC4);
  /// <summary>read title</summary>
  getline(in, str);
  awincTitle = str;
  /// <summary>read first year</summary>
  if (!(in >> temp))
    throw PiaException(PIA_IDS_FQINC3);
  if (in.eof() || in.fail())
    throw PiaException(PIA_IDS_FQINC3);
  setFirstYear(temp);
  /// <summary>read all data</summary>
  try {
    DoubleAnnualRead awincRead(awinc);
    awincRead.read(in, getFirstYear(), awinc.getLastYear());
  } catch (exception&) {
    throw PiaException(PIA_IDS_FQINC3);
  }
  in.close();
}

/// <summary>Saves projected average wage increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FQINC2"/> if unable to open output file.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void AwincFile::write( int altNum )
{
  setAwFile(altNum);
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_FQINC2);
  out.setf(ios::fixed, ios::floatfield);
  /// <summary>write title</summary>
  out << awincTitle << endl;
  /// <summary>write first year of data</summary>
  out << getFirstYear() << endl;
  /// <summary>write all data</summary>
  DoubleAnnualWrite awincWrite(awinc, 10, 6, 75);
  awincWrite.write(out, getFirstYear(), awinc.getLastYear());
  out.close();
}

/// <summary>Forms a file name for projected average wage increase file.
/// </summary>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void AwincFile::setAwFile( int altNum )
{
  const string directoryName(Path::getDirectoryName(pathname));
  ostringstream coreName;
  coreName << "aw" << getFirstYear() << altNum << ".dat";
  pathname = Path::combine(directoryName, coreName.str());
}

/// <summary>Dummy function.</summary>
void AwincFile::setData( int )
{ }

/// <summary>Dummy function.</summary>
void AwincFile::setData( int, const DoubleAnnual& )
{ }

/// <summary>Sets title of assumptions.</summary>
///
/// <param name="altNum">Which set of assumptions.</param>
void AwincFile::setTitle( int altNum )
{
  if (AssumptionType::isNotTR(altNum)) {
    awincTitle = avgwage[altNum - AssumptionType::FLAT];
  }
}

/// <summary>Sets title of assumptions.</summary>
///
/// <param name="altNum">Which set of assumptions.</param>
void AwincFile::setTitle( int altNum, const std::string& )
{
  if (AssumptionType::isNotTR(altNum)) {
    awincTitle = avgwage[altNum - AssumptionType::FLAT];
  }
}
