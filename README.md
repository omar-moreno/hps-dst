HPS Data Summary Tape
=====================

Overview
--------

The HPS Data Summary Tape (DST) maker uses the LCIO C++ API to read collections
of physics objects (e.g. ReconstructedParticle, Track) from reconstructed LCIO
files and convert them to ROOT data structures. The resulting ROOT files can be 
analyzed using the HpsEvent API in combination with the ROOT data analysis 
framework.

Installation
------------

###### Preliminaries

The DST maker and HpsEvent API require the following packages before they can 
built:
* [The LCIO C++ API](http://lcio.desy.de/v02-04-03/doc/manual_html/manual.html#SECTION00030000000000000000)
* [ROOT data analysis framework](http://root.cern.ch/drupal/content/installing-root-source)
* [Generalized Broken Lines](https://www.wiki.terascale.de/index.php/GeneralBrokenLines)
* [CMake > 2.8](http://www.cmake.org/cmake/help/install.html)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/install.html) (Optional. Used to build the documentation.)

###### Getting the source

The DST maker and HpsEvent API source code is stored in a public github 
repository.  The code can be "cloned" i.e. copied to a users local machine
by issuing the following commands from a terminal




The DST Maker
-------------

DST Structure
-------------

Getting Started with Analysis
-----------------------------

