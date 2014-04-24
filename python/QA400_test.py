##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import pyQA400
import time

# --------------------------------------------------------------------------
if __name__ == "__main__":
    pyQA400.launch_application_if_not_running()
    pyQA400.set_generator(pyQA400.GEN2, True, -20, 1000)
    pyQA400.run_single()
    while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
        time.sleep(0.1)

    percentTHD = pyQA400.compute_THD_percent_on_last_data(pyQA400.RIGHTIN, 1000, 20000)
    print "The percent THD is %f%%" % percentTHD
