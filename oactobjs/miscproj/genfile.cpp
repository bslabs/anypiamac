// Functions for the <see cref="Genfile"/> class to manage a general file.
//
// $Id: genfile.cpp 1.15 2011/08/11 16:23:34EDT 044579 Development  $

#include <fstream>
#include "genfile.h"
#include "Resource.h"
#include "PiaException.h"

using namespace std;

/// <summary>Initializes the parent directory to "".</summary>
Genfile::Genfile() : pathname()
{ }

/// <summary>Sets the parent directory to the specified directory or
/// path name.</summary>
///
/// <param name="newDirectory">New parent directory or full path name.</param>
Genfile::Genfile( const std::string& newDirectory ) : pathname(newDirectory)
{ }

/// <summary>Destroys this instance.</summary>
Genfile::~Genfile()
{ }

/// <summary>Opens a file for output with fixed fields for floats.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_GENFILE1"/> if unable to open file.</exception>
///
/// <param name="out">Output stream.</param>
void Genfile::openout( std::ofstream& out )
{
  // open with group write permission
  // out.open(pathname.getfullname(), ios::out, 0664);
  out.open(pathname.c_str(), ios::out);
  if (!out.is_open()) {
    throw PiaException(PIA_IDS_GENFILE1);
  }
  out.setf(ios::fixed,ios::floatfield);
}

/// <summary>Opens a file for appended output with fixed fields for floats.
/// </summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_GENFILE2"/> if unable to open file.</exception>
///
/// <param name="out">Output stream.</param>
void Genfile::openapp( std::ofstream& out )
{
  out.open(pathname.c_str(), ios::app);
  if (!out.is_open()) {
    throw PiaException(PIA_IDS_GENFILE2);
  }
  out.setf(ios::fixed, ios::floatfield);
}

/// <summary>Opens a file for input.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_GENFILE3"/> if unable to open file.</exception>
///
/// <param name="in">Input stream.</param>
void Genfile::openin( std::ifstream& in )
{
  // in.open(pathname.getfullname().c_str(), ios::in | ios::nocreate);
  in.open(pathname.c_str(), ios::in);
  if (!in.is_open()) {
    throw PiaException(PIA_IDS_GENFILE3);
  }
}
