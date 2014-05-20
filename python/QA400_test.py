##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import pyQA400
import time
import sys
import pyqtgraph as pg
import math

def test1():
    print pyQA400.get_name()
    print "Connecting..."

    # Turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -110, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -110, 1000)
    time.sleep(0.5)

    FREQS = [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000]
    MIN_FREQ = 20
    MAX_FREQ = 20000
    AMPLITUDE = -20

    print "----"
    for freq in FREQS:
        pyQA400.set_generator(pyQA400.GEN1, True, AMPLITUDE, freq)
        time.sleep(0.5)
        pyQA400.run_single()
        while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
            time.sleep(0.1)

        print "Frequency: %i" % freq
        percentTHD = pyQA400.compute_THD_percent_on_last_data(pyQA400.LEFTIN, freq, MAX_FREQ)
        print "The percent THD (on last data) is %f%%" % percentTHD
        # Alternatively...
        data = pyQA400.get_data(pyQA400.LEFTIN)
        percentTHD = pyQA400.compute_THD_percent(data, freq, MAX_FREQ)
        print "The percent THD (on data from Python) is %f%%" % percentTHD

        percentTHDN = pyQA400.compute_THDN_percent_on_last_data(pyQA400.LEFTIN, freq, MIN_FREQ, MAX_FREQ)
        print "The percent THDN (on last_data) is %f%%" % percentTHDN
        percentTHDN = pyQA400.compute_THDN_percent(data, freq, MIN_FREQ, MAX_FREQ)
        print "The percent THDN (on data from Python) is %f%%" % percentTHDN

        peakpwr = pyQA400.compute_power_DB_on_last_data(pyQA400.LEFTIN)
        print "The power (on last data) is %f dBFS" % peakpwr
        peakpwr = pyQA400.compute_power_DB(data)
        print "The power (on data from Python) is %f dBFS" % peakpwr

        peakpwr = pyQA400.compute_peak_power_DB_on_last_data(pyQA400.LEFTIN)
        print "The peak power (on last data) is %f dBFS" % peakpwr
        peakpwr = pyQA400.compute_peak_power_DB(data)
        print "The peak power (on data from Python) is %f dBFS" % peakpwr
        print "----"

def plot_freq_resp():
    print pyQA400.get_name(),
    print "...connected."

    # Turn off both generators
    pyQA400.set_generator(pyQA400.GEN1, False, -150, 1000)
    pyQA400.set_generator(pyQA400.GEN2, False, -150, 1000)
    time.sleep(0.5)

    pyQA400.run_single_freq_response(0.65)
    # This seems to be required
    time.sleep(1.0)

    data = pyQA400.get_data(pyQA400.LEFTIN)
    # x_data = [0] + [math.log10(w[0]) for w in data[1:]]
    x_data = [w[0] for w in data]
    y_data = [20*math.log10(w[1]) for w in data]
    win = pg.GraphicsWindow(title="Frequency Response")

    # Enable antialiasing for prettier plots
    pg.setConfigOptions(antialias=True)

    p1 = win.addPlot(title="Frequency Response")
    p1.setLogMode(True, False)
    x_axis = p1.getAxis("bottom")
    x_axis.setRange(0, 5)
    p1.plot(x_data, y_data)
    

# --------------------------------------------------------------------------
if __name__ == "__main__":
    if not pyQA400.connect_to_analyzer():
        print "Can't find the analyzer. Exiting."
        sys.exit(1)

    plot_freq_resp()
    pg.QtGui.QApplication.exec_()
