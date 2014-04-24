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
        AcquisitionState GetAcquisitionState()
        void SetGenerator(GenType gen, bint isOn, double amp, double freq)
        void RunSingle()
        double ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq)
