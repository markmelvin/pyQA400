
cdef extern from "QA400API.h":

    ctypedef struct PointF:
        float x
        float y

    ctypedef struct PointFVector:
        unsigned int length
        PointF *values

    enum AcquisitionState "QA400API::AcquisitionState":
        Stopped "QA400API::Stopped"
        Busy "QA400API::Busy"

    enum ChannelType "QA400API::ChannelType":
        LeftIn "QA400API::LeftIn"
        RightIn "QA400API::RightIn"
        LeftOut "QA400API::LeftOut"
        RightOut "QA400API::RightOut"

    enum UnitsType "QA400API::UnitsType":
        dBFS "QA400API::dBFS"
        dBV "QA400API::dBV"

    enum GenType "QA400API::GenType":
        Gen1 "QA400API::Gen1"
        Gen2 "QA400API::Gen2"


cdef extern from "QA400API.h" namespace "QA400API":
    bint AddToSearchPath(char *_path, bint shouldConnect)
    void LaunchApplicationIfNotRunning()
    void GetName(char *pBuffer, unsigned int length)
    unsigned int GetNameLength()
    bint IsConnected()
    bint SetToDefault(char *fileName)
    void Run()
    void Stop()
    void RunSingle(double ampl1, double freq1)
    void RunSingle()
    void RunSingleFR(double ampl)
    AcquisitionState GetAcquisitionState()
    unsigned int GetLastDataLength(ChannelType channel)
    void GetData(ChannelType channel, PointFVector *data)
    unsigned int GetLastTimeDataLength(ChannelType channel)
    void GetTimeData(ChannelType channel, PointFVector *data)
    double ComputePowerDBOnLastData(ChannelType channel)
    double ComputePowerDB(PointFVector *data)
    double ComputePowerDBOnLastData(ChannelType channel, double startFreq, double endFreq)
    double ComputePowerDB(PointFVector *data, double startFreq, double endFreq)
    double ComputePeakPowerDBOnLastData(ChannelType channel)
    double ComputePeakPowerDB(PointFVector *data)
    double ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq)
    double ComputeTHDPct(PointFVector *data, double fundamental, double maxFreq)
    double ComputePhaseOnLastData(ChannelType reference_channel, ChannelType signal_channel, bint applyCompensation, double compensationFreq)
    double ComputePhase(PointFVector *reference, PointFVector *signal, bint applyCompensation, double compensationFreq)
    double ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq)
    double ComputeTHDNPct(PointFVector *data, double fundamental, double minFreq, double maxFreq)
    void SetGenerator(GenType gen, bint isOn, double ampl, double freq)
    void GenerateTone(double ampl, double freq, int durationMS);
    void SetOffsets(double inputOffsets, double outputOffsets)
    void SetUnits(UnitsType type)
    void SetBufferLength(unsigned int samples)
