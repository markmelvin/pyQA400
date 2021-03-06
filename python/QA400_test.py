##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
from __future__ import print_function

import pyQA400
import time
import sys
import os
import utils

def test(pathname):
    pyQA400.set_units(pyQA400.DBV)

    # Turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -110, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -110, 1000)
    time.sleep(0.5)

    FREQS = utils.get_frequency_list(1000, 8000, freq_step=1000)
    MIN_FREQ = 20
    MAX_FREQ = 20000
    AMPLITUDE = -20

    print("----")
    for freq in FREQS:
        pyQA400.set_generator(pyQA400.GEN1, True, AMPLITUDE, freq)
        time.sleep(0.5)
        pyQA400.run_single()
        while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
            time.sleep(0.1)

        print("Frequency: %i" % freq)
        percentTHD = pyQA400.compute_THD_percent_on_last_data(pyQA400.LEFTIN, freq, MAX_FREQ)
        print("The percent THD (on last data) is %f%%" % percentTHD)
        # Alternatively...
        data = pyQA400.get_data(pyQA400.LEFTIN)
        percentTHD = pyQA400.compute_THD_percent(data, freq, MAX_FREQ)
        print("The percent THD (on data from Python) is %f%%" % percentTHD)

        percentTHDN = pyQA400.compute_THDN_percent_on_last_data(pyQA400.LEFTIN, freq, MIN_FREQ, MAX_FREQ)
        print("The percent THDN (on last_data) is %f%%" % percentTHDN)
        percentTHDN = pyQA400.compute_THDN_percent(data, freq, MIN_FREQ, MAX_FREQ)
        print("The percent THDN (on data from Python) is %f%%" % percentTHDN)

        peakpwr = pyQA400.compute_power_DB_on_last_data(pyQA400.LEFTIN)
        print("The power (on last data) is %f dBFS" % peakpwr)
        peakpwr = pyQA400.compute_power_DB(data)
        print("The power (on data from Python) is %f dBFS" % peakpwr)

        peakpwr = pyQA400.compute_peak_power_DB_on_last_data(pyQA400.LEFTIN)
        print("The peak power (on last data) is %f dBFS" % peakpwr)
        peakpwr = pyQA400.compute_peak_power_DB(data)
        print("The peak power (on data from Python) is %f dBFS" % peakpwr)
        print("----")

    print("Generating a fixed tone for 5 seconds...")
    pyQA400.generate_tone(-3, 1000, 5000)
    print("Done.")
        
# --------------------------------------------------------------------------
if __name__ == "__main__":
    # Add the dependencies folder to the .NET search path so the 
    # QAConnectionManager.dll can be found
    this_pathname = os.path.abspath(os.path.dirname(sys.argv[0]))

    dependencies_path = os.path.normpath(os.path.join(this_pathname,
                                                      "..", "dependencies"))
    pyQA400.add_to_search_path(dependencies_path)

    # Also add the local path to the .NET search path (in case you are 
    # executing in another folder and have copied the QAConnectionManager.dll
    # locally)
    pyQA400.add_to_search_path(this_pathname)

    # Also add the location of the QAAnalyzer.exe executable (NOTE: This is only
    # required if your analyzer software is installed in a non-default location)
    pyQA400.add_to_search_path("I:\\Programs\\QuantAsylum\\QA400")

    if not pyQA400.connect_to_analyzer():
        print("Can't find the analyzer. Exiting.")
        sys.exit(1)

    test(this_pathname)
