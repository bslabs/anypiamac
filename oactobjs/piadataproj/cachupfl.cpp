// Functions for the <see cref="CatchupFile"/> class to manage file of
// catch-up benefit increases.
//
// $Id: cachupfl.cpp 1.22 2011/07/29 13:49:27EDT 044579 Development  $

#include <sstream>
#include <fstream>
#include <cmath>
#include "CatchupFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "AssumptionType.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes catch-up benefit increase file.</summary>
///
/// <param name="newCstart">Starting year of catch-up benefit increases.
/// </param>
/// <param name="newDirectory">Directory with catch-up benefit increases file.
/// </param>
CatchupFile::CatchupFile( int newCstart, const std::string& newDirectory ) :
CatchupDoc(newCstart), pathname(Path::combine(newDirectory, "cuxxxxx.dat"))
{ }

/// <summary>Destroys catch-up benefit increase file.</summary>
CatchupFile::~CatchupFile()
{ }

/// <summary>Reads all catch-up increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CACHUP2"/> if unable to open input file.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void CatchupFile::read( int altNum )
{
  string inputLine;
  setCuFile(altNum);
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open()) {
    throw PiaException(PIA_IDS_CACHUP2);
  }
  for (int i = 0; i < Catchup::NUM_CATCHUP_ELIG_YEARS; i++) {
    getline(in, inputLine);
    catchup.parseString(i, inputLine);
  }
  in.close();
}

/// <summary>Writes all catch-up increases.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CACHUP3"/> if unable to open output file.</exception>
///
/// <param name="altNum">Desired alternative number (1-4).</param>
void CatchupFile::write( int altNum )
{
  setCuFile(altNum);
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open()) {
    throw PiaException(PIA_IDS_CACHUP3);
  }
  out.setf(ios::fixed, ios::floatfield);
  for (int i2 = 0; i2 < Catchup::NUM_CATCHUP_ELIG_YEARS; i2++) {
    out << catchup.getCatchupIncreases(i2).toString() << endl;
  }
  out.close();
}

/// <summary>Forms a file name for projected catch-up benefit
///  increase file.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void CatchupFile::setCuFile( int altNum )
{
  const string directoryName(Path::getDirectoryName(pathname));
  ostringstream coreName;
  coreName << "cu" + catchup.getCstart() << altNum << ".dat";
  pathname = Path::combine(directoryName, coreName.str());
}

/// <summary>Deletes current catch-up increases if other than TR
///  assumption.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void CatchupFile::setData( int altNum )
{
  if (AssumptionType::isNotTR(altNum)) {
    catchup.deleteContents();
  }
}

/// <summary>Deletes current catch-up increases if other than TR
///  assumption.</summary>
///
/// <param name="altNum">Desired alternative number (1-7).</param>
void CatchupFile::setData( int altNum, const Catchup& )
{
  if (AssumptionType::isNotTR(altNum)) {
    catchup.deleteContents();
  }
}
