cdef extern from "QA400API.h":
    enum AcquisitionState "QA400API::AcquisitionState":
        Stopped "QA400API::Stopped"
        Busy "QA400API::Busy"

    enum ChannelType "QA400API::ChannelType":
        LeftIn "QA400API::LeftIn"
        RightIn "QA400API::RightIn"
        LeftOut "QA400API::LeftOut"
        RightOut "QA400API::RightOut"

    enum UnitsType "QA400API::Unitsype":
        dBFS "QA400API::dBFS"
        dBV "QA400API::dBV"

    enum GenType "QA400API::GenType":
        Gen1 "QA400API::Gen1"
        Gen2 "QA400API::Gen2"


cdef extern from "QA400API.h" namespace "QA400API":
    void LaunchApplicationIfNotRunning()
    void GetName(char *pBuffer, unsigned int length)
    unsigned int GetNameLength()
    bint IsConnected()
    bint SetToDefault(const char *fileName)
    void Run()
    void Stop()
    void RunSingle(double ampl1, double freq1)
    void RunSingle()
    void RunSingleFR(double ampl)
    AcquisitionState GetAcquisitionState()
    double ComputePowerDBOnLastData(ChannelType channel)
    double ComputePowerDBOnLastData(ChannelType channel, double startFreq, double endFreq)
    double ComputePeakPowerDBOnLastData(ChannelType channel)
    double ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq)
    double ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq)
    void SetGenerator(GenType gen, bint isOn, double ampl, double freq)
    void SetOffsets(double inputOffsets, double outputOffsets)
    void SetUnits(UnitsType type)
    void SetBufferLength(unsigned int samples)
