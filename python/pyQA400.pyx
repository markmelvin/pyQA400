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

# --------------------------------------------------------------------------
# Utility functions
# --------------------------------------------------------------------------
def connect_to_analyzer(timeout=10.0):
    """Launches the QA400 software and connects to the analyzer. Returns
       True on success, or false if the operation times out.
       
       timeout - The number of seconds to wait for the device to connect."""
    inittime = time.clock()
    while not is_connected():
        if time.clock() > timeout:
            return False
        time.sleep(0.2)
    return True

# --------------------------------------------------------------------------
# API functions
# --------------------------------------------------------------------------
def launch_application_if_not_running():
    """Launches the QA400 application if it is not already running."""
    LaunchApplicationIfNotRunning()

def add_to_search_path(_path):
    """Adds the given path to the search path when looking for the QAAnalyzer
       executable."""
    py_byte_string = _path.encode('UTF-8')
    cdef char* c_string = py_byte_string
    AddToSearchPath(c_string)

def get_name():
    """Returns the friendly name of the host hardware. 
	   In the case of the QA400, this will be 'QuantAsylum QA400 Audio
       Analyzer' (without quotes).  This function will succeed whether 
       or not the hardware is attached to the PC, but the QA400
       application must be running for this to succeed."""
    cdef char* c_string
    cdef bytes py_string
    namelen = GetNameLength()
    c_string = <char*> PyMem_Malloc((namelen + 1)* sizeof(char))
    if not c_string:
        raise MemoryError("Failed to allocate memory")
    try:
        GetName(c_string, namelen)
        return c_string[:namelen]
    finally:
        PyMem_Free(c_string)

def is_connected():
    """Returns true if the hardware is connected and functioning. It is
       recommended that you call this as the first thing you do, as it will
       automatically launch the application and initialize the analyzer.
       If this returns false, do not attempt to use the analyzer.
	   You may need to unplug the USB cable and plug it back in again
       before retrying.  It is also recommended that you wait a couple
       seconds for the application to initialize if this is the first
       time you have connected (and the program just launched)."""
    return IsConnected()

def set_to_default(filename=""):
    """Sets the analyzer to a known default state. If fileName is an empty
       string, then the internal default is used. Otherwise, the indicated
       settings file is loaded. If indicated file name was successfully
       loaded, then true is returned. If the filename is empty then true
       is always returned and default is always loaded."""
    py_byte_string = filename.encode('UTF-8')
    cdef char* c_string = py_byte_string
    return SetToDefault(c_string)

def set_generator(generator, turn_on, amplitude_dbfs, frequency):
    """Sets the generator to the specified amplitude and frequency.
       The current units are used.
       
       generator        One of LEFTOUT or RIGHTOUT
       turn_on          True to turn on the generator, false to
                        leave it off
       amplitude_dbfs   The amplitude (in dBFS)
       frequency        The frequency of the sine wave (in Hz)"""
    SetGenerator(generator, turn_on, amplitude_dbfs, frequency)

def run():
    """This is the same as pressing the RUN button on the front
       panel when the analyzer is stopped."""
    Run()

def stop():
    """This is the same as pressing the STOP button on the front
       panel when the analyzer is stopped."""
    Stop()

def run_single_at_level_and_frequency(ampl1, freq1):
    """This will set Generator 1 to active, to an amplitude of
       amp1, and a frequency of freq1 and then a measurement will
       be made with the new generator settings. A single acquisition
       will be performed. After the acquisition finishes, the
       analyzer will automatically stop. The collected data can then
       be obtained using the GetData() function. Note that this
       function only starts the acquisition. The function will return
       immediately, and then the acquistion state must be polled via
       GetAcquisitionState() to know when the acquisition has
       finished.
       
       ampl1    The desired amplitude of Generator 1
       freq1    The desired frequency of Generator 1"""
    RunSingle(ampl1, freq1)

def run_single():
    """Performs a single acquisition with all of the current
       settings. After the acquisition finishes, the analyzer will 
	   automatically stop. The collected data can then be obtained
       using the GetData() function. Note that this function only
       starts the acquisition. The function will return immediately,
       and then the acquistion state must be polled via
       GetAcquisitionState() to know when the acquisition has finished."""
    RunSingle()

def run_single_freq_response(ampl):
    """Performs a single frequency response sweep.
    
       ampl     The desired amplitude of the step signal (on
                Generator 1)"""
    RunSingleFR(ampl)

def get_acquisition_state():
    """Returns the state of the analyzer. The state will either be
       STOPPED or BUSY."""
    return GetAcquisitionState()

def get_data(channel):
    """Retrieves the last collected data. If this is called while
       the analyzer is busy, the result is undefined. The returned
       data is a list of tuples of spectrum data, and each point
       contains the data amplitude (expressed linearly, and
       referenced to full scale) and data frequency. Typically,
       you will want to convert this amplitude data into dB.
       
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)"""
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

def get_time_data(channel):
    """Retrieves the last collected time data. If this is called
       while the analyzer is busy, the result is undefined. The
       returned data is a list of tuples of time data, and
       each point contains the data amplitude (y-value ranging from
       -1 to 1) and time.
       
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)"""
    cdef PointFVector data
    data.length = GetLastTimeDataLength(channel)
    data.values = <PointF*> PyMem_Malloc(data.length * sizeof(PointF))
    if not data.values:
        raise MemoryError("Failed to allocate memory")
    try:
        GetTimeData(channel, &data)
        return [(data.values[i].x, data.values[i].y) for i in range(data.length)]
    finally:
        PyMem_Free(data.values)

def compute_power_DB_on_last_data(channel):
    """Compute the power of the data from the last acquisition.
    
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)

       Returns the computed power in dB."""
    return ComputePowerDBOnLastData(channel)

def compute_power_DB(data):
    """Compute the power of the given spectrum data.
    
       data     The spectrum data
       
       Returns the computed power in dB."""
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePowerDB(&cdata)
    finally:
        PyMem_Free(cdata.values)

def compute_power_DB_on_last_data_over_bandwidth(channel, start_freq, end_freq):
    """Compute the power of the data from the last acquisition
       over the given bandwidth.
    
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)
       start_freq   The start frequency
       end_freq     The end frequency

       Returns the computed power in dB."""
    return ComputePowerDBOnLastData(channel, start_freq, end_freq)

def compute_power_DB_over_bandwidth(data, start_freq, end_freq):
    """Compute the power of the given spectrum data over the
       given bandwidth.
    
       data         The spectrum data
       start_freq   The start frequency
       end_freq     The end frequency
       
       Returns the computed power in dB."""
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePowerDB(&cdata, start_freq, end_freq)
    finally:
        PyMem_Free(cdata.values)

def compute_peak_power_DB_on_last_data(channel):
    """Find the peak and compute the power of the data from
       the last acquisition.
    
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)

       Returns the computed power in dB."""
    return ComputePeakPowerDBOnLastData(channel)

def compute_peak_power_DB(data):
    """Find the peak and compute the power of the given
       spectrum data.
    
       data     The spectrum data
       
       Returns the computed power in dB."""
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputePeakPowerDB(&cdata)
    finally:
        PyMem_Free(cdata.values)

def compute_THD_percent_on_last_data(channel, fundamental, max_freq):
    """Given a previous data acquisition, this will compute the
       THD of the data from the last acquisition.
       
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)
       fundamental  The target fundamental frequency
       max_freq     The upper harmonic to consider (in Hz)"""
    return ComputeTHDPctOnLastData(channel, fundamental, max_freq)

def compute_THD_percent(data, fundamental, max_freq):
    """Compute the THD of the given spectrum data.
       
       data         The spectrum data
       fundamental  The target fundamental frequency
       max_freq     The upper harmonic to consider (in Hz)"""
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputeTHDPct(&cdata, fundamental, max_freq)
    finally:
        PyMem_Free(cdata.values)

def compute_phase_on_last_data(reference_channel, signal_channel,
                               apply_compensation, compensation_freq):
    """Given a previous data acquisition, this will compute the
       phase of the signal_channel with respect to the
       reference_channel.
       
       reference_channel    The channel to obtain the reference data
                            from (one of LEFTOUT, LEFTIN, RIGHTOUT
                            or RIGHTIN)
       signal_channel       The channel to obtain the signal data
                            from (one of LEFTOUT, LEFTIN, RIGHTOUT
                            or RIGHTIN)
       apply_compensation   If True, the routine will compensate for
                            delays in the QA400 hardware
       compensation_freq    If apply_compensation is True, the frequency
                            of compensation must be specified. If
                            apply_compensation is False, this parameter
                            can be 0
       """
    return ComputePhaseOnLastData(reference_channel, signal_channel, apply_compensation, compensation_freq)

def compute_phase(reference_data, signal_data, apply_compensation,
                  compensation_freq):
    """Compute the phase of the signal_channel with respect to the
       reference_channel.
       
       reference_data       The reference channel data
       signal_data          The signal channel data
       apply_compensation   If True, the routine will compensate for
                            delays in the QA400 hardware
       compensation_freq    If apply_compensation is True, the frequency
                            of compensation must be specified. If
                            apply_compensation is False, this parameter
                            can be 0
       """
    cdef PointFVector crefdata, csigdata
    PointFVectorFromPythonList(&crefdata, reference_data)
    PointFVectorFromPythonList(&csigdata, signal_data)
    try:
        return ComputePhase(&crefdata, &csigdata, apply_compensation, compensation_freq)
    finally:
        PyMem_Free(crefdata.values)
        PyMem_Free(csigdata.values)

def compute_THDN_percent_on_last_data(channel, fundamental, min_freq, max_freq):
    """Given a previous data acquisition, this will compute the
       THD+N of the data from the last acquisition.
       
       channel      The channel to obtain the data from (one of
                    LEFTOUT, LEFTIN, RIGHTOUT or RIGHTIN)
       fundamental  The target fundamental frequency
       min_freq     The minimum frequency for the noise calculation
       max_freq     The upper harmonic to consider (in Hz)"""
    return ComputeTHDNPctOnLastData(channel, fundamental, min_freq, max_freq)

def compute_THDN_percent(data, fundamental, min_freq, max_freq):
    """Compute the THD+N of the given spectrum data.
       
       data         The spectrum data
       fundamental  The target fundamental frequency
       min_freq     The minimum frequency for the noise calculation
       max_freq     The upper harmonic to consider (in Hz)"""
    cdef PointFVector cdata
    PointFVectorFromPythonList(&cdata, data)
    try:
        return ComputeTHDNPct(&cdata, fundamental, min_freq, max_freq)
    finally:
        PyMem_Free(cdata.values)

def set_units(units):
    """Sets the units for the data.
    
       units        One of DBFS or DBV"""
    SetUnits(units)

# ------------------------------------------------------------------
# Internal helper functions
# ------------------------------------------------------------------
cdef PointFVectorFromPythonList(PointFVector *cdata, data):
    """Allocates a PointFVector array for the given list of tuples.

       It is up to the caller to later free the allocated array."""
    cdata.length = len(data)
    cdata.values = <PointF*> PyMem_Malloc(cdata.length * sizeof(PointF))
    if not cdata.values:
        raise MemoryError("Failed to allocate memory")
    for i in range(cdata.length):
        cdata.values[i].x = data[i][0]
        cdata.values[i].y = data[i][1]
