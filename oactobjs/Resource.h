// String IDS used in oactobjs library.

// $Id: Resource.h 1.44 2011/08/19 15:57:44EDT 044579 Development  $

#pragma once

/// <summary>(Currently unused).</summary>
#define PIA_IDS_MONTHTEXT               61204
/// <summary>Type of <see cref="PiaException"/> thrown if month of Statement
/// processing is out of range.</summary>
#define PIA_IDS_PEBESMONTH              61205
/// <summary>Type of <see cref="PiaException"/> thrown if planned age of
/// retirement is out of range.</summary>
#define PIA_IDS_AGEPLAN                 61206
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings is out of range.</summary>
#define PIA_IDS_BEGIN1                  61207
/// <summary>Type of <see cref="PiaException"/> thrown if last year of earnings
/// is before first year of earnings in backward projection period.</summary>
#define PIA_IDS_BEGIN2                  61208
/// <summary>Type of <see cref="PiaException"/> thrown if month of birth is out
/// of range.</summary>
#define PIA_IDS_BIRTH3                  61209
/// <summary>Type of <see cref="PiaException"/> thrown if day of birth is out
/// of range.</summary>
#define PIA_IDS_BIRTH4                  61210
/// <summary>Type of <see cref="PiaException"/> thrown if type of benefit is
/// out of range.</summary>
#define PIA_IDS_JOASDI                  61211
/// <summary>Type of <see cref="PiaException"/> thrown if year of birth is
/// before 1800.</summary>
#define PIA_IDS_BIRTH2                  61212
/// <summary>Type of <see cref="PiaException"/> thrown if year of birth is in
/// current year or later (in Statement case).</summary>
#define PIA_IDS_BIRTH1                  61213
/// <summary>Type of <see cref="PiaException"/> thrown if year of birth is
/// after maximum allowed.</summary>
#define PIA_IDS_BIRTH5                  61214
/// <summary>Type of <see cref="PiaException"/> thrown if sex is not
/// <see cref="Sex::sex_type::MALE"/> or <see cref="Sex::sex_type::FEMALE"/>.
/// </summary>
#define PIA_IDS_SEX                     61215
/// <summary>Type of <see cref="PiaException"/> thrown if month of entitlement
/// is out of range.</summary>
#define PIA_IDS_ENT1                    61216
/// <summary>Type of <see cref="PiaException"/> thrown if year of entitlement
/// is before 1940.</summary>
#define PIA_IDS_ENT2                    61217
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings is before 1937.</summary>
#define PIA_IDS_BEGIN3                  61218
/// <summary>Type of <see cref="PiaException"/> thrown if last year of earnings
/// is before 1937.</summary>
#define PIA_IDS_END1                    61219
/// <summary>Type of <see cref="PiaException"/> thrown if number of periods of
/// disability is not 0, 1, or 2 in a nondisability case.</summary>
#define PIA_IDS_VALDI1                  61220
/// <summary>Type of <see cref="PiaException"/> thrown if number of periods of
/// disability is not 1 or 2 in a disability case.</summary>
#define PIA_IDS_VALDI2                  61221
/// <summary>Type of <see cref="PiaException"/> thrown if backward projection
/// indicator is out of range in general case (0-2).</summary>
#define PIA_IDS_PROJBACK1               61222
/// <summary>Type of <see cref="PiaException"/> thrown if the total number of
/// years of earnings is greater than the maximum allowed
/// (<see cref="MAXYRS"/>).</summary>
#define PIA_IDS_END4                    61223
/// <summary>Type of <see cref="PiaException"/> thrown if forward projection
/// indicator is out of range in general case (0-2).</summary>
#define PIA_IDS_PROJFWRD1               61224
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings in forward projection period is after last year of earnings in
/// forward projection period.</summary>
#define PIA_IDS_END3                    61225
/// <summary>Type of <see cref="PiaException"/> thrown if year of entitlement
/// is after maximum allowed.</summary>
#define PIA_IDS_ENT3                    61226
/// <summary>Type of <see cref="PiaException"/> thrown if backward projection
/// indicator is out of range in backward projection case (1-2).</summary>
#define PIA_IDS_PROJBACK2               61227
/// <summary>Type of <see cref="PiaException"/> thrown if backward projection
/// percentage is out of range.</summary>
#define PIA_IDS_PERCBACK                61228
/// <summary>Type of <see cref="PiaException"/> thrown if forward projection
/// indicator is out of range in forward projection case (1-2).</summary>
#define PIA_IDS_PROJFWRD2               61229
/// <summary>Type of <see cref="PiaException"/> thrown if forward projection
/// percentage is out of range.</summary>
#define PIA_IDS_PERCFWRD                61230
/// <summary>Type of <see cref="PiaException"/> thrown if benefit date is
/// before entitlement date.</summary>
#define PIA_IDS_BENDATE1                61231
/// <summary>Type of <see cref="PiaException"/> thrown if month of benefit is
/// out of range.</summary>
#define PIA_IDS_BENDATE2                61232
/// <summary>Type of <see cref="PiaException"/> thrown if year of benefit is
/// before 1940.</summary>
#define PIA_IDS_BENDATE3                61233
/// <summary>Type of <see cref="PiaException"/> thrown if year of benefit is
/// after maximum allowed.</summary>
#define PIA_IDS_BENDATE4                61234
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings is in starting year of projections or later, in a Statement case.
/// </summary>
#define PIA_IDS_BEGIN4                  61235
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings is after maximum year allowed, in a non-Statement case.</summary>
#define PIA_IDS_BEGIN5                  61236
/// <summary>Type of <see cref="PiaException"/> thrown if last year of earnings
/// is after maximum year allowed.</summary>
#define PIA_IDS_END2                    61237
/// <summary>Type of <see cref="PiaException"/> thrown if month of onset is out
/// of range.</summary>
#define PIA_IDS_ONSET1                  61238
/// <summary>Type of <see cref="PiaException"/> thrown if day of onset is out
/// of range.</summary>
#define PIA_IDS_ONSET2                  61239
/// <summary>Type of <see cref="PiaException"/> thrown if year of onset is
/// after maximum allowed.</summary>
#define PIA_IDS_ONSET4                  61240
/// <summary>Type of <see cref="PiaException"/> thrown if year of onset is
/// before 1937.</summary>
#define PIA_IDS_ONSET3                  61241
/// <summary>Type of <see cref="PiaException"/> thrown if starting year of
/// assumptions is before 1979.</summary>
#define PIA_IDS_START1                  61242
/// <summary>Type of <see cref="PiaException"/> thrown if starting year of
/// assumptions is after maximum year allowed.</summary>
#define PIA_IDS_START2                  61243
/// <summary>Type of <see cref="PiaException"/> thrown if month of cessation
/// is out of range.</summary>
#define PIA_IDS_DICESS1                 61244
/// <summary>Type of <see cref="PiaException"/> thrown if year of cessation is
/// before 1937.</summary>
#define PIA_IDS_DICESS2                 61245
/// <summary>Type of <see cref="PiaException"/> thrown if year of cessation is
/// after maximum allowed.</summary>
#define PIA_IDS_DICESS3                 61246
/// <summary>Type of <see cref="PiaException"/> thrown if month of death is out
/// of range.</summary>
#define PIA_IDS_DEATH1                  61247
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_CUPROJTTL               61248
/// <summary>Type of <see cref="PiaException"/> thrown if month of prior
/// entitlement is out of range.</summary>
#define PIA_IDS_PRRENT1                 61249
/// <summary>Type of <see cref="PiaException"/> thrown if year of prior
/// entitlement is before 1940.</summary>
#define PIA_IDS_PRRENT2                 61250
/// <summary>Type of <see cref="PiaException"/> thrown if year of prior
/// entitlement is after maximum allowed.</summary>
#define PIA_IDS_PRRENT3                 61251
/// <summary>Type of <see cref="PiaException"/> thrown if entitlement of
/// survivor is before death of worker.</summary>
#define PIA_IDS_DEATH4                  61252
/// <summary>Type of <see cref="PiaException"/> thrown if death of worker is
/// before birth.</summary>
#define PIA_IDS_DEATH5                  61253
/// <summary>Type of <see cref="PiaException"/> thrown if death before 1937 in
/// a survivor case.</summary>
#define PIA_IDS_DEATH3                  61254
/// <summary>Type of <see cref="PiaException"/> thrown if disability is after
/// entitlement.</summary>
#define PIA_IDS_ONSET6                  61255
/// <summary>Type of <see cref="PiaException"/> thrown if disability is before
/// birth.</summary>
#define PIA_IDS_ONSET5                  61256
/// <summary>Type of <see cref="PiaException"/> thrown if death of worker is
/// before disability onset in a survivor case.</summary>
#define PIA_IDS_ONSET7                  61257
/// <summary>Type of <see cref="PiaException"/> thrown if disability cessation
/// before onset.</summary>
#define PIA_IDS_DICESS4                 61258
/// <summary>Type of <see cref="PiaException"/> thrown if entitlement is before
/// cessation.</summary>
#define PIA_IDS_DICESS6                 61259
/// <summary>Type of <see cref="PiaException"/> thrown if cessation is after
/// death in a survivor case.</summary>
#define PIA_IDS_DICESS7                 61260
/// <summary>Type of <see cref="PiaException"/> thrown if prior disability is
/// after current disability.</summary>
#define PIA_IDS_ONSET8                  61261
/// <summary>Type of <see cref="PiaException"/> thrown if prior disability
/// cessation after second entitlement.</summary>
#define PIA_IDS_DICESS5                 61262
/// <summary>Type of <see cref="PiaException"/> thrown if bic is not a valid
/// life or survivor bic.</summary>
#define PIA_IDS_JSURV                   61263
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JBIRTH1                 61264
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JBIRTH2                 61265
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JONSET1                 61266
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JONSET2                 61267
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JONSET3                 61268
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JONSET4                 61269
/// <summary>Type of <see cref="PiaException"/> thrown if first month of
/// waiting period is out of range.</summary>
#define PIA_IDS_WAITPD1                 61270
/// <summary>Type of <see cref="PiaException"/> thrown if year of first month
/// of waiting period is before 1940.</summary>
#define PIA_IDS_WAITPD2                 61271
/// <summary>Type of <see cref="PiaException"/> thrown if year of first month
/// of waiting period is after maximum allowed.</summary>
#define PIA_IDS_WAITPD3                 61272
/// <summary>Type of <see cref="PiaException"/> thrown if widow's disability is
/// after entitlement.</summary>
#define PIA_IDS_JONSET6                 61273
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JBIRTH3                 61274
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JBIRTH4                 61275
/// <summary>Type of <see cref="PiaException"/> thrown if widow's disability is
/// before birth.</summary>
#define PIA_IDS_JONSET5                 61276
/// <summary>Type of <see cref="PiaException"/> thrown if waiting period starts
/// before onset.</summary>
#define PIA_IDS_WAITPD4                 61277
/// <summary>Type of <see cref="PiaException"/> thrown if entitlement is before
/// waiting period.</summary>
#define PIA_IDS_WAITPD5                 61278
/// <summary>Type of <see cref="PiaException"/> thrown if prior entitlement to
/// disability is before disability onset.</summary>
#define PIA_IDS_PRRENT5                 61279
/// <summary>Type of <see cref="PiaException"/> thrown if prior entitlement is
/// not before current entitlement.</summary>
#define PIA_IDS_PRRENT4                 61280
/// <summary>Type of <see cref="PiaException"/> thrown if cessation is before
/// waiting period.</summary>
#define PIA_IDS_WAITPD6                 61281
/// <summary>Type of <see cref="PiaException"/> thrown if benefit increase
/// indicator or average wage increase indicator is out of range in Statement
/// case.</summary>
#define PIA_IDS_PEBSASM                 61282
/// <summary>Type of <see cref="PiaException"/> thrown if public pension amount
/// is out of range.</summary>
#define PIA_IDS_PUBPEN                  61283
/// <summary>Type of <see cref="PiaException"/> thrown if number of quarters of
/// coverage, 1937-77, is out of range.</summary>
#define PIA_IDS_QCTD                    61284
/// <summary>Type of <see cref="PiaException"/> thrown if number of quarters of
/// coverage, 1951-77, is out of range.</summary>
#define PIA_IDS_QC51TD                  61285
/// <summary>Type of <see cref="PiaException"/> thrown if number of annual
/// quarters of coverage is out of range.</summary>
#define PIA_IDS_QCANN                   61286
/// <summary>Type of <see cref="PiaException"/> thrown if number of quarters of
/// coverage, 1951-77, is greater than number of quarters of coverage, 1937-77.
/// </summary>
#define PIA_IDS_QC                      61287
/// <summary>Type of <see cref="PiaException"/> thrown if age is out of range
/// in <see cref="PiaCal::ardriCal"/>.</summary>
#define PIA_IDS_ARDRI1                  61288
/// <summary>Type of <see cref="PiaException"/> thrown if age is before early
/// retirement age for OAB in <see cref="PiaCal::ardriCal"/>.</summary>
#define PIA_IDS_ARDRI2                  61289
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_ARDRI3                  61290
/// <summary>Type of <see cref="PiaException"/> thrown if disabled widow(er)
/// age is before earliest possible age in <see cref="PiaCal::ardriAuxCal"/>.
/// </summary>
#define PIA_IDS_ARDRI4                  61291
/// <summary>Type of <see cref="PiaException"/> thrown if disabled widow(er)
/// entitlement is before benefit date in <see cref="PiaCal::ardriAuxCal"/>.
/// </summary>
#define PIA_IDS_ARDRI5                  61292
/// <summary>Type of <see cref="PiaException"/> thrown if aged widow(er) age is
/// before earliest possible age in <see cref="PiaCal::ardriAuxCal"/>.
/// </summary>
#define PIA_IDS_ARDRI6                  61293
/// <summary>Type of <see cref="PiaException"/> thrown if DIB entitlement
/// before 1957 in <see cref="PiaCal::ardriCal"/>.</summary>
#define PIA_IDS_ARDRI7                  61294
/// <summary>Type of <see cref="PiaException"/> thrown if age under 50 for DIB
/// before 1960 in <see cref="PiaCal::ardriCal"/>.</summary>
#define PIA_IDS_ARDRI8                  61295
/// <summary>Type of <see cref="PiaException"/> thrown if age over full
/// retirement age for DIB in <see cref="PiaCal::ardriCal"/>.</summary>
#define PIA_IDS_ARDRI9                  61296
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="AwincFile::read"/>.</summary>
#define PIA_IDS_FQINC4                  61297
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="AwbiDataFile::read"/>.</summary>
#define PIA_IDS_AWBI1                   61298
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read all
/// required data in <see cref="AwbiDataFile::read"/>.</summary>
#define PIA_IDS_AWBI3                   61299
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read all
/// required data in <see cref="AwincFile::read"/>.</summary>
#define PIA_IDS_FQINC3                  61300
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="AwbiDataFile::write"/>.</summary>
#define PIA_IDS_AWBI2                   61301
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="AwincFile::write"/>.</summary>
#define PIA_IDS_FQINC2                  61302
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="BaseYearFile::read"/>.</summary>
#define PIA_IDS_BASEYR4                 61303
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read base
/// year in <see cref="BaseYearFile::read"/>.</summary>
#define PIA_IDS_BASEYR3                 61304
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="BaseYearFile::write"/>.</summary>
#define PIA_IDS_BASEYR2                 61305
/// <summary>Type of <see cref="PiaException"/> thrown if average wage increase
/// is out of range.</summary>
#define PIA_IDS_FQINC1                  61306
/// <summary>Type of <see cref="PiaException"/> thrown if base year is out of
/// range (1979-2020).</summary>
#define PIA_IDS_BASEYR1                 61307
/// <summary>Type of <see cref="PiaException"/> thrown if benefit increase
/// indicator is out of range in general case.</summary>
#define PIA_IDS_IALTBI1                 61308
/// <summary>Type of <see cref="PiaException"/> thrown if average wage increase
/// indicator is out of range in general case.</summary>
#define PIA_IDS_IALTAW1                 61309
/// <summary>Type of <see cref="PiaException"/> thrown if wage base indicator
/// is not 1 or 2.</summary>
#define PIA_IDS_BASECH                  61310
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="BiprojFile::read"/>.</summary>
#define PIA_IDS_CPIINC4                 61311
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_BIPROJTTL               61312
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read a pia
/// file.</summary>
#define PIA_IDS_READERR                 61313
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read all
/// required data in <see cref="BiprojFile::read"/>.</summary>
#define PIA_IDS_CPIINC3                 61314
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_2YEARS                  61315
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="BiprojFile::write"/>.</summary>
#define PIA_IDS_CPIINC2                 61316
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_YEAR                    61317
/// <summary>Type of <see cref="PiaException"/> thrown if benefit increase is
/// out of range.</summary>
#define PIA_IDS_CPIINC1                 61318
/// <summary>Type of <see cref="PiaException"/> thrown if benefit increase
/// indicator is out of range in Statement case.</summary>
#define PIA_IDS_IALTBI2                 61319
/// <summary>Type of <see cref="PiaException"/> thrown if average wage increase
/// indicator is out of range in Statement case.</summary>
#define PIA_IDS_IALTAW2                 61320
/// <summary>Type of <see cref="PiaException"/> thrown if catch-up benefit
/// increase is out of range.</summary>
#define PIA_IDS_CACHUP1                 61321
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_ANDLATER                61322
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_HISTAMTTL               61323
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_AVGWGTTL                61324
/// <summary>Type of <see cref="PiaException"/> thrown if type of earnings is
/// out of range.</summary>
#define PIA_IDS_EARNTYPE                61325
/// <summary>Type of <see cref="PiaException"/> thrown if OASDI earnings are
/// out of range.</summary>
#define PIA_IDS_EARNPEBS                61326
/// <summary>Type of <see cref="PiaException"/> thrown if catch-up increase
/// indicator is out of range.</summary>
#define PIA_IDS_ANSCCH                  61327
/// <summary>Type of <see cref="PiaException"/> thrown if Medicare earnings are
/// out of range.</summary>
#define PIA_IDS_EARNHI                  61328
/// <summary>Type of <see cref="PiaException"/> thrown if wage base is out of
/// range.</summary>
#define PIA_IDS_BASE2                   61329
/// <summary>Type of <see cref="PiaException"/> thrown if type of taxes is out
/// of range.</summary>
#define PIA_IDS_TAXTYPE                 61330
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="CatchupFile::read"/>.</summary>
#define PIA_IDS_CACHUP2                 61331
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="CatchupFile::write"/>.</summary>
#define PIA_IDS_CACHUP3                 61332
/// <summary>Type of <see cref="PiaException"/> thrown if starting year of law
/// changes is out of range.</summary>
#define PIA_IDS_LAWCHG1                 61333
/// <summary>Type of <see cref="PiaException"/> thrown if law change wage
/// increase proportion is out of range.</summary>
#define PIA_IDS_LAWCHG2                 61334
/// <summary>Type of <see cref="PiaException"/> thrown if law change benefit
/// formula percentage is out of range.</summary>
#define PIA_IDS_LAWCHG3                 61335
/// <summary>Type of <see cref="PiaException"/> thrown if law change bend point
/// is out of range.</summary>
#define PIA_IDS_LAWCHG4                 61336
/// <summary>Type of <see cref="PiaException"/> thrown if law change constant
/// decline percentage is out of range.</summary>
#define PIA_IDS_LAWCHG5                 61337
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_ONSET9                  61338
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_ONSET10                 61339
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JBIRTH5                 61340
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_JONSET7                 61341
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_BIRTH6                  61342
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_PRRENT6                 61343
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_DICESS8                 61344
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_DEATH6                  61345
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_WAITPD7                 61346
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_TITLEPRO1               61347
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_TITLEPRO2               61348
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_TITLEPRO3               61349
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_ENT4                    61350
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="FieldOfficeInfoFile::read"/>.</summary>
#define PIA_IDS_FOINFO1                 61351
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read all
/// required data in <see cref="FieldOfficeInfoFile::read"/>.</summary>
#define PIA_IDS_FOINFO2                 61352
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="FieldOfficeInfoFile::write"/>.</summary>
#define PIA_IDS_FOINFO3                 61353
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="LawChangeRead::read"/>.</summary>
#define PIA_IDS_LAWCHG7                 61354
/// <summary>Type of <see cref="PiaException"/> thrown if unable to parse input
/// line in <see cref="LawChangeRead::read"/>.</summary>
#define PIA_IDS_LAWCHG6                 61355
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="LawChangeWrite::write"/>.</summary>
#define PIA_IDS_LAWCHG8                 61356
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_BENDATE5                61357
/// <summary>Type of <see cref="PiaException"/> thrown if disability onset is
/// after full retirement age.</summary>
#define PIA_IDS_ONSET11                 61358
/// <summary>Type of <see cref="PiaException"/> thrown if prior entitlement to
/// disability is after full retirement age.</summary>
#define PIA_IDS_PRRENT7                 61359
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_PIBTABLE1               61360
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_PIBTABLE2               61361
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_MAXIMUMTEXT             61362
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_AVERAGETEXT             61363
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_LOWTEXT                 61364
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_OUTFILE                 61365
/// <summary>Return value from <see cref="PiaRead::read"/> if successful read,
/// with end of file following.</summary>
#define PIA_IDS_READEOF                 61366
/// <summary>Return value from <see cref="PiaRead::read"/> if successful read,
/// with additional data following.</summary>
#define PIA_IDS_READMORE                61367
/// <summary>String ID for printing sex of worker.</summary>
#define PIA_IDS_SEXOUT                  61368
/// <summary>String ID for printing date of birth.</summary>
#define PIA_IDS_DOBOUT                  61369
/// <summary>String ID for printing OAB date of entitlement.</summary>
#define PIA_IDS_OABENTOUT               61370
/// <summary>String ID for printing OAB date of benefit.</summary>
#define PIA_IDS_OABBENOUT               61371
/// <summary>String ID for printing full retirement age.</summary>
#define PIA_IDS_NRAOUT                  61372
/// <summary>String ID for printing early retirement age.</summary>
#define PIA_IDS_NRA1OUT                 61373
/// <summary>String ID for printing date of death.</summary>
#define PIA_IDS_DEATHOUT                61374
/// <summary>String ID for printing secondary date of entitlement.</summary>
#define PIA_IDS_SURVENTOUT              61375
/// <summary>String ID for printing secondary date of benefit.</summary>
#define PIA_IDS_SURVBENOUT              61376
/// <summary>String ID for printing type of survivor.</summary>
#define PIA_IDS_JSURVOUT                61377
/// <summary>String ID for printing widow(er)'s full retirement age.</summary>
#define PIA_IDS_NRAWIDOUT               61378
/// <summary>String ID for printing date of onset.</summary>
#define PIA_IDS_ONSETOUT                61379
/// <summary>String ID for printing prior date of entitlement in disability
/// case.</summary>
#define PIA_IDS_PRIOROUT                61380
/// <summary>String ID for printing prior date of cessation.</summary>
#define PIA_IDS_PRCESSOUT               61381
/// <summary>String ID for printing DIB date of entitlement.</summary>
#define PIA_IDS_DIBENTOUT               61382
/// <summary>String ID for printing PIA.</summary>
#define PIA_IDS_PIAOUT                  61383
/// <summary>String ID for printing MFB.</summary>
#define PIA_IDS_MFBOUT                  61384
/// <summary>String ID for printing benefit increase assumptions.</summary>
#define PIA_IDS_BIOUT                   61385
/// <summary>String ID for printing average wage assumptions.</summary>
#define PIA_IDS_AWOUT                   61386
/// <summary>String ID for printing worker's name.</summary>
#define PIA_IDS_NAMEOUT                 61387
/// <summary>String ID for printing worker's Social Security number.</summary>
#define PIA_IDS_SSNOUT                  61388
/// <summary>String ID for printing noncovered pension.</summary>
#define PIA_IDS_NONPENOUT               61389
/// <summary>String ID for printing relative earnings level.</summary>
#define PIA_IDS_RELERNOUT               61390
/// <summary>String ID for printing projected wage base type.</summary>
#define PIA_IDS_PROBASOUT               61391
/// <summary>String ID for printing required quarters of coverage.</summary>
#define PIA_IDS_REQQCOUT                61392
/// <summary>String ID for printing actual quarters of coverage.</summary>
#define PIA_IDS_ACTQCOUT                61393
/// <summary>String ID for printing insured status.</summary>
#define PIA_IDS_INSTATOUT               61394
/// <summary>String ID for printing first part of disability insured status.
///</summary>
#define PIA_IDS_DINS1OUT                61395
/// <summary>String ID for printing second part of disability insured status.
///</summary>
#define PIA_IDS_DINS2OUT                61396
/// <summary>String ID for printing third part of disability insured status.
///</summary>
#define PIA_IDS_DINS3OUT                61397
/// <summary>String ID for printing elapsed years.</summary>
#define PIA_IDS_ELAPSOUT                61398
/// <summary>String ID for printing date of prior entitlement in
/// nondisability case.</summary>
#define PIA_IDS_PRENTOUT                61399
/// <summary>String ID for printing second prior date of onset.</summary>
#define PIA_IDS_ONSET2OUT               61400
/// <summary>String ID for printing second prior date of cessation.</summary>
#define PIA_IDS_CESS2OUT                61401
/// <summary>String ID for printing total special minimum years of coverage.
///</summary>
#define PIA_IDS_SPYRS2OUT               61402
/// <summary>String ID for printing second prior date of onset in
/// nondisability case.</summary>
#define PIA_IDS_PONSETOUT               61403
/// <summary>String ID for printing date of birth of widow(er).</summary>
#define PIA_IDS_DOBWIDOUT               61404
/// <summary>String ID for printing date of onset of widow(er).</summary>
#define PIA_IDS_WIDDIBOUT               61405
/// <summary>String ID for printing AMW.</summary>
#define PIA_IDS_AMEOUT                  61406
/// <summary>String ID for printing AIME.</summary>
#define PIA_IDS_AIMEOUT                 61407
/// <summary>String ID for printing total special minimum years of coverage.
///</summary>
#define PIA_IDS_SPYRSOUT                61408
/// <summary>String ID for printing PIA at eligibility.</summary>
#define PIA_IDS_PIALOUT                 61409
/// <summary>String ID for printing months of delayed retirement credit.
/// </summary>
#define PIA_IDS_DRCMOUT                 61410
/// <summary>String ID for printing delayed retirement credit factor.</summary>
#define PIA_IDS_DRCFOUT                 61411
/// <summary>String ID for printing months of actuarial reduction.</summary>
#define PIA_IDS_ARFMOUT                 61412
/// <summary>String ID for printing secondary benefit factor.</summary>
#define PIA_IDS_BENFOUT                 61413
/// <summary>String ID for printing actuarial reduction factor.</summary>
#define PIA_IDS_ARFFOUT                 61414
/// <summary>String ID for printing benefit before rounding.</summary>
#define PIA_IDS_BEN1OUT                 61415
/// <summary>String ID for printing MFB at eligibility.</summary>
#define PIA_IDS_MFBLOUT                 61416
/// <summary>String ID for printing support PIA.</summary>
#define PIA_IDS_SUPPIAOUT               61417
/// <summary>String ID for printing benefit after rounding.</summary>
#define PIA_IDS_BEN2OUT                 61418
/// <summary>String ID for printing not insured for totalization benefits.
/// </summary>
#define PIA_IDS_NTOTALOUT               61419
/// <summary>String ID for printing fully insured for totalization benefits.
/// </summary>
#define PIA_IDS_INSUREOUT               61420
/// <summary>String ID for printing not fully insured.</summary>
#define PIA_IDS_NINSUROUT               61421
/// <summary>String ID for printing unused earnings message.</summary>
#define PIA_IDS_UNEARNOUT               61422
/// <summary>String ID for printing earnings over maximum allowed.</summary>
#define PIA_IDS_ERNLIMOUT               61423
/// <summary>String ID for printing not insured in survivor case.</summary>
#define PIA_IDS_NINSTOUT                61424
/// <summary>String ID for printing special minimum PIA, support PIA unused.
/// </summary>
#define PIA_IDS_SPPIA1OUT               61425
/// <summary>String ID for printing special minimum PIA, support PIA used.
/// </summary>
#define PIA_IDS_SPPIA2OUT               61426
/// <summary>String ID for printing not disability insured.</summary>
#define PIA_IDS_NDISINOUT               61427
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_OPENERR                 61428
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_HIGHTEXT                61429
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_BIASSUM                 61430
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_AWASSUM                 61431
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_HISTAMTUPDATE           61432
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_UPDATEBI                61433
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_UPDATEAW                61434
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_UPDATEWB                61435
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_UPDATEWBOLD             61436
/// <summary>Type of <see cref="PiaException"/> thrown if no quarters of
/// coverage in a totalization case.</summary>
#define PIA_IDS_QCTOT0                  61437
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_REVIEWBI                61438
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_WAGEBASE                61439
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_PIACALRANGE             62000
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_PIACALPLRGE             62001
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_OSRANGE1                62002
/// <summary>(Currently unused.)</summary>
#define PIA_IDS_OSRANGE2                62003
/// <summary>Type of <see cref="PiaException"/> thrown if denominator is out of
/// range in <see cref="RtGeneral::project1"/>.</summary>
#define PIA_IDS_RTGENRL                 62004
/// <summary>Type of <see cref="PiaException"/> thrown if number of quarters in
/// a <see cref="QtrYear"/> is greater than 3.</summary>
#define PIA_IDS_QTRYRQTR                62005
/// <summary>Type of <see cref="PiaException"/> thrown if year in a
/// <see cref="QtrYear"/> is greater than 2100.</summary>
#define PIA_IDS_QTRYRYR                 62006
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open a
/// <see cref="Genfile"/> for output.</summary>
#define PIA_IDS_GENFILE1                62007
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open a
/// <see cref="Genfile"/> for appended output.</summary>
#define PIA_IDS_GENFILE2                62008
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open a
/// <see cref="Genfile"/> for input.</summary>
#define PIA_IDS_GENFILE3                62009
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="QcArray"/> functions.</summary>
#define PIA_IDS_QCARRAY1                62010
/// <summary>(Currently unused).</summary>
#define PIA_IDS_QCARRAY2                62011
/// <summary>Type of <see cref="PiaException"/> thrown if years are out of
/// range in <see cref="QcArray::accumulate"/>.</summary>
#define PIA_IDS_QCARRAY3                62012
/// <summary>Type of <see cref="PiaException"/> thrown if quarters are out of
/// range in <see cref="QcArray::accumulate"/>.</summary>
#define PIA_IDS_QCARRAY4                62013
/// <summary>(Currently unused).</summary>
#define PIA_IDS_FLOATANN1               62014
/// <summary>(Currently unused).</summary>
#define PIA_IDS_FLOATANN2               62015
/// <summary>(Currently unused).</summary>
#define PIA_IDS_DBLEANN1                62016
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read
/// specified years in <see cref="DoubleAnnualRead::read"/>.</summary>
#define PIA_IDS_DBLEANN2                62017
/// <summary>(Currently unused).</summary>
#define PIA_IDS_INTANN1                 62018
/// <summary>(Currently unused).</summary>
#define PIA_IDS_INTANN2                 62019
/// <summary>Type of <see cref="PiaException"/> thrown if years are out of
///range in a <see cref="FloatMonth"/> function.</summary>
#define PIA_IDS_FLOATMTH1               62020
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read
/// specified years in <see cref="FloatMonth::read"/>.</summary>
#define PIA_IDS_FLOATMTH2               62021
/// <summary>Type of <see cref="PiaException"/> thrown if years are out of
/// range in a <see cref="FloatQuarter"/> function.</summary>
#define PIA_IDS_FLOATQTR1               62022
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read
/// specified years in <see cref="FloatQuarter::read"/>.</summary>
#define PIA_IDS_FLOATQTR2               62023
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib50Mfb::getAt"/>.</summary>
#define PIA_IDS_PIB50MFB                62024
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib50Pib::getAt"/>.</summary>
#define PIA_IDS_PIB50PIB                62025
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib52Mfb::getAt"/>.</summary>
#define PIA_IDS_PIB52MFB                62026
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib54Mfb::getAt"/>.</summary>
#define PIA_IDS_PIB54MFB                62027
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib54Pia::getAt"/>.</summary>
#define PIA_IDS_PIB54PIA                62028
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib58Ame::getAt"/>.</summary>
#define PIA_IDS_PIB58AME                62029
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="Pib58Pib::getAt"/>.</summary>
#define PIA_IDS_PIB58PIB                62030
/// <summary>Type of <see cref="PiaException"/> thrown if first year is out of
/// range in <see cref="WageBaseGeneral::project"/>.</summary>
#define PIA_IDS_BASE1                   62031
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="Catchup"/> functions.</summary>
#define PIA_IDS_BICCHP                  62032
/// <summary>Type of <see cref="PiaException"/> thrown if year is before 1937
/// in <see cref="BenefitAmount"/> functions.</summary>
#define PIA_IDS_ROUND                   62033
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="PiaParams::setNoChange"/>.</summary>
#define PIA_IDS_SETNOCH                 62034
/// <summary>Type of <see cref="PiaException"/> thrown if average wage is zero
/// in <see cref="PiaParams::updateFq"/>.</summary>
#define PIA_IDS_UPDATEFQ                62035
/// <summary>Type of <see cref="PiaException"/> thrown if either year is out of
/// range in <see cref="PiaParams::unbiCatchUp"/>.</summary>
#define PIA_IDS_UNBICCHP                62036
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="EarnProject::setEarntype"/>.</summary>
#define PIA_IDS_EARNTYPEYR              62037
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="WorkerData::setTaxType"/>.</summary>
#define PIA_IDS_TAXTYPEYR               62038
/// <summary>(Currently unused).</summary>
#define PIA_IDS_NEEDAW                  62039
/// <summary>Type of <see cref="PiaException"/> thrown if year is before 1937
/// in <see cref="EarnProject::toString"/>.</summary>
#define PIA_IDS_EARNMAL                 62040
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="WorkerDataGeneral::need_bases"/>.</summary>
#define PIA_IDS_NEEDBASES               62041
/// <summary>(Currently unused).</summary>
#define PIA_IDS_NEEDBI                  62042
/// <summary>(Currently unused).</summary>
#define PIA_IDS_ATOFLOAT                62043
/// <summary>(Currently unused).</summary>
#define PIA_IDS_ATOINT                  62044
/// <summary>(Currently unused).</summary>
#define PIA_IDS_ATOLONG                 62045
/// <summary>(Currently unused).</summary>
#define PIA_IDS_ATODOUBLE               62046
/// <summary>Type of <see cref="PiaException"/> thrown if quarter is out of
/// range in a <see cref="FloatQuarter"/> function.</summary>
#define PIA_IDS_FLOATQTR3               62047
/// <summary>Type of <see cref="PiaException"/> thrown if data value is zero in
/// a <see cref="FloatQuarter"/> divide function.</summary>
#define PIA_IDS_FLOATQTR4               62048
/// <summary>(Currently unused).</summary>
#define PIA_IDS_TITLEPRO4               62049
/// <summary>Type of <see cref="PiaException"/> thrown if first year is out of
/// range in <see cref="PiaCal::relEarnPositionCal"/>.</summary>
#define PIA_IDS_RELERNPOS               62050
/// <summary>Type of <see cref="PiaException"/> thrown if type of law change is
/// out of range.</summary>
#define PIA_IDS_LAWCHG9                 62051
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in a <see cref="Catchup"/> function.</summary>
#define PIA_IDS_CACHUP4                 62052
/// <summary>Type of <see cref="PiaException"/> thrown if moddata is in error.
/// </summary>
#define PIA_IDS_MODTC                   62053
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in an <see cref="AverageWage"/> function.</summary>
#define PIA_IDS_AVGWG                   62054
/// <summary>Type of <see cref="PiaException"/> thrown if month in a date is
/// out of range.</summary>
#define PIA_IDS_DATEMONTH               62055
/// <summary>Type of <see cref="PiaException"/> thrown if year in a date is out
/// of range.</summary>
#define PIA_IDS_DATEYEAR                62056
/// <summary>Type of <see cref="PiaException"/> thrown if day in a date is out
/// of range.</summary>
#define PIA_IDS_DATEDAY                 62057
/// <summary>Type of <see cref="PiaException"/> thrown if unable to parse
/// string into a date.</summary>
#define PIA_IDS_DATEREAD                62058
/// <summary>Type of <see cref="PiaException"/> thrown if unable to difference
/// two dates.</summary>
#define PIA_IDS_DATEDIFF                62059
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="BenefitAmountArray"/> functions.</summary>
#define PIA_IDS_BENARRAY1               62060
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="BenefitAmountArray::[]"/>.</summary>
#define PIA_IDS_BENARRAY2               62061
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="OnePage::prepareStrings"/>.</summary>
#define PIA_IDS_ONEPAGE                 62062
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="InsuredPage::prepareStrings"/>.</summary>
#define PIA_IDS_TWOPAGE1                62063
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="EarningsPage::prepareStrings"/>.</summary>
#define PIA_IDS_TWOPAGE2                62064
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="Summary1Page::prepareStrings"/>.</summary>
#define PIA_IDS_TWOPAGE3                62065
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="Summary2Page::prepareStrings"/>.</summary>
#define PIA_IDS_TWOPAGE4                62066
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="Summary3Page::prepareStrings"/>.</summary>
#define PIA_IDS_TWOPAGE5                62067
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="OldStart1Page::prepareStrings"/>.</summary>
#define PIA_IDS_OSSTRING                62068
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="PiaTable1Page::prepareStrings"/>.</summary>
#define PIA_IDS_PTSTRING                62069
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="WageInd1Page::prepareStrings"/>.</summary>
#define PIA_IDS_WISTRING                62070
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="TransGuar1Page::prepareStrings"/>.</summary>
#define PIA_IDS_TGSTRING                62071
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="SpecMin1Page::prepareStrings"/>.</summary>
#define PIA_IDS_SMSTRING                62072
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="ReindWid1Page::prepareStrings"/>.</summary>
#define PIA_IDS_RWSTRING                62073
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="FrozMinPage::prepareStrings"/>.</summary>
#define PIA_IDS_FMSTRING                62074
/// <summary>Type of <see cref="PiaException"/> thrown if error in
/// <see cref="TaxesPage::prepareStrings"/>.</summary>
#define PIA_IDS_TXSTRING                62075
/// <summary>Type of <see cref="PiaException"/> thrown if year of death is
/// after maximum allowed.</summary>
#define PIA_IDS_DEATH2                  62076
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open input
/// file in <see cref="ConfigFile::read"/>.</summary>
#define PIA_IDS_CONFIG1                 62077
/// <summary>Type of <see cref="PiaException"/> thrown if unable to open output
/// file in <see cref="ConfigFile::write"/>.</summary>
#define PIA_IDS_CONFIG5                 62078
/// <summary>Type of <see cref="PiaException"/> thrown if unable to read all
/// required data in <see cref="ConfigFile::read"/>.</summary>
#define PIA_IDS_CONFIG6                 62079
/// <summary>Type of <see cref="PiaException"/> thrown if military service
/// starting date is after ending date.</summary>
#define PIA_IDS_MILSERV1                62080
/// <summary>Type of <see cref="PiaException"/> thrown if number of military
/// service periods is out of range in general case.</summary>
#define PIA_IDS_MILSERV2                62081
/// <summary>Type of <see cref="PiaException"/> thrown if number of military
/// service periods is out of range in military service case.</summary>
#define PIA_IDS_MILSERV3                62082
/// <summary>Type of <see cref="PiaException"/> thrown if military service
/// starting date is out of range.</summary>
#define PIA_IDS_MILSERV4                62083
/// <summary>Type of <see cref="PiaException"/> thrown if military service
/// ending date is out of range.</summary>
#define PIA_IDS_MILSERV5                62084
/// <summary>String ID for printing full secondary benefit.</summary>
#define PIA_IDS_FULLBEN                 62085
/// <summary>Type of <see cref="PiaException"/> thrown if family size is out of
/// bounds.</summary>
#define PIA_IDS_FAMSIZE                 62086
/// <summary>String ID for printing maximum family benefit.</summary>
#define PIA_IDS_MFBBEN                  62087
/// <summary>Type of <see cref="PiaException"/> thrown if aged wife age is
/// before early retirement age in <see cref="PiaCal::ardriAuxCal"/>.</summary>
#define PIA_IDS_ARDRI10                 62088
/// <summary>String ID for printing full retirement age of aged
/// wife/husband.</summary>
#define PIA_IDS_NRAWIFEOUT              62089
/// <summary>Type of <see cref="PiaException"/> thrown if last year of
/// historical data is too large in <see cref="AwbiDataNonFile::setData"/>.
/// </summary>
#define PIA_IDS_AWBIDAT1                62090
/// <summary>Type of <see cref="PiaException"/> thrown if type of bic is not
/// set in <see cref="Secondary::check"/>.</summary>
#define PIA_IDS_SECONDARY               62091
/// <summary>Type of <see cref="PiaException"/> thrown if number of characters
/// in a string used to construct an <see cref="Ssn"/> is less than 9.
/// </summary>
#define PIA_IDS_SSN1                    62092
/// <summary>Type of <see cref="PiaException"/> thrown if any starting or
/// ending dates are out of range in <see cref="PiaData::qcDisCal"/>.</summary>
#define PIA_IDS_QCDISCAL                62093
/// <summary>String ID for printing currently insured status in survivor
/// case.</summary>
#define PIA_IDS_CURINSOUT               62094
/// <summary>Type of <see cref="PiaException"/> thrown if secondary entitlement
/// after benefit date.</summary>
#define PIA_IDS_BENDATE6                62095
/// <summary>Type of <see cref="PiaException"/> thrown if number of months in
/// an <see cref="Age"/> is greater than 11.</summary>
#define PIA_IDS_AGEMONTH                62096
/// <summary>Type of <see cref="PiaException"/> thrown if quarter is out of
/// range in a <see cref="FloatMonth"/> function.</summary>
#define PIA_IDS_FLOATMTH3               62097
/// <summary>Type of <see cref="PiaException"/> thrown if years are
/// inconsistent in <see cref="PiaMethod::totalEarnCal"/>.</summary>
#define PIA_IDS_METHOD                  62098
/// <summary>Type of <see cref="PiaException"/> thrown if date of entitlement
/// is out of range in <see cref="OldStart::calculate"/>.</summary>
#define PIA_IDS_OLDSTART1               62099
/// <summary>Type of <see cref="PiaException"/> thrown if old start method
/// number is out of range.</summary>
#define PIA_IDS_OLDSTART2               62100
/// <summary>Type of <see cref="PiaException"/> thrown if year of entitlement
/// is out of range in <see cref="PiaTable::calculate"/>.</summary>
#define PIA_IDS_PIATABLE1               62101
/// <summary>Type of <see cref="PiaException"/> thrown if year of benefit is
/// out of range in <see cref="PiaTable::calculate"/>.</summary>
#define PIA_IDS_PIATABLE2               62102
/// <summary>Type of <see cref="PiaException"/> thrown if years are out of
/// range in <see cref="MilServDates::getQc3750"/>.</summary>
#define PIA_IDS_MILSERV6                62103
/// <summary>Type of <see cref="PiaException"/> thrown if years are out of
/// range in <see cref="MilServDates::getEarn3750"/>.</summary>
#define PIA_IDS_MILSERV7                62104
/// <summary>Type of <see cref="PiaException"/> thrown if year in most recent
/// freeze period is out of range in <see cref="PiaData::qcDisReqCal"/>.
/// </summary>
#define PIA_IDS_DISREQCAL1              62105
/// <summary>Type of <see cref="PiaException"/> thrown if year in next most
/// recent freeze period is out of range in <see cref="PiaData::qcDisReqCal"/>.
/// </summary>
#define PIA_IDS_DISREQCAL2              62106
/// <summary>Type of <see cref="PiaException"/> thrown if quarter of coverage
/// amount has not been set in <see cref="PiaData::qcCal"/>.</summary>
#define PIA_IDS_QCCAL                   62107
/// <summary>Type of <see cref="PiaException"/> thrown if projection year is
/// out of range in <see cref="EarnProject::project"/>.</summary>
#define PIA_IDS_EARNPROJ                62108
/// <summary>Type of <see cref="PiaException"/> thrown if projection year is
/// out of range in <see cref="PiaWrite::writeEarnType"/>.</summary>
#define PIA_IDS_EARNTYPE1               62109
/// <summary>Type of <see cref="PiaException"/> thrown if first year of
/// earnings is out of range in <see cref="PiaWrite::writeEarnOasdi"/> or
/// <see cref="PiaWriteAny::writeEarnOasdi"/>.</summary>
#define PIA_IDS_WRITEEARN               62110
/// <summary>Type of <see cref="PiaException"/> thrown if average wages have
/// not been set in <see cref="Qcamt::project"/>.</summary>
#define PIA_IDS_QCAMT                   62111
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="BitAnnual"/> functions.</summary>
#define PIA_IDS_BITANN1                 62112
/// <summary>Type of <see cref="PiaException"/> thrown if qclumpyear is out of
/// range.</summary>
#define PIA_IDS_QCLUMPYEAR              62113
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="QcPacked::get"/>.</summary>
#define PIA_IDS_QCPACKED1               62114
/// <summary>Type of <see cref="PiaException"/> thrown if index is out of range
/// in <see cref="QcPacked::set"/>.</summary>
#define PIA_IDS_QCPACKED2               62115
/// <summary>Type of <see cref="PiaException"/> thrown if there are no regular,
/// railroad, or military earnings.</summary>
#define PIA_IDS_BEGIN6                  62116
/// <summary>Type of <see cref="PiaException"/> thrown if number of decimals is
/// nonzero for other than a float type in pout.</summary>
#define PIA_IDS_COMMA                   62117
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="PiaParams::applyCola"/> without catch-up.</summary>
#define PIA_IDS_APPCOLA1                62118
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="PiaParams::applyCola"/> with catch-up.</summary>
#define PIA_IDS_APPCOLA2                62119
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="PiaParams::applyColaMfb"/> without catch-up.</summary>
#define PIA_IDS_APPCOLA3                62120
/// <summary>Type of <see cref="PiaException"/> thrown if year is out of range
/// in <see cref="PiaParams::applyColaMfb"/> with catch-up.</summary>
#define PIA_IDS_APPCOLA4                62121
/// <summary>Type of <see cref="PiaException"/> thrown if annual
/// <see cref="Sga"/> project function is called.</summary>
#define PIA_IDS_SGAPROJ                 62122
/// <summary>Type of <see cref="PiaException"/> thrown if noncovered pension
/// entitlement date is before birth.</summary>
#define PIA_IDS_NONPENDATE1             62123
/// <summary>Type of <see cref="PiaException"/> thrown if noncovered pension
/// entitlement date is after death.</summary>
#define PIA_IDS_NONPENDATE2             62124
/// <summary>Type of <see cref="PiaException"/> thrown if noncovered pension
/// entitlement date entry is invalid.</summary>
#define PIA_IDS_NONPENDATE3             62125
/// <summary>String ID for printing noncovered pension entitlement date.
/// </summary>
#define PIA_IDS_NONPENDATEOUT           62126
