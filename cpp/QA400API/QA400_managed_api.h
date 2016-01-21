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
	ArrayList^ resolvedPaths = gcnew ArrayList();
	ArrayList^ lookupPaths = gcnew ArrayList();
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


// A managed class that holds a singleton-like reference
// to both a QA400Interface managed object, and provides other
// static methods involved with initialization and
// Assembly resolution.
ref class QA400Application
{
public:
	static QA400Interface^ getAnalyzer();
	static QA400Interface^ getAnalyzer(QA400ApplicationResolver^ resolver);
private:
	static QA400Interface^ analyzer;
	static bool has_augmented_path = false;
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
