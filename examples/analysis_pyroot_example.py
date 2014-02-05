#!/usr/bin/python2.7

#
# @section purpose:
#		A simple PyRoot analysis demonstrating the use of a DST to make simple
#		plots of Ecal and SVT physics objects
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
import os
from ROOT import gSystem, gDirectory, AddressOf
from ROOT import TFile, TCanvas, TH1F, TH2F, TRefArray

#--- Functions ---#
#-----------------#

def setupCanvas(canvas):
	canvas.SetFillColor(0)
	canvas.SetBorderMode(0)
	canvas.SetBorderSize(0)
	canvas.SetFrameFillColor(0)
	canvas.SetFrameBorderMode(0)
	
def setup1DHistogram(histo, x_axis_title):
	histo.SetStats(0)
	histo.GetXaxis().SetTitle(x_axis_title)
	histo.GetXaxis().SetTitleSize(0.03)
	histo.GetXaxis().SetLabelSize(0.03)
	histo.GetYaxis().SetTitleSize(0.03)
	histo.GetYaxis().SetLabelSize(0.03)

def setup2DHistogram(histo, x_axis_title, y_axis_title):
	histo.GetYaxis().SetTitle(y_axis_title)
	setup1DHistogram(histo, x_axis_title)

#------------------#

#--- Main ---#
#------------#

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input",  help="ROOT file to process")
parser.add_argument("-o", "--output",  help="Name of output pdf file")
args = parser.parse_args()

# If an input ROOT file was not specified, exit
if not args.input:
	print "\nPlease specify a ROOT file to process."
	print "\nUse the -h flag for usage\n"
	sys.exit(2)

# If an output file name was not specified, set a default name and warn
# the user 
if args.output:
    output_file = args.output
else: 
	output_file = "analysis_output.pdf"
	print "An output file name was not specified. Setting the name to " 
	print output_file

# Load the HpsEvent library
if os.getenv('HPS_DST_HOME') is None: 
	print "Error! Environmental variable HPS_DST_HOME is not set."
	print "\nExiting ..."
	sys.exit(2)
	
hps_dst_path = os.environ['HPS_DST_HOME']
hps_dst_path += "/build/lib/libHpsEvent.so"
gSystem.Load(hps_dst_path)

# import the modules used by HpsEvent i.e. HpsEvent, 
# SvtTrack, EcalCluster ...
from ROOT import HpsEvent, SvtTrack, EcalCluster, EcalHit

#--- Setup ROOT histograms ---#
#-----------------------------#

# Create a canvas and set its characteristics
canvas = TCanvas("canvas", "Analysis Example", 700, 700)
setupCanvas(canvas)

# Ecal
h_hit_pos = TH2F("h_hit_pos", "Ecal Hit Position", 47, -23, 24, 12, -6, 6)
setup2DHistogram(h_hit_pos, "Ecal Hit Index - x", "Ecal Hit Index -y")
h_cluster_energy = TH1F("h_cluster_energy", "Ecal Cluster Energy", 100, 0, 5.5)
setup1DHistogram(h_cluster_energy, "Ecal Cluster Energy [GeV]")

# Tracking
h_p  = TH1F("h_p",  "Momentum - All Tracks", 100, 0, 5.5)
setup1DHistogram(h_p, "Momentum [GeV]")
h_px = TH1F("h_px", "p_{x} - All Tracks", 100, 0, 5.5)
setup1DHistogram(h_px, "p_{x} [GeV]")
h_py = TH1F("h_py", "p_{y} - All Tracks", 40, -.2, .2)
setup1DHistogram(h_py, "p_{y} Momentum [GeV]");
h_pz = TH1F("h_pz", "p_{z} - All Tracks", 40, -.2, .2)
setup1DHistogram(h_pz, "p_{z} Momentum [GeV]");

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

#--- Analysis ---#
#----------------#

# Loop over all events in the file
for entry in xrange(0, tree.GetEntries()) : 
    
    # Print the event number every 500 events
    if (entry+1)%500 == 0 : print "Event " + str(entry+1)

    # Read the ith entry from the tree.  This "fills" HpsEvent and allows 
    # access to all collections
    tree.GetEntry(entry)

	# Loop over all of the Ecal clusters in the event
    for cluster_n in xrange(0, hps_event.getNumberOfEcalClusters()):
		
		# Get an Ecal cluster from the event
		ecal_cluster = hps_event.getEcalCluster(cluster_n)
		
		# Get the Ecal cluster energy
		cluster_energy = ecal_cluster.getEnergy()
		
		# Fill the cluster energy plot
		h_cluster_energy.Fill(cluster_energy)
		
		# Get the Ecal hits used to create the cluster
		ecal_hits = ecal_cluster.getEcalHits()
		
		#Loop through all of the Ecal hits and plot their positions	
		for hit_n in xrange(0, ecal_hits.GetEntries()):
			
			# Get an Ecal hit from the cluster
			ecal_hit = ecal_hits.At(hit_n)
			
			# Get the crystal index of the ecal hit
			index_x = ecal_hit.getXCrystalIndex()
			index_y = ecal_hit.getYCrystalIndex()
			
			# Fill the Ecal hit position plot
			h_hit_pos.Fill(index_x, index_y, 1)

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
h_hit_pos.Draw("colz")
canvas.Print(output_file + "(")
h_cluster_energy.Draw()
canvas.Print(output_file + "(")
h_p.Draw("")
canvas.Print(output_file + "(")
h_px.Draw("")
canvas.Print(output_file + "(")
h_py.Draw("")
canvas.Print(output_file + "(")
h_pz.Draw("")
canvas.Print(output_file + ")")


	