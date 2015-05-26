// Functions for the <see cref="FieldOfficeInfoFile"/> class to manage field
// office information stored on disk.
//
// $Id: foinfofl.cpp 1.16 2011/07/29 15:26:17EDT 044579 Development  $

#include <fstream>
#include "FieldOfficeInfoFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes field office info.</summary>
///
/// <param name="newDirectory">Directory with field office file.</param>
FieldOfficeInfoFile::FieldOfficeInfoFile( const std::string& newDirectory ) :
FieldOfficeInfo(), pathname()
{
  pathname = Path::combine(newDirectory, "fo.dat");
}

/// <summary>Destroys field office info.</summary>
FieldOfficeInfoFile::~FieldOfficeInfoFile()
{ }

/// <summary>Reads field office information from disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FOINFO1"/> if unable to open input file; of type
/// <see cref="PIA_IDS_FOINFO2"/> if unable to read all required data.
/// </exception>
void FieldOfficeInfoFile::read()
{
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_FOINFO1);
  try {
    string line;  // input line
    getline(in, line);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_FOINFO2);
    setFieldOfficeName(line);
    getline(in, line);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_FOINFO2);
    setFieldOfficeTitle(line);
    for (int i1 = 0; i1 < numLines; i1++) {
      getline(in, line);
      if (in.eof() || in.fail())
        throw PiaException(PIA_IDS_FOINFO2);
      setAddress(i1, line);
    }
    in.close();
  } catch (exception&) {
    in.close();
    throw PiaException(PIA_IDS_FOINFO2);
  }
}

/// <summary>Saves field office information to disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_FOINFO3"/> if unable to open output file.</exception>
void FieldOfficeInfoFile::write()
{
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_FOINFO3);
  out << getFieldOfficeName() << endl;
  out << getFieldOfficeTitle() << endl;
  for (int i1 = 0; i1 < numLines; i1++)
    out << getAddress(i1) << endl;
  out.close();
}

/// <summary>Dummy function for file version.</summary>
void FieldOfficeInfoFile::setData()
{ }
