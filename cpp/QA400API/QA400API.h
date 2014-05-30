// QA400API.h
#pragma once

// Sets the interface function's decoration as export or import
#ifdef CSHARPWRAPPER_EXPORTS 
#define EXPORT_SPEC __declspec( dllexport )
#else
#define EXPORT_SPEC __declspec( dllimport )
#pragma comment (lib, "QA400API.lib")
#endif

struct EXPORT_SPEC PointF
{
	float x;
	float y;
};

struct EXPORT_SPEC PointFVector
{
	unsigned int length;
	PointF *values;
};

class EXPORT_SPEC QA400API
{
public:
	enum AcquisitionState{ Stopped, Busy };
	enum ChannelType{ LeftIn, RightIn, LeftOut, RightOut };
	enum UnitsType { dBFS, dBV };
	enum GenType { Gen1, Gen2 };

	/// <summary>
	/// Launches the QA400 application if it is not already running. 
	/// </summary>
	/// <returns></returns>
	static void LaunchApplicationIfNotRunning();

	/// <summary>
	/// Returns the friendly name of the host hardware (via pBuffer). 
	/// In the case of the QA400, this will be "QuantAsylum QA400 Audio Analyzer" without quotes.
	/// This function will succeed whether or not the hardware is attached to the PC, but the QA400
	/// application must be running for this to succeed. 
	/// Use GetNameLength to determine how big of a character buffer to allocate (length).
	///
	/// This whole messiness is required due to strings being passed across .dll boundaries.
	/// There may be a better way to do this...
	/// </summary>
	/// <returns></returns>
	static void GetName(char *pBuffer, unsigned int length);

	/// <summary>
	/// Returns the length of the string that would be returned by GetName.  The returned
	/// length does not include a byte for the null termination character.
	/// </summary>
	/// <returns></returns>
	static unsigned int GetNameLength();

	/// <summary>
	///  
	/// </summary>
	/// <returns></returns>
	static bool IsConnected();

	/// <summary>
	/// Sets the analyzer to a known default state. If fileName is an empty string (which means "", which isn't the same as NULL), then the 
	/// internal default is used. Otherwise, the indicated settings file is loaded. If indicated file name was successfully loaded, then true 
	/// is returned. If the filename is empty then true is always returned and default is always loaded. 
	/// </summary>
	static bool SetToDefault(const char *fileName);

	/// <summary>
	/// Sets the generator to the specified amplitude and frequency. The current units are used. 
	/// </summary>
	/// <param name="gen">Generator 1 or 2</param>
	/// <param name="isOn">Sets on/off state</param>
	/// <param name="ampl">Sets amplitude</param>
	/// <param name="freq">Sets frequency. This might be rounded, depending on the host settings</param>
	static void SetGenerator(GenType gen, bool isOn, double ampl, double freq);

	/// <summary>
	/// This is the same as pressing the RUN button on the front panel when the analyzer is stopped.
	/// </summary>
	static void Run();

	/// <summary>
	/// This is the same as pressing the STOP button on the front panel when the analyzer is running.
	/// </summary>
	static void Stop();

	/// <summary>
	/// This will set the Generator 1 to active, to an amplitude of amp1, and a frequency of freq1 and then a measurement will be made 
	/// with the new generator settings. A single acquisition will be performed. After the acquisition finishes, the analyzer will 
	/// automatically stop. The collected data can then be pulled over using the GetData() function. Note that this function only starts 
	/// the acquisition. The function will return immediately, and then the acquistion state must be polled via GetAcquisitionState() to 
	/// know when the acquisition has finished.
	/// </summary>
	/// <param name="ampl1"></param>
	/// <param name="freq1"></param>
	static void RunSingle(double ampl1, double freq1);

	/// <summary>
	/// Performs a single acquisition with all of the current settings. After the acquisition finishes, the analyzer will 
	/// automatically stop. The collected data can then be pulled over using the GetData() function. Note that this function only starts 
	/// the acquisition. The function will return immediately, and then the acquistion state must be polled via GetAcquisitionState() to 
	/// know when the acquisition has finished.
	/// </summary>
	static void RunSingle();

	/// <summary>
	/// Performs a single frequency response sweep.
	/// </summary>
	/// <param name="ampl"></param>
	static void RunSingleFR(double ampl);

	/// <summary>
	///  Returns the state of the analyzer. The state will either be STOPPED or BUSY.
	/// </summary>
	/// <returns></returns>
	static AcquisitionState GetAcquisitionState();

	/// <summary>
	/// Retrieves the number of elements in the last collected data.
	/// </summary>
	/// <param name="channel"></param>
	/// <returns></returns>
	static unsigned int GetLastDataLength(ChannelType channel);

	/// <summary>
	/// Retrieves the last collected data and returns it in the passed PointFVector (assumes the vector
	/// has already allocated enough memory for the data - see GetLastDataLength).
	/// If this is called while the analyzer is busy, the result is undefined. The returned data is a PointF
	/// array of spectrum data, and each point contains the data amplitude (expressed linearly, and referenced to full scale) and data 
	/// frequency. Typically, you will want to convert this data into dB.
	/// </summary>
	/// <param name="channel"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	static void GetData(ChannelType channel, PointFVector *data);

	/// <summary>
	/// Retrieves the number of elements in the last collected time-domain data.
	/// </summary>
	/// <param name="channel"></param>
	/// <returns></returns>
	static unsigned int GetLastTimeDataLength(ChannelType channel);

	/// <summary>
	/// Retrieves the last collected time-domain data. and returns it in the passed PointFVector (assumes the vector
	/// has already allocated enough memory for the data - see GetLastTimeDataLength).
	/// If this is called while the analyzer is busy, the result is undefined. The returned data is a PointF
	/// array of time data, and each point contains the data amplitude (y value, ranging from -1 to 1) and time 
	/// </summary>
	/// <param name="channel"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	static void GetTimeData(ChannelType channel, PointFVector *data);

	/// <summary>
	/// Given a previous data acquisition, this will compute the power of the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDBOnLastData(ChannelType channel);

	/// <summary>
	/// Compute the power of the data.
	/// </summary>
	/// <param name="data"></param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDB(PointFVector *data);

	/// <summary>
	/// Given a previous data acquisition, this will compute the power of the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <param name="startFreq">The starting frequency.</param>
	/// <param name="endFreq">The ending frequency.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDBOnLastData(ChannelType channel, double startFreq, double endFreq);

	/// <summary>
	/// Compute the power of the data.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="startFreq">The starting frequency.</param>
	/// <param name="endFreq">The ending frequency.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDB(PointFVector *data, double startFreq, double endFreq);

	/// <summary>
	/// Finds the peak and computes the power in presently selected units on the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePeakPowerDBOnLastData(ChannelType channel);

	/// <summary>
	/// Find the peak and computes the power in presently selected units on the data.
	/// </summary>
	/// <param name="data"></param>
	/// <returns>Computed power in dB</returns>
	static double ComputePeakPowerDB(PointFVector *data);

	/// <summary>
	/// Given a previous data acquisition, this will compute the THD of the data from the last acquisition. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. 
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the THD computation</param>
	/// <returns>THD level in %</returns>
	static double ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq);

	/// <summary>
	/// Compute the THD of the data. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. 
	/// </summary>
	/// <param name="data">The data to compute the THD percent on.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the THD computation</param>
	/// <returns>THD level in %</returns>
	static double ComputeTHDPct(PointFVector *data, double fundamental, double maxFreq);

	/// <summary>
	/// Given a previous data acquisition, computes the phase between a reference signal and a second signal and returns the phase between those signals in degrees (-180 to +180). The input
	/// signals must be sine waves of the exact same frequency. The expected use of this function is as follows: In situations where you are measuring a DUT
	/// using a single sine generated from the GEN1, the output and input time data series can be retrieved using the GetData() call. Once you have the output and 
	/// input time data, calling this function will compute the phase between these signals. If 'applyCompensation' is true, then the routine will account
	/// for internal delays in the QA400. This will ensure that in loopback mode the phase will be reported as 0 degrees for any frequency between 0 Hz and Nyquist.
	/// </summary>
	/// <param name="reference_channel">The reference channel</param>
	/// <param name="signal_channel">The second, signal channel. If this signal occurs slightly after the reference, this the phase will be indicated as lagging</param>
	/// <param name="applyCompensation">If true, then the routine will compensate for delays inside the QA400. If false, the phase calculation will not. The frequency of compensation must be specified if true, otherwise 0 may be used.</param>
	/// <returns></returns>
	static double ComputePhaseOnLastData(ChannelType reference_channel, ChannelType signal_channel, bool applyCompensation, double compensationFreq);

	/// <summary>
	/// Computes the phase between a reference signal and a second signal and returns the phase between those signals in degrees (-180 to +180). The input
	/// signals must be sine waves of the exact same frequency. The expected use of this function is as follows: In situations where you are measuring a DUT
	/// using a single sine generated from the GEN1, the output and input time data series can be retrieved using the GetData() call. Once you have the output and 
	/// input time data, calling this function will compute the phase between these signals. If 'applyCompensation' is true, then the routine will account
	/// for internal delays in the QA400. This will ensure that in loopback mode the phase will be reported as 0 degrees for any frequency between 0 Hz and Nyquist.
	/// </summary>
	/// <param name="reference">The reference waveform</param>
	/// <param name="signal">The second signal. If this signal occurs slightly after the reference, this the phase will be indicated as lagging</param>
	/// <param name="applyCompensation">If true, then the routine will compensate for delays inside the QA400. If false, the phase calculation will not. The frequency of compensation must be specified if true, otherwise 0 may be used.</param>
	/// <returns></returns>
	static double ComputePhase(PointFVector *reference, PointFVector *signal, bool applyCompensation, double compensationFreq);

	/// Given a previous data acquisition, this will compute the THDN of the data from the last acquisition. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. As this also contains a noise calculation, the lower frequency bound must also be specified. It is expected
	/// that the minFreq < fundamental < maxFreq
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="minFreq">Determines the min frequency for the noise calculation</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the noise and THD computation</param>
	/// <returns>THDN level in %</returns>
	static double ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq);

	/// <summary>
	/// Compute the THDN of the data. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. 
	/// </summary>
	/// <param name="data">The data to compute the THD percent on.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="minFreq">Determines the min frequency for the noise calculation</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the noise and THD computation</param>
	/// <returns>THDN level in %</returns>
	static double ComputeTHDNPct(PointFVector *data, double fundamental, double minFreq, double maxFreq);

	/// <summary>
	/// Sets the input and output offsets used in all calculations.
	/// </summary>
	/// <param name="inputOffsets"></param>
	/// <param name="outputOffsets"></param>
	static void SetOffsets(double inputOffsets, double outputOffsets);

	/// <summary>
	/// Sets the units for data
	/// </summary>
	/// <param name="type">The units type to use</param>
	static void SetUnits(UnitsType type);

	/// <summary>
	/// Sets the length of the in and out sample buffers. The buffer length must be a power of 2 and must be a supported buffer
	/// length. If not a power of two, it will be rounded up to the next power of 2. 
	/// </summary>
	/// <param name="samples">Buffer length (in number of samples)</param>
	static void SetBufferLength(unsigned int samples);

	// Below are factory test interfaces. Do not call these. 
	//static void zzzTestInferface1(int i);
	//static void zzzTestInterface2(UInt16 i);
};
