// Functions for the <see cref="ConfigFile"/> class to manage configuration
// information used by the PIA calculation program stored on disk.
//
// $Id: configFile.cpp 1.16 2011/07/29 13:49:27EDT 044579 Development  $

#include <fstream>
#include "ConfigFile.h"
#include "Resource.h"
#include "PiaException.h"
#include "Path.h"

using namespace std;

/// <summary>Initializes configuration.</summary>
///
/// <param name="newDirectory">New directory to use.</param>
ConfigFile::ConfigFile( const std::string& newDirectory ) : Config(),
pathname()
{
  pathname = Path::combine(newDirectory, "config.dat");
}

/// <summary>Destroys configuration.</summary>
ConfigFile::~ConfigFile( )
{ }

/// <summary>Reads the configuration from disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CONFIG1"/> if unable to open input file; of type
/// <see cref="PIA_IDS_CONFIG6"/> if unable to read all required data.
/// </exception>
void ConfigFile::read()
{
  ifstream in(pathname.c_str(), ios::in);
  if (!in.is_open())
    throw PiaException(PIA_IDS_CONFIG1);
  try {
    string temp;  // temporary string
    getline(in, temp);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_CONFIG6);
    bool longOutputt = (atoi(temp.c_str()) > 0) ? true : false;
    setLongOutput(longOutputt);
    getline(in, temp);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_CONFIG6);
    int screenPointst = atoi(temp.c_str());
    setScreenPoints(screenPointst);
    getline(in, temp);
    if (in.eof() || in.fail())
      throw PiaException(PIA_IDS_CONFIG6);
    int printerPointst = atoi(temp.c_str());
    setPrinterPoints(printerPointst);
    in.close();
  } catch (exception&) {
    in.close();
    throw PiaException(PIA_IDS_CONFIG6);
  }
}

/// <summary>Saves the configuration to disk.</summary>
///
/// <exception cref="PiaException"><see cref="PiaException"/> of type
/// <see cref="PIA_IDS_CONFIG5"/> if unable to open output file.</exception>
void ConfigFile::write()
{
  ofstream out(pathname.c_str(), ios::out);
  if (!out.is_open())
    throw PiaException(PIA_IDS_CONFIG5);
  out.setf(ios::fixed, ios::floatfield);
  out << (getLongOutput() ? 1 : 0) << endl;
  out << getScreenPoints() << endl;
  out << getPrinterPoints() << endl;
  out.close();
}
