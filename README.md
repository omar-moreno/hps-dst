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

##### Preliminaries #####

In order to build the hps-dst package, the following build tools are required:
* [GCC >= 4.8](https://gcc.gnu.org/install/)
* [CMake >= 2.8](http://www.cmake.org/cmake/help/install.html)

The hps-dst package has the following dependencies: 
* [The LCIO C++ API](http://lcio.desy.de/v02-04-03/doc/manual_html/manual.html#SECTION00030000000000000000)
* [ROOT data analysis framework](http://root.cern.ch/drupal/content/installing-root-source)
* [Generalized Broken Lines](https://www.wiki.terascale.de/index.php/GeneralBrokenLines)

In order the build the documentation, the following package is also required:
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/install.html) 

##### Getting the Source Code #####

###### Getting a Tagged Release ######

Unless you are an active developer on the project, it is best to use a tagged 
release. A release can be downloaded from the projects 
[release page](https://github.com/omar-moreno/hps-dst/releases).  The project
can be downloaded as a zip file or tag.gz file.

Alternatively, a release can be downloaded and untarred by issuing the 
following commands from a terminal

    wget https://github.com/omar-moreno/hps-dst/archive/v0.7.tar.gz
    tar zxvf v0.7.tar.gz
    cd hps-dst-0.7

Once downloaded, the hps-dst package can be built in the usual way.

###### Cloning the Repository from GitHub ######

The hps-dst package is stored in a public github repository.  The code can be 
"cloned" i.e. copied to a users local machine by issuing the following commands
from a terminal

	cd /path/to/workdir
	git clone https://github.com/omar-moreno/hps-dst.git

A github account is not required to clone the source code.

##### Building the DST Maker and HpsEvent API

Before the DST source can be build, the following environmental variables need to be set:

	ROOTSYS=/path/to/root
	GBL_DIR=/path/to/gbl/cpp
	LCIO=/path/to/lcio

In order to avoid polluting the source tree, an out-of-source build is encouraged.
This can be done as follows:

	cd hps-dst
	mkdir build; cd build
	cmake ../
	make

This will create the binaries (in the build/bin directory) along with the shared
library HpsEvent.so (in the build/lib directory) which contains the ROOT 
dictionary and HpsEvent API.  Building of the API documentation is done by Doxygen
as follows:

	make doc

This will generate both LaTex and html documentation in the directory hps-dst/doc.

The DST Maker
-------------

DST Structure
-------------

Getting Started with Analysis
-----------------------------

