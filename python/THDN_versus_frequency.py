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

def compute_THDN(generator, channel, level, fundamental, min_freq, max_freq):
    pyQA400.set_generator(generator, True, level, fundamental)
    pyQA400.run_single()
    while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
        time.sleep(0.01)
    return pyQA400.compute_THDN_percent_on_last_data(channel, fundamental, min_freq, max_freq)

def THDN_versus_frequency(output_level_dBV=-10, freq_start_hz=20,
                            freq_end_hz=20000, points_per_octave=5,
                            generator=pyQA400.GEN1,
                            input_channel=pyQA400.LEFTIN):
    test_freqs = utils.get_frequency_list(freq_start_hz, freq_end_hz, 
                                    points_per_octave=points_per_octave)

    results = []
    for freq in test_freqs:
        results.append((freq, compute_THDN(generator, input_channel, output_level_dBV, freq, freq_start_hz, freq_end_hz)))
    return results

def plot(data):
    x_data = [w[0] for w in data]
    y_data = [w[1] for w in data]
    win = pg.GraphicsWindow(title="THD+N vs Frequency")

    # Enable antialiasing for prettier plots
    pg.setConfigOptions(antialias=True)

    p1 = win.addPlot(title="THD+N vs Frequency")
    p1.setLogMode(True, False)
    p1.plot(x_data, y_data)

# --------------------------------------------------------------------------
if __name__ == "__main__":
    SHOULD_PLOT = False

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
    data = THDN_versus_frequency(output_level_dBV=0, freq_start_hz=20, freq_end_hz=20000, points_per_octave=10)
    if SHOULD_PLOT:
        import pyqtgraph as pg
        plot(data)
        pg.QtGui.QApplication.exec_()
    else:
        print(data)