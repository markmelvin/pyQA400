#pragma once

#include <string>

// .Net System Namespaces
using namespace System;
using namespace System::Collections;
using namespace System::Threading;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;

// C# Projects
using namespace Com::QuantAsylum;


// An application resolver that maintains a list of augmented lookup
// paths for an assembly.
ref class QA400ApplicationResolver
{
public:
	void addToPath(String^ path);
	ArrayList^ resolvedPaths;
	ArrayList^ lookupPaths;

	public: QA400ApplicationResolver()
	{
		resolvedPaths = gcnew ArrayList();
		lookupPaths = gcnew ArrayList();
	}
};


// A managed class providing Singleton access to an assembly involved
// with resolving and finding the QA400 assemblies.
ref class QA400ApplicationResolverSingleton
{
public:
	static QA400ApplicationResolver^ getResolver();
private:
	static QA400ApplicationResolver^ resolver;
	static Assembly^ AssemblyResolveEventHandler(Object^ sender, ResolveEventArgs^ args);
};


// A managed class that holds a Singleton reference to a QA400Interface
// managed object.
ref class QA400Application
{
public:
	static QA400Interface^ getAnalyzer();
private:
	static QA400Interface^ analyzer;
};

//////////////////////////////////////////////////
// String Conversion Functions

inline
String^ ToManagedString(const char* pString) {
	return Marshal::PtrToStringAnsi(IntPtr((char *) pString));
}

inline
const std::string ToStdString(String^ strString) {
	IntPtr ptrString = IntPtr::Zero;
	std::string strStdString;
	try {
		ptrString = Marshal::StringToHGlobalAnsi(strString);
		strStdString = (char *) ptrString.ToPointer();
	}
	finally {
		if (ptrString != IntPtr::Zero) {
			Marshal::FreeHGlobal(ptrString);
		}
	}
	return strStdString;
}
