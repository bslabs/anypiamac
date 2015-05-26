// Functions of the <see cref="AwbiDataFile"/> class to manage historical
// Social Security parameters stored in a file.
//
// $Id: awbidtfl.cpp 1.28 2011/07/28 10:16:27EDT 044579 Development  $

#include <sstream>
#include <fstream>
#include <iomanip>
#include "AwbiDataFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes historical parameters info.</summary>
///
/// <param name="newLastYear">Last year of wage bases.</param>
/// <param name="newMaxyear">Maximum projected year.</param>
/// <param name="newDirectory">Directory with historical parameters file.
/// </param>
AwbiDataFile::AwbiDataFile( int newLastYear, int newMaxyear,
const std::string& newDirectory ) :
AwbiData(newLastYear, newMaxyear),
pathname(Path::combine(newDirectory, "awbixxxxx.dat"))
{ }

/// <summary>Reads historical parameters.</summary>
///
/// <remarks>This version opens an input file and passes that file as an
/// argument to the version with one argument.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AWBI1"/> if unable to open input file.
/// <see cref="PiaException"/> of type <see cref="PIA_IDS_AWBI3"/> if unable
/// to read all required data.</exception>
void AwbiDataFile::read()
{
  ifstream in; // input stream
  in.open(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_AWBI1);
  read(in);
  in.close();
}

/// <summary>Reads historical parameters from an open file.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AWBI3"/> if unable to read all required data.
/// </exception>
///
/// <param name="in">An open input stream to read from.</param>
void AwbiDataFile::read( std::istream& in )
{
  double dtemp;  // temporary double
  // read benefit increases
  for (int year = cpiinc.getBaseYear(); year <= getLastYear() - 1; year++) {
    if (!(in >> dtemp))
      throw PiaException(PIA_IDS_AWBI3);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_AWBI3);
    cpiinc[year] = dtemp;
  }
  // read average wages
  for (int year = fq.getBaseYear(); year <= getLastYear() - 2; year++) {
    if (!(in >> dtemp))
      throw PiaException(PIA_IDS_AWBI3);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_AWBI3);
    fq[year] = dtemp;
  }
  // read wage bases
  for (int year = baseOasdi.getBaseYear(); year <= getLastYear(); year++) {
    if (!(in >> dtemp))
      throw PiaException(PIA_IDS_AWBI3);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_AWBI3);
    baseOasdi[year] = dtemp;
  }
  // HI bases equal OASDI bases through 1990
  baseHi.assign(baseOasdi, baseHi.getBaseYear(), HI_BASE_YEAR - 1);
  // read HI bases beginning 1991
  for (int year = HI_BASE_YEAR; year <= getLastYear(); year++) {
    if (!(in >> dtemp))
      throw PiaException(PIA_IDS_AWBI3);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_AWBI3);
    baseHi[year] = dtemp;
  }
  // read old-law bases
  for (int year = base77.getBaseYear(); year <= getLastYear(); year++) {
    if (!(in >> dtemp))
      throw PiaException(PIA_IDS_AWBI3);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_AWBI3);
    base77[year] = dtemp;
  }
}

/// <summary>Saves historical parameters.</summary>
///
/// <remarks>This version opens an output file and passes that file as an
/// argument to the version with one argument.</remarks>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_AWBI2"/> if unable to open output file.</exception>
void AwbiDataFile::write()
{
  ofstream out;  // output stream
  out.open(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_AWBI2);
  out.setf(ios::fixed, ios::floatfield);
  write(out);
  out.close();
}

/// <summary>Saves historical parameters to an open file.</summary>
///
/// <param name="out">Open output stream to write to.</param>
void AwbiDataFile::write( std::ostream& out )
{
  out.precision(1);
  for (int year = cpiinc.getBaseYear(); year <= getLastYear() - 1; year++)
    out << setw(4) << cpiinc[year] << endl;
  out.precision(2);
  for (int year = fq.getBaseYear(); year <= getLastYear() - 2; year++)
    out << setw(10) << fq[year] << endl;
  out.precision(2);
  for (int year = baseOasdi.getBaseYear(); year <= getLastYear(); year++)
    out << setw(10) << baseOasdi[year] << endl;
  out.precision(2);
  for (int year = HI_BASE_YEAR; year <= getLastYear(); year++)
    out << setw(10) << baseHi[year] << endl;
  out.precision(2);
  for (int year = base77.getBaseYear(); year <= getLastYear(); year++)
    out << setw(10) << base77[year] << endl;
}

/// <summary>Forms a file name for historical parameters file.</summary>
void AwbiDataFile::setAwbiFile()
{
  const string directoryName(Path::getDirectoryName(pathname));
  ostringstream coreName;
  coreName << "awbi" << getLastYear() << ".dat";
  pathname = Path::combine(directoryName, coreName.str());
}

/// <summary>Dummy function (descendant class may use it).</summary>
void AwbiDataFile::setData()
{ }
