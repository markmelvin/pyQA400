// This is the main DLL file.
#include "stdafx.h"

#include "QA400_managed_api.h"
#include "QA400API.h"

/// Managed class providing access to the managed API
QA400Interface^ QA400Application::getAnalyzer()
{
	if (!QA400Application::analyzer)
	{
		QA400API::LaunchApplicationIfNotRunning();
		Thread::Sleep(2000);
		analyzer = (QA400Interface^) QAConnectionManager::ConnectTo(QAConnectionManager::Devices::QA400);
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
void QA400API::GetName(char *pBuffer, unsigned int length)
{
	strncpy(pBuffer, ToStdString(QA400Application::getAnalyzer()->GetName()).c_str(), length);
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
	String ^name = gcnew String(fileName);
	return QA400Application::getAnalyzer()->SetToDefault(name);
}

/// ----------------------------------------------------------------
bool QA400API::IsConnected()
{
	return QA400Application::getAnalyzer()->IsConnected();
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

