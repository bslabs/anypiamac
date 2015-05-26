// Functions for the <see cref="BaseYearFile"/> class to manage the
// computation year on disk.
//
// $Id: baseyrfl.cpp 1.16 2011/07/28 11:50:15EDT 044579 Development  $

#include <fstream>
#include "BaseYearFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes base year info.</summary>
///
/// <param name="newDirectory">Directory with base year file.</param>
BaseYearFile::BaseYearFile( const std::string& newDirectory ) :
BaseYear(), pathname()
{
  pathname = Path::combine(newDirectory, "baseyear.dat");
}

/// <summary>Reads base year from disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASEYR4"/> if unable to open input file.
/// <see cref="PiaException"/> of type <see cref="PIA_IDS_BASEYR3"/> if unable
/// to read base year.</exception>
void BaseYearFile::read()
{
  int temp;  // temporary integer
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_BASEYR4);
  in >> temp;
  setYear(temp);
  if (in.eof() || in.fail())
    throw PiaException(PIA_IDS_BASEYR3);
  in.close();
}

/// <summary>Saves base year to disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_BASEYR2"/> if unable to open output file.</exception>
void BaseYearFile::write()
{
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_BASEYR2);
  out.setf(ios::fixed, ios::floatfield);
  out << getYear() << endl;
  out.close();
}
