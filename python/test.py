##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import pyQA400
import utils
import time
import sys
import os

def initialize_analyzer():
    # Initialize the units to dBV
    pyQA400.set_units(pyQA400.DBV)

    # Initialize/turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -160, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -160, 1000)
    time.sleep(0.5)


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

    frequency = 1000
    initialize_analyzer()
    for level in [-120, -100, -80, -50, -40, -30, -20, -10]:
        pyQA400.set_generator(pyQA400.GEN1, True, level, frequency)
        pyQA400.run_single()
        while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
            time.sleep(0.01)

        print("Power: %f\tPeak Power: %f" % (pyQA400.compute_power_DB_on_last_data_over_bandwidth(pyQA400.LEFTIN, frequency-12, frequency+12), pyQA400.compute_peak_power_DB_on_last_data(pyQA400.LEFTIN)))
        time.sleep(1.0)