To build:
- Metrowerks CodeWarrior 9 or 10 is required
- I used Mac OS X 10.4 on PowerPC

1. Download boost_1_33_1.zip and extract it into the root of the repository
2. In boost_1_33_1/boost/date_time/local_time/local_time_types.hpp, wrap the line
'typedef date_time::microsec_clock<local_date_time> local_microsec_clock;'
in an #ifdef for BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK
3. Launch and make anypiamacprojOSX/anypiamac.mcp

Xcode/Intel port by Brendan Shanks, May 2015
There is no warranty. Use at your own risk.
My work on this program is hereby placed in the public domain.

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
