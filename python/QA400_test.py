##!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import pyQA400
import time

# --------------------------------------------------------------------------
if __name__ == "__main__":
    pyQA400.launch_application_if_not_running()
    pyQA400.set_to_default()

    # pyQA400.set_generator(pyQA400.GEN1, True, -10, 1000)
    # pyQA400.set_generator(pyQA400.GEN2, False, -10, 2000)
    # pyQA400.run()
    # time.sleep(1.0)
    # pyQA400.set_generator(pyQA400.GEN1, True, -20, 2000)
    # time.sleep(1.0)
    # pyQA400.set_generator(pyQA400.GEN1, True, -30, 3000)
    # time.sleep(1.0)
    # pyQA400.set_generator(pyQA400.GEN1, True, -40, 4000)
    # time.sleep(1.0)
    # pyQA400.set_generator(pyQA400.GEN1, True, -50, 5000)
    # time.sleep(1.0)

    pyQA400.set_generator(pyQA400.GEN1, True, -10, 1000)
    pyQA400.run_single()
    while (pyQA400.get_acquisition_state() == pyQA400.BUSY):
        time.sleep(0.1)

    percentTHD = pyQA400.compute_THD_percent_on_last_data(pyQA400.LEFTIN, 1000, 20000)
    print "The percent THD is %f%%" % percentTHD