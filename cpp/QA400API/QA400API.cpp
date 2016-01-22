// This is the main DLL file.
#include "stdafx.h"

#include "QA400_managed_api.h"
#include "QA400API.h"


/// Class providing access to the underlying managed QA400 API
QA400Interface^ QA400Application::getAnalyzer()
{
	// If we have yet to use/find the analyzer - find it
	if (!QA400Application::analyzer)
	{
		// Do we have a custom resolver?
		QA400ApplicationResolver^ resolver = QA400ApplicationResolverSingleton::getResolver();
		if (resolver)
		{
			for each (String^ _p in resolver->resolvedPaths)
			{
				QAConnectionManager::AddSearchPath(_p);
			}
		}

		// Now try to find the executable

		// Retry for a total of 10 seconds
		int numretries = 100;
		QA400API::LaunchApplicationIfNotRunning();
		while (!analyzer && numretries > 0)
		{
			Thread::Sleep(100);
			analyzer = (QA400Interface^) QAConnectionManager::ConnectTo(QAConnectionManager::Devices::QA400);
			numretries--;
		}
	}
	return analyzer;
}


/// Class providing access to the custom assembly resolver which can be augmented
/// with lookup paths to resolve .NET assemblies
QA400ApplicationResolver^ QA400ApplicationResolverSingleton::getResolver()
{
	if (!QA400ApplicationResolverSingleton::resolver)
	{
		// Connect the custom resolver on first access
		QA400ApplicationResolverSingleton::resolver = gcnew QA400ApplicationResolver();
		AppDomain^ currentDomain = AppDomain::CurrentDomain;
		currentDomain->AssemblyResolve += gcnew ResolveEventHandler(QA400ApplicationResolverSingleton::AssemblyResolveEventHandler);
	}
	return resolver;
}


/// Adds the given path to the search path when looking for QAConnectionManager.dll
/// and QAAnalyzer.exe. If you are using this method, it MUST be called as the
/// absolute first thing, before *any* other API call.
void QA400ApplicationResolver::addToPath(String^ path)
{
	// Now add the new path to the lookup paths, if it isn't already there
	if (!QA400ApplicationResolver::lookupPaths->Contains(path))
	{
		QA400ApplicationResolver::lookupPaths->Add(path);
	}
}


/// Implementation of the custom assembly resolver event handler
Assembly^ QA400ApplicationResolverSingleton::AssemblyResolveEventHandler(Object^ sender, ResolveEventArgs^ args)
{
	/* Based on code in http://support2.microsoft.com/kb/837908 */
	//This handler is called only when the common language runtime tries to bind to the assembly and fails.
	resolver = QA400ApplicationResolverSingleton::getResolver();

	//Retrieve the list of referenced assemblies in an array of AssemblyName.
	Assembly^ MyAssembly;
	Assembly^ objExecutingAssemblies = Assembly::GetExecutingAssembly();

	array<AssemblyName^, 1>^ arrReferencedAssmbNames = objExecutingAssemblies->GetReferencedAssemblies();

	//Loop through the array of referenced assembly names.
	for each(AssemblyName^ strAssmbName in arrReferencedAssmbNames)
	{
		// Check for the assembly names that have raised the "AssemblyResolve" event.
		String^ assemblyName = args->Name->Substring(0, args->Name->IndexOf(","));
		if (strAssmbName->FullName->Substring(0, strAssmbName->FullName->IndexOf(",")) == assemblyName)
		{
			for each (String^ _p in resolver->lookupPaths)
			{
				try {
					String^ suffix = ".dll";
					if (assemblyName->Equals("QAAnalyzer"))
						suffix = ".exe";
					String^ strTempAssmbPath = _p + assemblyName + suffix;
					// Try to load the assembly from the specified path
					MyAssembly = Assembly::LoadFrom(strTempAssmbPath);
					if (MyAssembly)
					{
						if (assemblyName->Equals("QAAnalyzer"))
						{
							// If we found the QAAnalyzer.exe executable, add the path for future
							// resolution with and without the 'QA400' suffix.
							if (_p->LastIndexOf("QA400") >= (_p->Length - 6))
							{
								String^ _pathWithoutQA400 = _p->Substring(0, _p->LastIndexOf("QA400"));
								if (!resolver->resolvedPaths->Contains(_pathWithoutQA400))
								{
									resolver->resolvedPaths->Add(_pathWithoutQA400);
								}
							}
						}

						if (!resolver->resolvedPaths->Contains(_p))
						{
							resolver->resolvedPaths->Add(_p);
						}

						return MyAssembly;
					}
				}
				catch (...) {}
			}
		}
	}
	return nullptr;
}


/// Unmanaged API function bodies (callable from unmanaged C++)

/// ----------------------------------------------------------------
/// Initializes the API and launches the application if it is
/// not already running. This should be the first thing you call
/// (after AddToSearchPath(path), if you are augmenting the search path).
/// ----------------------------------------------------------------
bool QA400API::Connect()
{
	QA400ApplicationResolver^ resolver = QA400ApplicationResolverSingleton::getResolver();
	if (resolver)
	{
		// Populate it with some default paths
		resolver->addToPath("C:\\Program Files\\QuantAsylum\\");
		resolver->addToPath("C:\\Program Files (x86)\\QuantAsylum\\");
		resolver->addToPath("C:\\Program Files\\QuantAsylum\\QA400\\");
		resolver->addToPath("C:\\Program Files (x86)\\QuantAsylum\\QA400\\");
		resolver->addToPath("D:\\Program Files\\QuantAsylum\\");
		resolver->addToPath("D:\\Program Files (x86)\\QuantAsylum\\");
		resolver->addToPath("D:\\Program Files\\QuantAsylum\\QA400\\");
		resolver->addToPath("D:\\Program Files (x86)\\QuantAsylum\\QA400\\");
		resolver->addToPath("E:\\Program Files\\QuantAsylum\\");
		resolver->addToPath("E:\\Program Files (x86)\\QuantAsylum\\");
		resolver->addToPath("E:\\Program Files\\QuantAsylum\\QA400\\");
		resolver->addToPath("E:\\Program Files (x86)\\QuantAsylum\\QA400\\");
	}
	return QA400API::IsConnected();
}

/// ----------------------------------------------------------------
void QA400API::LaunchApplicationIfNotRunning()
{
	// Check if the app is running. If not, start it. 
	QAConnectionManager::LaunchAppIfNotRunning(QAConnectionManager::Devices::QA400);
}

/// ----------------------------------------------------------------
void QA400API::AddToSearchPath(char *path)
{
	String^ s = ToManagedString(path);
	if (!s->EndsWith("\\"))
		s += "\\";

	QA400ApplicationResolver^ resolver = QA400ApplicationResolverSingleton::getResolver();
	if (resolver)
		resolver->addToPath(s);
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
void QA400API::GenerateTone(double ampl, double freq, int durationMS)
{
	QA400Application::getAnalyzer()->GenerateTone(ampl, freq, durationMS);
}

/// ----------------------------------------------------------------
void QA400API::SetUnits(UnitsType units)
{
	QA400Application::getAnalyzer()->SetUnits((Com::QuantAsylum::QA400::UnitsType)units);
}


