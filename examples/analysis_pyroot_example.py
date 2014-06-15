#!/usr/bin/python2.7

#
# @section purpose:
#		A simple PyRoot analysis demonstrating the use of a DST to make simple
#		plots of Ecal, SVT and Particle physics objects
#
# @author:		Omar Moreno <omoreno1@ucsc.edu>
#				Santa Cruz Institute for Particle Physics
#				University of California, Santa Cruz
# @date: March 29, 2013
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

#if args.help: 
#	print "Usage: analysis_pyroot_example -i [DST_INPUT_FILE] [OPTIONS]"
#	print "A DST_INPUT_FILE must be specified"
#	print "OPTIONS:\n"
#	print 

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
canvas = TCanvas("canvas", "Data Summary Tape Plots", 700, 700)
setupCanvas(canvas)

#
# Ecal
#
h_hit_pos = TH2F("h_hit_pos", "Ecal Hit Position", 47, -23, 24, 12, -6, 6)
setup2DHistogram(h_hit_pos, "Ecal Hit Index - x", "Ecal Hit Index -y")
h_cluster_energy = TH1F("h_cluster_energy", "Ecal Cluster Energy", 100, 0, 5.5)
setup1DHistogram(h_cluster_energy, "Ecal Cluster Energy [GeV]")

#
# Tracking
#
h_d0   = TH1F("h_d0",  "Track D0", 64, -8, 8);
setup1DHistogram(h_d0, "D0 [mm]");
h_tlambda = TH1F("h_tlambda",  "Track Tan(#lambda)", 64, -0.08, 0.08);
setup1DHistogram(h_tlambda, "Tan #lambda");
h_chi2 = TH1F("h_chi2", "Track #chi^{2}", 25, 0, 25);
setup1DHistogram(h_chi2, "#chi^{2}");

#
# Particles
#
h_p = TH1F("h_p", "Particle Momentum", 64, 0, 2.2);
setup1DHistogram(h_p, "Momentum [GeV]");  
h_vertex_z = TH1F("h_vertex_z", "Particle Vertex - Z", 150, -150, 150); 
setup1DHistogram(h_vertex_z, "Vertex z [mm]"); 

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
		d0 = track.getD0()
		tan_lambda = track.getTanLambda()
		chi2 = track.getChi2()

		# Fill the plots
		h_d0.Fill(d0)
		h_tlambda.Fill(tan_lambda)
		h_chi2.Fill(chi2)

	# Loop over all final state particles in the event
	for particle_n in xrange(0, hps_event.getNumberOfParticles(hps_event.FINAL_STATE_PARTICLES)):

		# Get the final state particle from the event
		particle = hps_event.getParticle(hps_event.FINAL_STATE_PARTICLES, particle_n)

		# 
		if particle.getPDG() == 22: continue

		p = particle.getMomentum()
		h_p.Fill(math.sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]))

	# Loop over all unconstrained vertexed particles in the event
	for particle_n in xrange(0, hps_event.getNumberOfParticles(hps_event.UC_VTX_PARTICLES)):

		# Get a vertexed particle from the event
		particle = hps_event.getParticle(hps_event.UC_VTX_PARTICLES, particle_n)

		vertex_z = particle.getVertexPosition()[2]
		h_vertex_z.Fill(vertex_z)


# Save all the plots to a single pdf file
h_hit_pos.Draw("colz")
canvas.Print(output_file + "(")
h_cluster_energy.Draw()
canvas.Print(output_file + "(")
h_d0.Draw("");
canvas.Print(output_file + "(");
h_tlambda.Draw(""); 
canvas.Print(output_file + "(");
h_chi2.Draw("");
canvas.Print(output_file + "(");
h_p.Draw(""); 
canvas.Print(output_file + "(");
h_vertex_z.Draw("");
canvas.Print(output_file + ")")



