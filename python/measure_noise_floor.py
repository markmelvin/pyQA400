##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import pyQA400
import utils
import time
import sys
import math
import os

def initialize_analyzer():
    # Initialize the units to dBV
    pyQA400.set_units(pyQA400.DBV)

    # Initialize/turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -160, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -160, 1000)
    time.sleep(0.5)

def measure_noise_floor(input_channel=pyQA400.LEFTIN):
    pyQA400.run()
    time.sleep(10.0)
    pyQA400.stop()
    return pyQA400.get_data(input_channel)

def plot(data):
    x_data = [w[0] for w in data]
    y_data = [20*math.log10(w[1]) for w in data]
    win = pg.GraphicsWindow(title="FFT of Noise Floor")

    # Enable antialiasing for prettier plots
    pg.setConfigOptions(antialias=True)

    p1 = win.addPlot(title="FFT of Noise Floor")
    p1.setLogMode(True, False)
    p1.plot(x_data, y_data)
    x_axis = p1.getAxis("bottom")
    x_axis.setRange(0, 5)

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
    data = measure_noise_floor(input_channel=pyQA400.RIGHTIN)
    if SHOULD_PLOT:
        import pyqtgraph as pg
        plot(data)
        pg.QtGui.QApplication.exec_()
    else:
        print([w[0] for w in data])