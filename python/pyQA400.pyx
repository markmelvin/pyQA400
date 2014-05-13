cimport pyQA400
import cython
from cpython.mem cimport PyMem_Malloc, PyMem_Free
import time

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

def get_name():
    cdef char* c_string
    cdef bytes py_string
    namelen = GetNameLength()
    c_string = <char*> PyMem_Malloc(namelen * sizeof(char))
    if not c_string:
        raise MemoryError("Failed to allocate memory")
    try:
        GetName(c_string, namelen)
        return c_string[:namelen]
    finally:
        PyMem_Free(c_string)

def is_connected():
    return IsConnected()

def set_to_default(filename=""):
    return SetToDefault(filename)

def set_generator(generator, turn_on, amplitude_dbfs, frequency):
    SetGenerator(generator, turn_on, amplitude_dbfs, frequency)

def run_single(ampl1, freq1):
    RunSingle(ampl1, freq1)

def run():
    Run()

def stop():
    Stop()

def run_single():
    RunSingle()

def run_single_freq_response(ampl):
    RunSingleFR(ampl)

def get_acquisition_state():
    return GetAcquisitionState()

def get_data(channel):
    cdef PointFVector data
    data.length = GetLastDataLength(channel)
    data.values = <PointF*> PyMem_Malloc(data.length * sizeof(PointF))
    if not data.values:
        raise MemoryError("Failed to allocate memory")
    try:
        GetData(channel, &data)
        return [(data.values[i].x, data.values[i].y) for i in range(data.length)]
    finally:
        PyMem_Free(data.values)

def compute_power_DB_on_last_data(channel):
    return ComputePowerDBOnLastData(channel)

def compute_power_DB(data):
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePowerDB(&cdata)
    finally:
        PyMem_Free(cdata.values)

def compute_power_DB_on_last_data2(channel, startFreq, endFreq):
    return ComputePowerDBOnLastData(channel, startFreq, endFreq)

def compute_power_DB2(data, startFreq, endFreq):
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePowerDB(&cdata, startFreq, endFreq)
    finally:
        PyMem_Free(cdata.values)

def compute_peak_power_DB_on_last_data(channel):
    return ComputePeakPowerDBOnLastData(channel)

def compute_peak_power_DB(data):
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePeakPowerDB(&cdata)
    finally:
        PyMem_Free(cdata.values)

def compute_THD_percent_on_last_data(channel, fundamental, max_freq):
    return ComputeTHDPctOnLastData(channel, fundamental, max_freq)

def compute_THD_percent(data, fundamental, max_freq):
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputeTHDPct(&cdata, fundamental, max_freq)
    finally:
        PyMem_Free(cdata.values)

def compute_THDN_percent_on_last_data(channel, fundamental, minFreq, max_freq):
    return ComputeTHDNPctOnLastData(channel, fundamental, minFreq, max_freq)

def compute_THDN_percent(data, fundamental, minFreq, max_freq):
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputeTHDNPct(&cdata, fundamental, minFreq, max_freq)
    finally:
        PyMem_Free(cdata.values)

cdef PointFVectorFromPythonList(PointFVector *cdata, data):
    cdata.length = len(data)
    cdata.values = <PointF*> PyMem_Malloc(cdata.length * sizeof(PointF))
    if not cdata.values:
        raise MemoryError("Failed to allocate memory")
    for i in range(cdata.length):
        cdata.values[i].x = data[i][0]
        cdata.values[i].y = data[i][1]
