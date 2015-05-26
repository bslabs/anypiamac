// Declarations for the <see cref="Pib50Mfb"/>, <see cref="Pib50Pib"/>,
// <see cref="Pib52Ame"/>, <see cref="Pib52Mfb"/>, <see cref="Pib54Ame"/>,
// <see cref="Pib54Mfb"/>, <see cref="Pib54Pia"/>, <see cref="Pib58Ame"/>,
// and <see cref="Pib58Pib"/> classes to manage pib-pia conversion tables.
//
// $Id: pibtable.h 1.9 2011/08/08 08:45:13EDT 044579 Development  $

#pragma once

/// <summary>Manages the 1950 pib-mfb conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib50Mfb
{
private:
  /// <summary>The 1950 mfb's.</summary>
  static const double mfb50[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1950 pib-pia conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib50Pib
{
private:
  /// <summary>The 1950 pib's.</summary>
  static const double pib50[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1952 pib-ame conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib52Ame
{
private:
  /// <summary>The 1952 Ame's.</summary>
  static const double ame52[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1952 pib-mfb conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib52Mfb
{
private:
  /// <summary>The 1952 mfb's.</summary>
  static const double mfb52[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1954 pib-ame conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib54Ame
{
private:
  /// <summary>The 1954 ame's.</summary>
  static const double ame54[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1954 pib-mfb conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib54Mfb
{
private:
  /// <summary>The 1954 mfb's.</summary>
  static const double mfb54[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1954 pib-pia conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::oldStart50Cal"/>.
/// </remarks>
class Pib54Pia
{
private:
  /// <summary>The 1954 pia's.</summary>
  static const double pia54[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1958 pib-ame conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::calculate"/>.
/// </remarks>
class Pib58Ame
{
private:
  /// <summary>The 1958 ame's.</summary>
  static const double ame58[];
public:
  static double getAt( int index );
};

/// <summary>Manages the 1958 pib-pia conversion figures.</summary>
///
/// <remarks>This class is used in <see cref="OldStart::calculate"/>.
/// </remarks>
class Pib58Pib
{
private:
  /// <summary>The 1958 pib's.</summary>
  static const double pib58[];
public:
  static double getAt( int index );
};
