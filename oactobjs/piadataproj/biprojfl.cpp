// Declarations for the <see cref="BiprojFile"/> class to manage projected
// benefit increases stored on disk.
//
// $Id: biprojfl.cpp 1.30 2011/07/29 11:25:03EDT 044579 Development  $

#include <sstream>
#include <fstream>
#include <iomanip>
#include "BiprojFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "AssumptionType.h"
#include "Path.h"
#include "DoubleAnnualRW.h"

using namespace std;

/// <summary>Initializes projected benefit increase info.</summary>
///
/// <param name="newIstart">Starting year of benefit increases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newDirectory">Directory with benefit increase file.</param>
BiprojFile::BiprojFile( int newIstart, int newMaxyear,
const std::string& newDirectory ) : Biproj(newIstart, newMaxyear),
pathname(Path::combine(newDirectory, "bixxx.dat"))
{ }

/// <summary>Reads projected benefit increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CPIINC4"/> if unable to open input file.
/// <see cref="PiaException"/> of type <see cref="PIA_IDS_CPIINC3"/> if unable
/// to read all required data.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void BiprojFile::read( int altNum )
{
  int temp;  // temporary istart
  string str;
  setBiFile(altNum);
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_CPIINC4);
  /// <summary>read title</summary>
  getline(in, str);
  biprojTitle = str;
  /// <summary>read first year</summary>
  if (!(in >> temp))
    throw PiaException(PIA_IDS_CPIINC3);
  if (in.eof() || in.fail())
    throw PiaException(PIA_IDS_CPIINC3);
  setFirstYear(temp);
  /// <summary>read all data</summary>
  try {
    DoubleAnnualRead dataRead(theData);
    dataRead.read(in, getFirstYear(), theData.getLastYear());
  } catch (exception&) {
    throw PiaException(PIA_IDS_CPIINC3);
  }
  in.close();
}

/// <summary>Saves projected benefit increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CPIINC2"/> if unable to open output file.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void BiprojFile::write( int altNum )
{
  setBiFile(altNum);
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_CPIINC2);
  out.setf(ios::fixed, ios::floatfield);
  /// <summary>write title</summary>
  out << biprojTitle << endl;
  /// <summary>write first year of data</summary>
  out << getFirstYear() << endl;
  /// <summary>write all data</summary>
  DoubleAnnualWrite dataWrite(theData, 6, 1, 75);
  dataWrite.write(out, getFirstYear(), theData.getLastYear());
  out.close();
}

/// <summary>Forms a file name for projected benefit increase file.</summary>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void BiprojFile::setBiFile( int altNum )
{
  const string directoryName(Path::getDirectoryName(pathname));
  ostringstream coreName;
  coreName << "bi" << getFirstYear() << altNum << ".dat";
  pathname = Path::combine(directoryName, coreName.str());
}

/// <summary>Dummy function.</summary>
void BiprojFile::setData( int )
{ }

/// <summary>Dummy function.</summary>
void BiprojFile::setData( int, const DoubleAnnual& )
{ }

/// <summary>Sets title of assumptions if not gotten from disk.</summary>
///
/// <param name="altNum">Which set of assumptions.</param>
void BiprojFile::setTitle( int altNum )
{
  if (AssumptionType::isNotTR(altNum)) {
    biprojTitle = beninc[altNum - AssumptionType::FLAT];
  }
}

/// <summary>Sets title of assumptions as gotten from disk.</summary>
///
/// <param name="altNum">Which set of assumptions.</param>
void BiprojFile::setTitle( int altNum, const std::string& )
{
  if (AssumptionType::isNotTR(altNum)) {
    biprojTitle = beninc[altNum - AssumptionType::FLAT];
  }
}
