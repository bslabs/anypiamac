// Declarations for the <see cref="DibGuar"/> class to manage disability
// guarantee pia calculations.
//
// $Id: DibGuar.h 1.7 2011/07/29 15:26:18EDT 044579 Development  $

#pragma once

#include "PiaMethod.h"

/// <summary>Manages the functions required for calculation of a disability
/// guarantee Social Security benefit.</summary>
///
/// <remarks>This method is used when a person has recovered from one period of
/// disability and is claiming a subsequent benefit (old-age, disability, or
/// death). An instance of this class is created in <see cref="PiaCalPL"/>.
/// </remarks>
class DibGuar : public PiaMethod
{
public:
  /// <summary>The type of mfb at cessation, after conversion.</summary>
  ///
  /// <remarks>The type of mfb first depends on the date of entitlement to the
  /// subsequent benefit. If it is January 1996 or later, then the type is
  /// <see cref="ConvertedMfbType::POST1995_NOCHANGE"/> if the new benefit is a
  /// disability benefit, or if the Disability Maximum does not apply. The type
  /// is <see cref="ConvertedMfbType::POST1995_DECONVERTED"/> if it is an
  /// old-age or survivor benefit and the Disability Maximum applies.
  ///
  /// If the date of entitlement to the subsequent benefit is before January
  /// 1996, then the type depends on if the prior DIB entitlement continued
  /// in the 12 months preceding the new entitlement. If so, the type is
  /// <see cref="ConvertedMfbType::PRE1996_PRE1979_LAST12"/> if eligibility to
  /// the prior DIB benefit was before 1979, or
  /// <see cref="ConvertedMfbType::PRE1996_POST1978_LAST12"/> if eligibility to
  /// the prior DIB benefit was after 1978. Otherwise, the type is
  /// <see cref="ConvertedMfbType::PRE1996_NOTLAST12"/>.</remarks>
  enum ConvertedMfbType {
    /// <summary>Subsequent entitlement in January 1996 or later, new
    /// benefit is DI, or the Disability Maximum does not apply
    /// (no conversion required).</summary>
    POST1995_NOCHANGE = 0,
    /// <summary>Subsequent entitlement in January 1996 or later,
    /// new benefit is OASI, and Disability Maximum applies (requires
    /// deconversion to prior eligibility year).</summary>
    POST1995_DECONVERTED,
    /// <summary>Subsequent entitlement before January 1996, prior DIB
    /// continued in the 12 months preceding the new entitlement, prior
    /// DIB eligibility was before 1979.</summary>
    PRE1996_PRE1979_LAST12,
    /// <summary>Subsequent entitlement before January 1996, prior DIB
    /// continued in the 12 months preceding the new entitlement, prior
    /// DIB eligibility was after 1978.</summary>
    PRE1996_POST1978_LAST12,
    /// <summary>Subsequent entitlement before January 1996, prior DIB did
    /// not continue in the 12 months preceding the new entitlement.</summary>
    PRE1996_NOTLAST12
  };
private:
  /// <summary>True if colas apply from cessation date of prior
  /// disability to new entitlement date.</summary>
  bool colasApply;
  /// <summary>True if mfb at cessation date must be refigured by
  /// deconverting the pia at cessation date to onset date, calculating
  /// raw mfb, and converting forward to cessation date.</summary>
  ConvertedMfbType convertedMfbType;
  /// <summary>True if the mfb from the prior disability was determined
  /// using the Disability Maximum method.</summary>
  bool diMaxApplies;
  /// <summary>Which disability period to use (0 for most recent, 1 for
  /// next most recent).</summary>
  int whichPeriod;
  /// <summary>The pia deconverted from cessation date to eligibility date.
  /// </summary>
  double rawPia;
  /// <summary>The mfb based on the deconverted pia at eligibility date.
  /// </summary>
  double rawMfb;
  /// <summary>The original mfb at the cessation date.</summary>
  ///
  /// <remarks>This could be based on the Disability Maximum, and so may
  /// need to be converted (see <see cref="cessationMfbConv"/>).</remarks>
  double cessationMfbOrig;
  /// <summary>The mfb based on the deconverted pia brought up to the
  /// cessation date.</summary>
  double cessationMfbConv;
  /// <summary>The date of entitlement in a life case, or date of death
  /// in a survivor case.</summary>
  DateMoyr entDeathDate;
  /// <summary>Portion of AIME PIA in each interval of MFB formula.</summary>
  double portionPiaElig[4];
  /// <summary>Bend points for MFB formula.</summary>
  BendMfb bendMfb;
  /// <summary>Wage-indexed MFB formula percentages.</summary>
  PercMfb percMfb;
  /// <summary>Year of eligibility for prior DIB benefit.</summary>
  int eligYear;
protected:
  double cessationMfbCal( const DisabPeriod& disabPeriod );
  ConvertedMfbType convertedMfbTypeCal() const;
  DateMoyr dateCpiCal() const;
  DateMoyr entDeathDateCal() const;
  int yearCpi1Cal() const;
public:
  DibGuar( const WorkerDataGeneral& newWorkerData, const PiaData& newPiaData,
    const PiaParams& newPiaParams, int newMaxyear );
  virtual ~DibGuar();
  void calculate();
  /// <summary>Returns the mfb based on the deconverted pia brought up to
  /// the cessation date.</summary>
  ///
  /// <returns>The mfb based on the deconverted pia brought up to the
  /// cessation date.</returns>
  double getCessationMfbConv() const { return cessationMfbConv; }
  /// <summary>Returns the original mfb at the cessation date.</summary>
  ///
  /// <returns>The original mfb at the cessation date.</returns>
  ///
  /// <remarks>This could be based on the Disability Maximum, and so may
  /// need to be converted (see <see cref="getCessationMfbConv"/>).</remarks>
  double getCessationMfbOrig() const { return cessationMfbOrig; }
  /// <summary>Returns true if colas apply from cessation date of prior
  /// disability to new entitlement date.</summary>
  ///
  /// <returns>True if colas apply from cessation date of prior
  /// disability to new entitlement date.</returns>
  bool getColasApply() const { return colasApply; }
  /// <summary>Returns the type of converted mfb.</summary>
  ///
  /// <returns>The type of converted mfb.</returns>
  ConvertedMfbType getConvertedMfbType() const { return convertedMfbType; }
  /// <summary>Returns true if the mfb from the prior disability was
  /// determined using the Disability Maximum method.</summary>
  ///
  /// <returns>True if the mfb from the prior disability was
  /// determined using the Disability Maximum method.</returns>
  bool getDiMaxApplies() const { return diMaxApplies; }
  /// <summary>Returns the mfb based on the deconverted pia at eligibility
  /// date.</summary>
  ///
  /// <returns>The mfb based on the deconverted pia at eligibility
  /// date.</returns>
  double getRawMfb() const { return rawMfb; }
  /// <summary>Returns the pia deconverted from cessation date to
  /// eligibility date.</summary>
  ///
  /// <returns>The pia deconverted from cessation date to eligibility
  /// date.</returns>
  double getRawPia() const { return rawPia; }
  bool isApplicable();
  static bool isApplicable( const WorkerDataGeneral& workerData,
    const PiaData& piaData, WorkerDataGeneral::ben_type ioasdi );
private:
  DibGuar operator=( DibGuar newDibGuar );
};
