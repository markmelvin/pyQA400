##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
from __future__ import print_function

import pyQA400
import time
import sys
import utils

def test():
    print(pyQA400.get_name())
    print("Connecting...")

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

# --------------------------------------------------------------------------
if __name__ == "__main__":
    # This is only required if your analyzer software is installed in a non-default location
    pyQA400.add_to_search_path("I:\\Programs\\QuantAsylum\\")

    if not pyQA400.connect_to_analyzer():
        print("Can't find the analyzer. Exiting.")
        sys.exit(1)

    test()
