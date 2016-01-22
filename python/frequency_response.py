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

def get_magnitude_and_phase(generator, channel, level, fundamental):
    phase_ref_channel = pyQA400.RIGHTOUT
    if generator == pyQA400.GEN1:
        phase_ref_channel = pyQA400.LEFTOUT

    pyQA400.set_generator(generator, True, level, fundamental)
    pyQA400.run_single()
    while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
        time.sleep(0.01)

    mag = pyQA400.compute_peak_power_DB_on_last_data(channel)
    phase = pyQA400.compute_phase_on_last_data(phase_ref_channel, 
                                               channel, 
                                               True, 
                                               fundamental)
    return (mag, phase)

def frequency_response(output_level_dBV=-10, freq_start_hz=10,
                       freq_end_hz=50000, points_per_octave=10,
                       generator=pyQA400.GEN1,
                       input_channel=pyQA400.LEFTIN):

                            # Create a list of frequencies to test
    test_freqs = utils.get_frequency_list(freq_start_hz, freq_end_hz, 
                                    points_per_octave=points_per_octave)

    results = []
    for freq in test_freqs:
        results.append( (freq, get_magnitude_and_phase(generator,
                                                       input_channel,
                                                       output_level_dBV,
                                                       freq)) )
    return results

def plot(data):
    x_data = [w[0] for w in data]
    y_data1 = [w[1][0] for w in data]
    y_data2 = [w[1][1] for w in data]
    win = pg.GraphicsWindow(title="Frequency Response")

    # Enable antialiasing for prettier plots
    pg.setConfigOptions(antialias=True)

    p1 = win.addPlot(title="Frequency Response")
    p1.setLogMode(True, False)
    p1.plot(x_data, y_data1)
    #p1.plot(x_data, y_data2)

# --------------------------------------------------------------------------
if __name__ == "__main__":
    SHOULD_PLOT = True

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
    data = frequency_response(output_level_dBV=-10,
                              freq_start_hz=20,
                              freq_end_hz=70000,
                              points_per_octave=5,
                              generator=pyQA400.GEN1,
                              input_channel=pyQA400.LEFTIN)
    if SHOULD_PLOT:
        import pyqtgraph as pg
        plot(data)
        pg.QtGui.QApplication.exec_()
    else:
        print(data)