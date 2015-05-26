// $Id: pathname.h,v 1.8 2010/11/08 15:02:38 djolson Exp $
//
// Declarations for class to manage pathname of a file.

#ifndef __PATHNAME_H
#define __PATHNAME_H    1    // Prevent multiple #includes

#include <string>

// {group:Miscellaneous Classes}
/// <summary>Manages the pathname of a file.</summary>
///
/// <remarks>The pathname is stored in three parts, as well as a full
///  pathname.</remarks>
class Pathname
{
   public:
      /// <summary>Separator between directory names and file name.</summary>
      ///
      /// <remarks>This is determined at compile time to be either a backward
      /// slash (for Windows) or a forward slash (for anything else).</remarks>
      static const std::string separator;
      /// <summary>Separator between directory names and file name.</summary>
      ///
      /// <remarks>This is determined at compile time to be either a backward
      /// slash (for Windows) or a forward slash (for anything else).</remarks>
      static const char separatorChar;
   protected:
      /// <summary>Path to file.</summary>
      std::string pathname;
      /// <summary>Core file name.</summary>
      std::string corename;
      /// <summary>Filename extension.</summary>
      std::string extension;
      /// <summary>Full pathname.</summary>
      std::string fullname;
   public:
      Pathname();
      explicit Pathname( const std::string& newDirectory );
      std::string getCorename() const;
      std::string getExtension() const;
      std::string getFullname() const;
      std::string getPathname() const;
      void  setCorename( const std::string& newCorename );
      void  setCorename( int start, const std::string& newCorename );
      void  setExtension( const std::string& newExtension );
      void  setFullname( const std::string& newFullname );
      void  setPathname( const std::string& newPathname );
   protected:
      void  pastename();
};

/// <summary>Returns core filename.</summary>
///
/// <returns>Core filename.</returns>
inline std::string Pathname::getCorename() const
   { return(corename); }

/// <summary>Returns extension of filename.</summary>
///
/// <returns>Extension of filename.</returns>
inline std::string Pathname::getExtension() const
   { return(extension); }

/// <summary>Returns full filename.</summary>
///
/// <returns>Full filename.</returns>
inline std::string Pathname::getFullname() const
   { return(fullname); }

/// <summary>Returns path part of filename.</summary>
///
/// <returns>Path part of filename.</returns>
inline std::string Pathname::getPathname() const
   { return(pathname); }

/// <summary>Sets path leading to filename.</summary>
///
/// <param name="newPathname">New pathname.</param>
inline void Pathname::setPathname( const std::string& newPathname )
   { pathname = newPathname; pastename(); }

/// <summary>Sets core filename.</summary>
///
/// <remarks>The whole core filename is replaced unless a starting position is
///   specified.</remarks>
///
/// <param name="newCorename">New core filename.</param>
inline void Pathname::setCorename( const std::string& newCorename )
   { corename = newCorename; pastename(); }


/// <param name="start">Starting point of new core filename. New core filename starts
///     at position 0 if not specified.
/// <param name="newCorename">New core filename.</param>
inline void Pathname::setCorename( int start, const std::string& newCorename )
   { corename = corename.substr(0, start);
     corename += newCorename; pastename(); }

/// <summary>Sets extension of filename.</summary>
///
/// <param name="newExtension">New extension of filename.</param>
inline void Pathname::setExtension( const std::string& newExtension )
   { extension = newExtension; pastename(); }

/// <summary>Sets full filename from a string.</summary>
///
/// <param name="newFullname">New full filename.</param>
inline void Pathname::setFullname( const std::string& newFullname )
   { fullname = newFullname; }

#endif     // __PATHNAME_H
