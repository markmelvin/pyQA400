// QA400API.h
#pragma once

// Sets the interface function's decoration as export or import
#ifdef CSHARPWRAPPER_EXPORTS 
#define EXPORT_SPEC __declspec( dllexport )
#else
#define EXPORT_SPEC __declspec( dllimport )
#pragma comment (lib, "../Debug/QA400API.lib")
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
	/// Launches the application if it is not already running. 
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
	static unsigned int GetNameLength();

	/// <summary>
	/// Returns true if the hardware is connected and functioning. 
	/// </summary>
	/// <returns></returns>
	static bool IsConnected();

	/// <summary>
	/// Sets the analyzer to a known default state. If fileName is an empty string (which means "", which isn't the same as NULL), then the 
	/// internal default is used. Otherwise, the indicated settings file is loaded. If indicated file name was successfully loaded, then true 
	/// is returned. If the filename is empty then true is always returned and default is always loaded. 
	/// </summary>
	bool SetToDefault(const char *fileName);

	/// <summary>
	/// This is the same as pressing the RUN button on the front panel when the analyzer is stopped.
	/// </summary>
	static void Run();

	/// <summary>
	/// This is the same as pressing the RUN button on the front panel when the analyzer is running.
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
	/// Performs a singe frequency response sweep.
	/// </summary>
	/// <param name="ampl"></param>
	static void RunSingleFR(double ampl);

	/// <summary>
	///  Returns the state of the analyzer. The state will either be STOPPED or BUSY
	/// </summary>
	/// <returns></returns>
	static AcquisitionState GetAcquisitionState();

	/// <summary>
	/// Retrieves the last collected data. If this is called while the analyzer is busy, the result is undefined. The returned data is a PointF
	/// array of spectrum data, and each point contains the data amplitude (expressed linearly, and referenced to full scale) and data 
	/// frequency. Typically, you will want to convert this data into dB.
	/// </summary>
	/// <param name="channel"></param>
	/// <returns></returns>
	static PointFVector GetData(ChannelType channel);

	/// <summary>
	/// Retrieves the last collected time-domain data. If this is called while the analyzer is busy, the result is undefined. The returned data is a PointF
	/// array of time data, and each point contains the data amplitude (y value, ranging from -1 to 1) and time 
	/// </summary>
	/// <param name="channel"></param>
	/// <returns></returns>
	static PointFVector GetTimeData(ChannelType channel);

	/// <summary>
	/// Given a previous data acquisition, this will compute the power of the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDBOnLastData(ChannelType channel);

	/// <summary>
	/// Given a previous data acquisition, this will compute the power of the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <returns>Computed power in dB</returns>
	static double ComputePowerDBOnLastData(ChannelType channel, double startFreq, double endFreq);

	/// <summary>
	/// Finds the peak and computes the power in presently selected units on the data from the last acquisition.
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <returns>Computed peak power in dB</returns>
	static double ComputePeakPowerDBOnLastData(ChannelType channel);

	/// <summary>
	/// Given a previous data acquisition, this will compute the THD of the data from the last acquisition. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. 
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the THD computation</param>
	/// <returns>THD level in %</returns>
	static double ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq);

	/// Given a previous data acquisition, this will compute the THDN of the data from the last acquisition. The fundamental parameter specifies the target
	/// fundamental, and the max frequency specifies the upper harmonic (in Hertz) that will be considered. As this also contains a noise calculation, the lower frequency bound must also be specified. It is expected
	/// that the minFreq < fundamental < maxFreq
	/// </summary>
	/// <param name="channel">The channel to read the last data from.</param>
	/// <param name="fundamental">The desired fundamental frequency. The level at this frequency will be suppressed in the calculation, while harmonics of this frequency will be used to determine the THD</param>
	/// <param name="maxFreq">Determines the max frequency that will be used for the noise and THD computation</param>
	/// <param name="minFreq">Determines the min freuqency for the noise calculation</param>
	/// <returns>THD level in %</returns>
	static double ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq);

	/// <summary>
	/// Sets the generator to the specified amplitude and frequency. The current units are used. 
	/// </summary>
	/// <param name="gen">Generator 1 or 2</param>
	/// <param name="isOn">Sets on/off state</param>
	/// <param name="amp">Sets amplitude</param>
	/// <param name="freq">Sets frequency. This might be rounded, depending on the host settings</param>
	static void SetGenerator(GenType gen, bool isOn, double amp, double freq);

	/// <summary>
	/// Sets the input and output offsets used in all calculations.
	/// </summary>
	/// <param name="inputOffsets"></param>
	/// <param name="outputOffsets"></param>
	static void SetOffsets(double inputOffsets, double outputOffsets);

	/// <summary>
	/// Sets the units for data
	/// </summary>
	/// <param name="type"></param>
	static void SetUnits(UnitsType type);

	/// <summary>
	/// Sets the length of the in and out sample buffers. The buffer length must be a power of 2 and must be a supported buffer
	/// length. If not a power of two, it will be rounded up to the next power of 2. 
	/// </summary>
	/// <param name="samples"></param>
	static void SetBufferLength(unsigned int samples);

	// Below are factory test interfaces. Do not call these. 
	//static void zzzTestInferface1(int i);
	//static void zzzTestInterface2(UInt16 i);
};
