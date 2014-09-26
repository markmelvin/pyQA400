// This is the main DLL file.
#include "stdafx.h"

#include "QA400_managed_api.h"
#include "QA400API.h"

/// Managed class providing access to the managed API
QA400Interface^ QA400Application::getAnalyzer()
{
	if (!QA400Application::analyzer)
	{
		// Retry for a total of 5 seconds
		int numretries = 50;
		QA400API::LaunchApplicationIfNotRunning();
		while (!analyzer && numretries > 0)
		{
			analyzer = (QA400Interface^) QAConnectionManager::ConnectTo(QAConnectionManager::Devices::QA400);
			Thread::Sleep(100);
			numretries--;
		}
	}
	return analyzer;
}

/// Unmanaged API function bodies (callable from unmanaged C++)

/// ----------------------------------------------------------------
void QA400API::LaunchApplicationIfNotRunning()
{
	// Check if the app is running. If not, start it. 
	QAConnectionManager::LaunchAppIfNotRunning(QAConnectionManager::Devices::QA400);
}

/// ----------------------------------------------------------------
void QA400API::AddToSearchPath(char *path)
{
	QAConnectionManager::AddSearchPath(ToManagedString(path));
}

/// ----------------------------------------------------------------
void QA400API::GetName(char *pBuffer, unsigned int length)
{
	System::String^ s = QA400Application::getAnalyzer()->GetName();
	strncpy(pBuffer, ToStdString(s).c_str(), s->Length);
	pBuffer[length] = '\0';
}

/// ----------------------------------------------------------------
unsigned int QA400API::GetNameLength()
{
	return QA400Application::getAnalyzer()->GetName()->Length;
}

/// ----------------------------------------------------------------
bool QA400API::SetToDefault(const char *fileName)
{
	return QA400Application::getAnalyzer()->SetToDefault(ToManagedString(fileName));
}

/// ----------------------------------------------------------------
bool QA400API::IsConnected()
{
	QA400Interface^ analyzer = QA400Application::getAnalyzer();
	if (analyzer)
	{
		return analyzer->IsConnected();
	}
	return false;
}

/// ----------------------------------------------------------------
void QA400API::Run()
{
	QA400Application::getAnalyzer()->Run();
}

/// ----------------------------------------------------------------
void QA400API::Stop()
{
	QA400Application::getAnalyzer()->Stop();
}

/// ----------------------------------------------------------------
void QA400API::RunSingle(double ampl1, double freq1)
{
	QA400Application::getAnalyzer()->RunSingle(ampl1, freq1);
}

/// ----------------------------------------------------------------
void QA400API::RunSingle()
{
	QA400Application::getAnalyzer()->RunSingle();
}

/// ----------------------------------------------------------------
void QA400API::RunSingleFR(double ampl)
{
	QA400Application::getAnalyzer()->RunSingleFR(ampl);
}

/// ----------------------------------------------------------------
QA400API::AcquisitionState QA400API::GetAcquisitionState()
{
	return (QA400API::AcquisitionState) QA400Application::getAnalyzer()->GetAcquisitionState();
}

/// ----------------------------------------------------------------
unsigned int QA400API::GetLastDataLength(ChannelType channel)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return data->Length;
}

/// ----------------------------------------------------------------
unsigned int QA400API::GetLastTimeDataLength(ChannelType channel)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetTimeData((Com::QuantAsylum::QA400::ChannelType) channel);
	return data->Length;
}

/// ----------------------------------------------------------------
void QA400API::GetData(ChannelType channel, PointFVector *data)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	for each (System::Drawing::PointF p in _data)
	{
		curPt->x = p.X;
		curPt->y = p.Y;
		curPt++;
	}
}

/// ----------------------------------------------------------------
void QA400API::GetTimeData(ChannelType channel, PointFVector *data)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = QA400Application::getAnalyzer()->GetTimeData((Com::QuantAsylum::QA400::ChannelType) channel);
	for each (System::Drawing::PointF p in _data)
	{
		curPt->x = p.X;
		curPt->y = p.Y;
		curPt++;
	}
}

/// ----------------------------------------------------------------
double QA400API::ComputePowerDBOnLastData(ChannelType channel)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputePowerDB(data);
}

/// ----------------------------------------------------------------
double QA400API::ComputePowerDB(PointFVector *data)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = gcnew array<System::Drawing::PointF>(data->length);
	for (size_t i = 0; i < data->length; i++)
	{
		_data[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputePowerDB(_data);
}

/// ----------------------------------------------------------------
double QA400API::ComputePowerDBOnLastData(ChannelType channel, double startFreq, double endFreq)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputePowerDB(data, startFreq, endFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputePowerDB(PointFVector *data, double startFreq, double endFreq)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = gcnew array<System::Drawing::PointF>(data->length);
	for (size_t i = 0; i < data->length; i++)
	{
		_data[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputePowerDB(_data, startFreq, endFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputePeakPowerDBOnLastData(ChannelType channel)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputePeakPowerDB(data);
}

/// ----------------------------------------------------------------
double QA400API::ComputePeakPowerDB(PointFVector *data)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = gcnew array<System::Drawing::PointF>(data->length);
	for (size_t i = 0; i < data->length; i++)
	{
		_data[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputePeakPowerDB(_data);
}

/// ----------------------------------------------------------------
double QA400API::ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputeTHDPct(data, fundamental, maxFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputeTHDPct(PointFVector *data, double fundamental, double maxFreq)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = gcnew array<System::Drawing::PointF>(data->length);
	for (size_t i = 0; i < data->length; i++)
	{
		_data[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputeTHDPct(_data, fundamental, maxFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputePhaseOnLastData(ChannelType reference_channel, ChannelType signal_channel, bool applyCompensation, double compensationFreq)
{
	array<System::Drawing::PointF>^ reference_data = QA400Application::getAnalyzer()->GetTimeData((Com::QuantAsylum::QA400::ChannelType) reference_channel);
	array<System::Drawing::PointF>^ signal_data = QA400Application::getAnalyzer()->GetTimeData((Com::QuantAsylum::QA400::ChannelType) signal_channel);
	return QA400Application::getAnalyzer()->ComputePhase(reference_data, signal_data, applyCompensation, compensationFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputePhase(PointFVector *reference, PointFVector *signal, bool applyCompensation, double compensationFreq)
{
	PointF *curPt = reference->values;
	array<System::Drawing::PointF>^ _refdata = gcnew array<System::Drawing::PointF>(reference->length);
	array<System::Drawing::PointF>^ _sigdata = gcnew array<System::Drawing::PointF>(signal->length);
	for (size_t i = 0; i < reference->length; i++)
	{
		_refdata[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	curPt = signal->values;
	for (size_t i = 0; i < signal->length; i++)
	{
		_sigdata[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputePhase(_refdata, _sigdata, applyCompensation, compensationFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputeTHDNPct(data, fundamental, minFreq, maxFreq);
}

/// ----------------------------------------------------------------
double QA400API::ComputeTHDNPct(PointFVector *data, double fundamental, double minFreq, double maxFreq)
{
	PointF *curPt = data->values;
	array<System::Drawing::PointF>^ _data = gcnew array<System::Drawing::PointF>(data->length);
	for (size_t i = 0; i < data->length; i++)
	{
		_data[i] = System::Drawing::PointF(curPt->x, curPt->y);
		curPt++;
	}
	return QA400Application::getAnalyzer()->ComputeTHDNPct(_data, fundamental, minFreq, maxFreq);
}

/// ----------------------------------------------------------------
void QA400API::SetGenerator(GenType gen, bool isOn, double ampl, double freq)
{
	QA400Application::getAnalyzer()->SetGenerator((Com::QuantAsylum::QA400::GenType)gen, isOn, ampl, freq);
}

/// ----------------------------------------------------------------
void QA400API::SetUnits(UnitsType units)
{
	QA400Application::getAnalyzer()->SetUnits((Com::QuantAsylum::QA400::UnitsType)units);
}


