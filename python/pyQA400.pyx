cimport pyQA400
import cython

cdef extern from "Python.h": 
    void PyEval_InitThreads()

# Python-exposed wrappers

# Enumerations
STOPPED     = pyQA400.Stopped
BUSY        = pyQA400.Busy
    
LEFTIN      = pyQA400.LeftIn
RIGHTIN     = pyQA400.RightIn
LEFTOUT     = pyQA400.LeftOut
RIGHTOUT    = pyQA400.RightOut

DBFS        = pyQA400.dBFS
DBV         = pyQA400.dBV

GEN1        = pyQA400.Gen1
GEN2        = pyQA400.Gen2

# API functions

def launch_application_if_not_running():
    LaunchApplicationIfNotRunning()

def get_acquisition_state():
    return GetAcquisitionState()

def set_generator(generator, turn_on, amplitude_dbfs, frequency):
    SetGenerator(generator, turn_on, amplitude_dbfs, frequency)

def run_single():
    RunSingle()

def compute_THD_percent_on_last_data(channel, fundamental, max_freq):
    return ComputeTHDPctOnLastData(channel, fundamental, max_freq)
