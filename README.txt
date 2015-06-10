## U.S. Social Security Administration Detailed Calculator for Mac

This is the Social Security Administration's Detailed Calculator, as
available from http://www.ssa.gov/OACT/anypia/anypia.html

I (Brendan Shanks) ported the 2015.1 version to Xcode and Intel.

NOTICE: I am not an employee of the Social Security Administration and
this source code/binary is provided with NO WARRANTY and NO GUARANTEE OF ACCURACY

Supported toolchains:
- Metrowerks CodeWarrior 9.6 (tested on 10.4)
- Metrowerks CodeWarrior 10
- Xcode 2.5 (on 10.4 or 10.5)
- Xcode 3.1 (on 10.5)
- Xcode 3.2 (on 10.6)

Xcode 2.5 is what I've been using, newer Xcode versions may need SDK/compiler versions to be tweaked.

To build:

1. Download boost_1_33_1.zip and extract it into the root of the repository
2. *CodeWarrior only* In boost_1_33_1/boost/date_time/local_time/local_time_types.hpp,
wrap the line 'typedef date_time::microsec_clock<local_date_time> local_microsec_clock;'
in an #ifdef for BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK
3. Launch and make anypiamacprojOSX/anypiamac.mcp or anypiamac.xcodeproj

There is no warranty. Use at your own risk.
My work on this program is hereby placed in the public domain.
Xcode/Intel port by Brendan Shanks, May 2015


---------------------------------------------------
Original "read me.txt" provided with source code downloaded from
http://www.ssa.gov/oact/anypia/source.html on 5/20/2015:


This is a straight conversion to OS X (and OS 9) of the social security benefit
calculator at http://www.ssa.gov/OACT/ANYPIA/source.html

There is no warranty. Use at your own risk.

This compiles with Metrowerks Codewarrior 9.3.

To compile it, you'll need to download oactobjs.sea from 
http://www.ssa.gov/OACT/ANYPIA/source.html, unpack it, and put the resulting 
oactobjs folder next to the anypiamacprojOSX folder (the folder that contains this file).

My work on this program is hereby placed in the public domain.

David Phillip Oster April 25, 2005
