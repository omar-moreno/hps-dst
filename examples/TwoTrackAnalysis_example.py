#!/usr/bin/python2.7

##
# @section purpose:
#		Script to look at events with two tracks and create some simple plots
#
# @author:		Omar Moreno <omoreno1@ucsc.edu>
#				Santa Cruz Institute for Particle Physics
#				University of California, Santa Cruz
# @date: March 29, 2013
# @version v1.0
#

#--- imports ---#
import sys
import math
import argparse
from ROOT import gSystem, gDirectory, AddressOf
from ROOT import TFile, TCanvas, TH1F

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input",  help="ROOT file to run")
parser.add_argument("-o", "--output",  help="Name of output pdf file")
args = parser.parse_args()

# If an input ROOT file was not specified, exit
if not args.input:
	print "\nPlease specify a ROOT file to process."
	print "Use the -h flag for usage\n"
	sys.exit(2)

# If the output file is not specified, write everything out to 
# default file
output_file = "default.pdf"
if args.output:
    output_file = args.output

# Load the HpsEvent library
gSystem.Load("../lib/libHpsEvent.so")

# import the modules used by HpsEvent i.e. HpsEvent, 
# SvtTrack, EcalCluster ...
from ROOT import HpsEvent, SvtTrack

#--- Setup ROOT histograms ---#
#-----------------------------#

# Create a canvas and set its characteristics
canvas = TCanvas("canvas", "Track Momentum", 700, 700)
canvas.SetFillColor(0)
canvas.SetBorderMode(0)
canvas.SetBorderSize(0)
canvas.SetFrameFillColor(0)
canvas.SetFrameBorderMode(0)

# Create histograms and set its characteristics 
h_p  = TH1F("h_p",  "Momentum - All Tracks", 100, 0, 5.5)
h_px = TH1F("h_px", "p_{x} - All Tracks", 100, 0, 5.5)
h_py = TH1F("h_py", "p_{y} - All Tracks", 100, 0, 5.5)
h_pz = TH1F("h_pz", "p_{z} - All Tracks", 100, 0, 5.5)

#-----------------------------#

# Open the ROOT file
root_file = TFile(str(args.input))

# Get the TTree "HPS_EVENT" containing the HpsEvent branch and all
# other colletions
tree = root_file.Get("HPS_Event")

# Create an HpsEvent object in order to read the TClonesArray 
# collections
hps_event = HpsEvent()

# Get the HpsEvent branch from the TTree 
b_hps_event = tree.GetBranch("Event")
b_hps_event.SetAddress(AddressOf(hps_event))

# Loop over all events in the file
for entry in xrange(0, tree.GetEntries()) : 
    if (entry+1)%500 == 0 : print "Event " + str(entry+1)

    # Read the ith entry from the tree.  This "fills" HpsEvent 
    # and all collections
    tree.GetEntry(entry)

    # If the event does not contain two tracks, skip it
    if hps_event.getNumberOfTracks() != 2 : continue

    # Loop over all tracks in the event
    for track_n in xrange(0, hps_event.getNumberOfTracks()) : 
        
        # Get the track from the event
        track = hps_event.getTrack(track_n)

        # Calculate the momentum
        px = track.getPx()
        py = track.getPy()
        pz = track.getPz()
        p = math.sqrt(px*px + py*py + pz*pz)

        # Fill the plots
        h_p.Fill(p)
        h_px.Fill(px)
        h_py.Fill(py)
        h_pz.Fill(pz)


# Save all the plots to a single pdf file
h_p.Draw("")
canvas.Print(output_file + "(")
h_px.Draw("")
canvas.Print(output_file + "(")
h_py.Draw("")
canvas.Print(output_file + "(")
h_pz.Draw("")
canvas.Print(output_file + ")")

