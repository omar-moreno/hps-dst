# XCode files for hps-dst

This directory contains the setup that you need if you want to edit/maintain/compile 
the dst-make and libHpsEvent.dylib from a Mac using XCode. 

The only file(s) you will need to customize for your own setup would be the *.xcconfig
files. Most things you would want to modify are in HpsEventlib.xcconfig

## Modifying HpsEventlib.xcconfig

This file contains pointers to various locations on your computer:

DSTROOT=/...    => This is where the library and executable will go. Library is in "Debug" or "Release", exec in "bin". 
ROOTSYS=/...    => The location of the root system files, i.e. where $ROOTSYS points.
LCIO=/...       => The location of the LCIO installation. 
GBL=/...        => The location of the GBL installation.

