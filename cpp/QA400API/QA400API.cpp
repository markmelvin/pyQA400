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
		analyzer = (QA400Interface^) QAConnectionManager::ConnectTo(QAConnectionManager::Devices::QA400);
	}
	return analyzer;
}

/// Unmanaged API function bodies (callable from unmanaged C++)
bool QA400API::IsConnected()
{
	return QA400Application::getAnalyzer()->IsConnected();
}

void QA400API::Run()
{
	QA400Application::getAnalyzer()->Run();
}

void QA400API::RunSingle()
{
	QA400Application::getAnalyzer()->RunSingle();
}

QA400API::AcquisitionState QA400API::GetAcquisitionState()
{
	return (QA400API::AcquisitionState) QA400Application::getAnalyzer()->GetAcquisitionState();
}

double QA400API::ComputeTHDNPctOnLastData(ChannelType channel, double fundamental, double minFreq, double maxFreq)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputeTHDNPct(data, fundamental, minFreq, maxFreq);
}

double QA400API::ComputeTHDPctOnLastData(ChannelType channel, double fundamental, double maxFreq)
{
	array<System::Drawing::PointF>^ data = QA400Application::getAnalyzer()->GetData((Com::QuantAsylum::QA400::ChannelType) channel);
	return QA400Application::getAnalyzer()->ComputeTHDPct(data, fundamental, maxFreq);
}

void QA400API::Stop()
{
	QA400Application::getAnalyzer()->Stop();
}

void QA400API::GetName(char *pBuffer, unsigned int length)
{
	strncpy(pBuffer, ToStdString(QA400Application::getAnalyzer()->GetName()).c_str(), length);
	pBuffer[length] = '\0';
}

void QA400API::SetGenerator(GenType gen, bool isOn, double amp, double freq)
{
	QA400Application::getAnalyzer()->SetGenerator((Com::QuantAsylum::QA400::GenType)gen, isOn, amp, freq);
}

unsigned int QA400API::GetNameLength()
{
	return QA400Application::getAnalyzer()->GetName()->Length;
}

void QA400API::LaunchApplicationIfNotRunning()
{
	// Check if the app is running. If not, start it. 
	QAConnectionManager::LaunchAppIfNotRunning(QAConnectionManager::Devices::QA400);
} 