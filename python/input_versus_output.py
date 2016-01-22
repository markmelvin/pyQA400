##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
from __future__ import print_function

import pyQA400
import utils
import time
import sys
import os

_TOL = 12

def initialize_analyzer():
    # Initialize the units to dBV
    pyQA400.set_units(pyQA400.DBV)

    # Initialize/turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -160, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -160, 1000)
    time.sleep(0.5)

def get_peak_power(generator, channel, level, fundamental):
    pyQA400.set_generator(generator, True, level, fundamental)
    pyQA400.run_single()
    while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
        time.sleep(0.01)
    return pyQA400.compute_power_DB_on_last_data_over_bandwidth(channel, fundamental-_TOL, fundamental+_TOL)

def input_versus_output(start_dBV, end_dBV, step_dBV,
                        frequency_hz=1000,
                        generator=pyQA400.GEN1,
                        input_channel=pyQA400.LEFTIN):

    # Create a list of levels to sweep across
    test_levels = utils.linspace(start_dBV, end_dBV, step=step_dBV)

    results = []
    for level in test_levels:
        power_dbv = get_peak_power(generator, input_channel, level, frequency_hz)
        print(level, power_dbv)
        results.append( (level, power_dbv) )
    return results

def plot(data):
    x_data = [w[0] for w in data]
    y_data = [w[1] for w in data]
    win = pg.GraphicsWindow(title="Input versus Output")

    # Enable antialiasing for prettier plots
    pg.setConfigOptions(antialias=True)

    p1 = win.addPlot(title="Input versus Output")
    p1.setLogMode(False, False)
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
    data = input_versus_output(-60, -8, 1,
                               generator=pyQA400.GEN1,
                               input_channel=pyQA400.LEFTIN)
    if SHOULD_PLOT:
        import pyqtgraph as pg
        plot(data)
        pg.QtGui.QApplication.exec_()
    else:
        print(data)