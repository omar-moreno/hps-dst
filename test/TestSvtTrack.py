#!/usr/bin/python

import os
import unittest
import ROOT as r

class TestSvtTrack(unittest.TestCase) : 

    def setUp(self):
        # Load the HpsEvent library. 
        if os.getenv('HPS_DST_PATH') is None: 
            print "[ HPS ANALYSIS ]: Error! Environmental variable HPS_DST_HOME is not set."
            print "\n[ HPS ANALYSIS ]: Exiting ..."
            sys.exit(2)

        hps_dst_path = os.environ['HPS_DST_PATH']
        hps_dst_path += "/build/lib/libHpsEvent.so"

        # Load the library in ROOT
        r.gSystem.Load(hps_dst_path)

        # Open the ROOT file
        resource_path = os.environ['HPS_DST_PATH'] + "/test/resources/dst_file.root"
        self.root_file = r.TFile(resource_path)
        
        # Get the TTree "HPS_Event" 
        self.tree = self.root_file.Get("HPS_Event")

        # Create an HpsEvent object 
        self.hps_event = r.HpsEvent()

        self.b_hps_event = self.tree.GetBranch("Event")
        self.b_hps_event.SetAddress(r.AddressOf(self.hps_event))


    def test_gbl_to_seed_track_reference(self) :
        print "Running gbl_reference test"
        # Loop over all events in the file
        for entry in xrange(0, self.tree.GetEntries()) : 

            # Print the event number every 500 events
            if (entry+1)%500 == 0 : print "Event " + str(entry+1)

            # Read the ith entry from the tree.  This "fills" HpsEvent and allows 
            # access to all collections
            self.tree.GetEntry(entry)

            # Loop over all tracks in the event
            for track_n in xrange(0, self.hps_event.getNumberOfTracks()) : 

                # Get the track from the event
                track = self.hps_event.getTrack(track_n)
                
                # Get the GBL track associated with the current seed track
                gbl_track = track.getGblTrack()

                # All seed tracks should have an associated GBL track
                self.assertIsNotNone(gbl_track, "GBL Track has not been set correctly.")

                # Get the seed track associated with the GBL track
                seed_track = gbl_track.getSeedTrack()

                # Require the seed track reference obtained from the GBL track 
                # matches the seed track used to obtain the GBL track in the first
                # place.
                self.assertEqual(track, seed_track, "References are set incorrectly")


        
if __name__ == '__main__' :
    unittest.main()
