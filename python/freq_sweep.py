##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
from __future__ import print_function

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

def frequency_sweep(output_level_dBV=-10, freq_start_hz=10,
                       freq_end_hz=50000, points_per_octave=10,
                       generator=pyQA400.GEN1):

    # Create a list of frequencies to test
    test_freqs = utils.get_frequency_list(freq_start_hz, freq_end_hz, 
                                    points_per_octave=points_per_octave)

    results = []
    for freq in test_freqs:
        print("Outputting %f Hz" % freq)
        pyQA400.set_generator(generator, True, output_level_dBV, freq)
        pyQA400.run()
        time.sleep(1.0)
        pyQA400.stop()

    return results

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

    initialize_analyzer()
    frequency_sweep(output_level_dBV=-10, freq_start_hz=20, freq_end_hz=15000, points_per_octave=2)
